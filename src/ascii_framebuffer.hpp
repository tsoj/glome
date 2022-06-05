#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <cassert>
#include <chrono>
#include <thread>
#include <cstddef>
#include <algorithm>

#include <sys/ioctl.h>
#include <unistd.h>

namespace ascii_framebuffer
{
    struct Color
    {
        unsigned char r;
        unsigned char g;
        unsigned char b;

        friend bool operator==(const Color& lhs, const Color& rhs)
        {
            return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
        }

        friend bool operator!=(const Color& lhs, const Color& rhs)
        {
            return !(lhs == rhs);
        }
    };

    constexpr Color transparent_color = {0, 250, 250};
    constexpr char transparent_ascii = '\r';

    using Style = int8_t;
    enum : Style
    {
        normal, bold, dim, italic, underlined, blinking, reverse, invisible
    };

    class Framebuffer
    {
    private:

        int m_width;
        int m_height;
        std::vector<char> m_char_read_buffer;
        std::vector<char> m_char_write_buffer;
        std::vector<Color> m_text_color_read_buffer;
        std::vector<Color> m_text_color_write_buffer;
        std::vector<Color> m_background_color_read_buffer;
        std::vector<Color> m_background_color_write_buffer;
        std::vector<Style> m_style_read_buffer;
        std::vector<Style> m_style_write_buffer;
        std::thread m_printer_thread;
        std::streambuf* m_original_coutbuf;
        std::stringstream m_redirect_cout;

        [[nodiscard]] size_t getIndex(const int col, const int row) const
        {
            assert(row < m_height && col < m_width && row >= 0 && col >= 0);
            return row * m_width + col;
        }

        void swapBuffers()
        {
            m_char_read_buffer = m_char_write_buffer;
            m_text_color_read_buffer = m_text_color_write_buffer;
            m_background_color_read_buffer = m_background_color_write_buffer;
            m_style_read_buffer = m_style_write_buffer;
        }

        template<typename F, typename T>
        void forBox(const int col, const int row, F set_function, const T& box)
        {
            for (int row_offset = 0; (size_t) row_offset < box.size(); ++row_offset)
            {
                for (int col_offset = 0; (size_t) col_offset < box[row_offset].size(); col_offset++)
                {
                    set_function(col + col_offset, row + row_offset, box[row_offset][col_offset]);
                }
            }
        }

        template<typename F, typename T>
        void fillRectangle(const int col, const int row, const int width, const int height, F set_function, const T& elem)
        {
            for (int row_offset = 0; row_offset < height; ++row_offset)
            {
                for (int col_offset = 0; col_offset < width; col_offset++)
                {
                    set_function(col + col_offset, row + row_offset, elem);
                }
            }
        }

        static inline bool instanced = false;
    public:

        Framebuffer()
        {
            if (instanced)
            {
                throw std::runtime_error(
                    std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " +
                    "Only one instance of 'Framebuffer' allowed.");
            }
            instanced = true;
            m_original_coutbuf = std::cout.rdbuf();
            std::cout.rdbuf(m_redirect_cout.rdbuf());
            winsize w{};
            ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
            m_width = w.ws_col;
            m_height = w.ws_row;
            clear();
        }

        ~Framebuffer()
        {
            if (m_printer_thread.joinable())
            {
                m_printer_thread.join();
            }
            std::cout.rdbuf(m_original_coutbuf);
            std::cout << m_redirect_cout.str() << std::endl;
            instanced = false;
        }

        [[nodiscard]] int width() const
        {
            return m_width;
        }

        [[nodiscard]] int height() const
        {
            return m_height;
        }

        void clear(const Color& background_color, const Color& text_color, const char c, const Style style = normal)
        {
            if (m_printer_thread.joinable())
            {
                m_printer_thread.join();
            }
            m_char_read_buffer = std::vector<char>(m_height * m_width, c);
            m_char_write_buffer = std::vector<char>(m_height * m_width, c);
            m_text_color_read_buffer = std::vector<Color>(m_height * m_width, text_color);
            m_text_color_write_buffer = std::vector<Color>(m_height * m_width, text_color);
            m_background_color_read_buffer = std::vector<Color>(m_height * m_width, background_color);
            m_background_color_write_buffer = std::vector<Color>(m_height * m_width, background_color);
            m_style_read_buffer = std::vector<Style>(m_height * m_width, style);
            m_style_write_buffer = std::vector<Style>(m_height * m_width, style);
        }

        void clear()
        {
            clear({0u, 0u, 0u}, {255u, 255u, 255u}, ' ', normal);
        }

        [[nodiscard]] std::string getCoutHistory() const
        {
            return m_redirect_cout.str();
        }

        void setChar(const int col, const int row, const char c)
        {
            if (c != transparent_ascii)
            {
                m_char_write_buffer.at(getIndex(col, row)) = c;
            }
        }

        void setChar(const int col, const int row, const std::vector<std::string>& box)
        {
            forBox(col, row, [&](auto... a)
            { setChar(a...); }, box);
        }

        void setChar(const int col, const int row, const int width, const int height, const char c)
        {
            fillRectangle(col, row, width, height, [&](auto... a)
            { setChar(a...); }, c);
        }

        void setCoutHistory(const int col, const int row, const int width, const int height)
        {
            if (height <= 0 || width <= 0)
            {
                return;
            }
            std::vector<std::string> lines = {""};
            std::string cout_history = getCoutHistory();
            int line_char_counter = 0;
            for (int i = 0; i < (int) cout_history.size(); ++i)
            {
                if (line_char_counter >= width)
                {
                    cout_history.insert(i, 1, '\n');
                    line_char_counter = 0;
                }
                else
                {
                    if (cout_history.at(i) == '\n')
                    {
                        line_char_counter = 0;
                    }
                    else
                    {
                        line_char_counter += 1;
                    }
                }
            }
            for (int i = (int) cout_history.size() - 1; i >= 0; --i)
            {
                if (cout_history.at(i) == '\n')
                {
                    if ((int) lines.size() == height)
                    {
                        break;
                    }
                    lines.emplace_back("");
                }
                else
                {
                    lines.back() += cout_history.at(i);
                }
            }
            for (auto& line : lines)
            {
                std::reverse(line.begin(), line.end());
            }
            std::reverse(lines.begin(), lines.end());

            setChar(col, row, lines);
        }


        void setTextColor(const int col, const int row, const Color& color)
        {
            if (color != transparent_color)
            {
                m_text_color_write_buffer.at(getIndex(col, row)) = color;
            }
        }

        void setTextColor(const int col, const int row, const std::vector<std::vector<Color>>& box)
        {
            forBox(col, row, [&](auto... a)
            { setTextColor(a...); }, box);
        }

        void setTextColor(const int col, const int row, const int width, const int height, const Color color)
        {
            fillRectangle(col, row, width, height, [&](auto... a)
            { setTextColor(a...); }, color);
        }


        void setBackgroundColor(const int col, const int row, const Color& color)
        {
            if (color != transparent_color)
            {
                m_background_color_write_buffer.at(getIndex(col, row)) = color;
            }
        }

        void setBackgroundColor(const int col, const int row, const std::vector<std::vector<Color>>& box)
        {
            forBox(col, row, [&](auto... a)
            { setBackgroundColor(a...); }, box);
        }

        void setBackgroundColor(const int col, const int row, const int width, const int height, const Color color)
        {
            fillRectangle(col, row, width, height, [&](auto... a)
            { setBackgroundColor(a...); }, color);
        }


        void setStyle(const int col, const int row, const Style& style)
        {
            m_style_write_buffer.at(getIndex(col, row)) = style;
        }

        void setStyle(const int col, const int row, const std::vector<std::vector<Style>>& box)
        {
            forBox(col, row, [&](auto... a)
            { setStyle(a...); }, box);
        }

        void setStyle(const int col, const int row, const int width, const int height, const Style style)
        {
            fillRectangle(col, row, width, height, [&](auto... a)
            { setStyle(a...); }, style);
        }


        [[nodiscard]] char getChar(const int col, const int row) const
        {
            return m_char_read_buffer.at(getIndex(col, row));
        }

        [[nodiscard]] Color getTextColor(const int col, const int row) const
        {
            return m_text_color_read_buffer.at(getIndex(col, row));
        }

        [[nodiscard]] Color getBackgroundColor(const int col, const int row) const
        {
            return m_background_color_read_buffer.at(getIndex(col, row));
        }

        [[nodiscard]] Style getStyle(const int col, const int row) const
        {
            return m_style_read_buffer.at(getIndex(col, row));
        }

        void print()
        {
            const auto printer = [this]()
            {

                std::stringstream output;
                output << "\033[0;0H"; // move cursor to the top left corner
                for (int row = 0; row < m_height; row++)
                {
                    for (int col = 0; col < m_width; col++)
                    {
                        const Color text_color = getTextColor(col, row);
                        const Color background_color = getBackgroundColor(col, row);
                        const Style style = getStyle(col, row);
                        output
                            << "\033[" << (int) style << "m"
                            << "\033[38;2;" // ESC[ 38;2;⟨r⟩;⟨g⟩;⟨b⟩ select RGB foreground color
                            << static_cast<int>(text_color.r) << ";"
                            << static_cast<int>(text_color.g) << ";"
                            << static_cast<int>(text_color.b) << "m"
                            << "\033[48;2;" // ESC[ 48;2;⟨r⟩;⟨g⟩;⟨b⟩ select RGB background color
                            << static_cast<int>(background_color.r) << ";"
                            << static_cast<int>(background_color.g) << ";"
                            << static_cast<int>(background_color.b) << "m"
                            << getChar(col, row);
                    }
                }
                std::cout << std::flush;
                std::cout.rdbuf(m_original_coutbuf);
                std::cout << output.rdbuf() << std::flush;
                std::cout.rdbuf(m_redirect_cout.rdbuf());
            };

            if (m_printer_thread.joinable())
            {
                m_printer_thread.join();
            }
            swapBuffers();
            m_printer_thread = std::thread(printer);
        }
    };
}

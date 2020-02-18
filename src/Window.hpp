#pragma once

#include "gl.hpp"

#include <GLFW/glfw3.h>
#include <utility>
#include <functional>

class Window
{
public:
    Window(int w, int h, const char* title);

    Window() = default;

    ~Window()
    {
        glfwDestroyWindow(m_glfw_window);
    }

    Window(const Window&) = delete;

    Window& operator=(const Window&) = delete;

    void makeContextCurrent()
    {
        glfwMakeContextCurrent(m_glfw_window);
    }

    void swapBuffers()
    {
        glfwSwapBuffers(m_glfw_window);
    }

    [[nodiscard]] bool shouldClose() const
    {
        return glfwWindowShouldClose(m_glfw_window);
    }

    void setTitle(const std::string& title)
    {
        glfwSetWindowTitle(m_glfw_window, title.c_str());
    }

    [[nodiscard]] int width() const
    {
        int width, height;
        glfwGetFramebufferSize(m_glfw_window, &width, &height);
        return width;
    }

    [[nodiscard]] int height() const
    {
        int width, height;
        glfwGetFramebufferSize(m_glfw_window, &width, &height);
        return height;
    }

    void drawImage(const gl::Texture2D& image);

    [[nodiscard]] bool isKeyPressed(int key);

private:
    GLFWwindow* m_glfw_window = nullptr;

    struct RaiiInitGlfw
    {
        RaiiInitGlfw()
        {
            glfwSetErrorCallback(
                [](int error, const char* description)
                {
                    throw std::runtime_error("Error: " + std::string(description) + " (" + std::to_string(error) + ")\n");
                }
            );

            if (!glfwInit())
            {
                throw std::runtime_error("Failed to initialize GLFW.\n");
            }
        }

        ~RaiiInitGlfw()
        {
            glfwTerminate();
        }
    };

    inline static const RaiiInitGlfw init_glfw = RaiiInitGlfw();
};

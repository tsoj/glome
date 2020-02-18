#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <map>
#include <tuple>
#include <filesystem>


#ifndef NDEBUG
#define USE_SHADER_PRINTF
#endif

#ifdef USE_SHADER_PRINTF

#include <shader-printf/shaderprintf.h>

#endif

namespace gl
{

    namespace details
    {
        struct GlBufferTraits
        {
            static GLuint create()
            {
                GLuint id;
                glGenBuffers(1, &id);
                return id;
            }

            static void destroy(const GLuint id)
            {
                glDeleteBuffers(1, &id);
            }
        };

        struct GlProgramTraits
        {
            static GLuint create()
            {
                return glCreateProgram();
            }

            static void destroy(const GLuint id)
            {
                glDeleteProgram(id);
            }
        };

        struct GlVertexArrayTraits
        {
            static GLuint create()
            {
                GLuint id;
                glGenVertexArrays(1, &id);
                return id;
            }

            static void destroy(const GLuint id)
            {
                glDeleteVertexArrays(1, &id);
            }
        };

        struct GlTextureTraits
        {
            static GLuint create()
            {
                GLuint id;
                glGenTextures(1, &id);
                return id;
            }

            static void destroy(const GLuint id)
            {
                glDeleteTextures(1, &id);
            }
        };

        struct GlRenderbufferTraits
        {
            static GLuint create()
            {
                GLuint id;
                glGenRenderbuffers(1, &id);
                return id;
            }

            static void destroy(const GLuint id)
            {
                glDeleteRenderbuffers(1, &id);
            }
        };

        struct GlFramebufferTraits
        {
            static GLuint create()
            {
                GLuint id;
                glGenFramebuffers(1, &id);
                return id;
            }

            static void destroy(const GLuint id)
            {
                glDeleteFramebuffers(1, &id);
            }
        };

        template<typename T>
        class GlObject
        {
        public:
            GlObject() :
                m_copy_counter(new size_t(1)),
                m_id(T::create())
            {}

            ~GlObject()
            {
                destroy();
            }

            // copy constructor
            GlObject(const GlObject<T>& other) :
                m_copy_counter(other.m_copy_counter),
                m_id(other.m_id)
            {
                *m_copy_counter += 1;
            }

            // copy operator=
            GlObject<T>& operator=(const GlObject<T>& other)
            {
                if (&other != this)
                {
                    destroy();
                    m_id = other.m_id;
                    m_copy_counter = other.m_copy_counter;
                    *m_copy_counter += 1;
                }
                return *this;
            }

            // move constructor
            GlObject(GlObject<T>&& other) = delete;

            // move operator=
            GlObject<T>& operator=(GlObject<T>&& other) = delete;

            [[nodiscard]] GLuint id() const
            {
                return m_id;
            }

        private:

            void destroy()
            {
                assert(m_copy_counter != nullptr);
                assert(*m_copy_counter > 0);
                *m_copy_counter -= 1;
                if (*m_copy_counter == 0)
                {
                    T::destroy(m_id);
                    m_id = 0;
                    delete m_copy_counter;
                }
            }

            size_t* m_copy_counter = nullptr;

            GLuint m_id = 0;
        };


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-but-set-parameter"

        template<size_t i, size_t size, typename F, typename... T>
        void tupleForeachConstexpr(const std::tuple<T...>& tuple, F func)
        {
            if constexpr(i < size)
            {
                func(std::get<i>(tuple));
                tupleForeachConstexpr<i + 1, size, F, T...>(tuple, func);
            }
        }

#pragma GCC diagnostic pop

        template<typename F, typename... T>
        void tupleForeachConstexpr(const std::tuple<T...>& tuple, F func)
        {
            tupleForeachConstexpr<0, std::tuple_size<std::tuple<T...>>::value, F, T...>(tuple, func);
        }

        template<typename>
        struct IsTuple : std::false_type
        {
        };
        template<typename ...T>
        struct IsTuple<std::tuple<T...>> : std::true_type
        {
        };

        void checkForErrors(const char* filename = nullptr, int line = INT_MAX);
    }

    class Texture2D
    {
        friend class Framebuffer;

        friend class Program;

    public:
        Texture2D(
            GLint internal_format,
            GLsizei width,
            GLsizei height,
            GLenum format,
            GLenum type
        );

        explicit Texture2D(const std::filesystem::path& file_path);


        Texture2D() = default;

        void setParameter(GLenum pname, GLint param);

    private:
        details::GlObject<details::GlTextureTraits> m_object;
    };

    class Texture2DArray
    {
        friend class Framebuffer;

        friend class Program;

    public:
        Texture2DArray(
            GLenum internal_format,
            GLsizei width,
            GLsizei height,
            GLsizei depth
        );

        Texture2DArray() = default;

        void setParameter(GLenum pname, GLint param);

        void setParameter(GLenum pname, GLfloat param);

    private:
        details::GlObject<details::GlTextureTraits> m_object;
    };

    class Renderbuffer
    {
        friend class Framebuffer;

    public:
        Renderbuffer(GLenum internal_format, GLsizei width, GLsizei height);

        Renderbuffer() = default;

    private:
        details::GlObject<details::GlRenderbufferTraits> m_object;
    };

    class Framebuffer
    {
    public:
        Framebuffer();

        void attach(const Texture2D& texture, GLenum attachment);

        void attach(const Texture2DArray& texture_array, GLenum attachment, GLint layer);

        void attach(const Renderbuffer& renderbuffer, GLenum attachment);

        void setDrawBuffer(const std::initializer_list<GLenum>& attachment);

        void bind();

    private:
        details::GlObject<details::GlFramebufferTraits> m_object;
    };

    class VertexArray
    {
        friend class Program;

    public:
        // TODO: Class 3: redesign, use differend buffer objects for (vertices) and (texture coordinates, normals, tangents)
        template<typename T>
        explicit VertexArray(const std::vector<T>& vertices)
        {
            assert(m_object.id() != 0);
            assert(m_buffer_object.id() != 0);

            m_num_vertices = vertices.size();

            glBindVertexArray(m_object.id());

            glBindBuffer(GL_ARRAY_BUFFER, m_buffer_object.id());

            glBufferData(GL_ARRAY_BUFFER, sizeof(T) * vertices.size(), vertices.data(), GL_STATIC_DRAW);
        }

        VertexArray() = default;

        void setVertexAttribPointer(
            GLuint index,
            GLint size,
            GLenum type,
            GLboolean normalized,
            size_t size_of_type,
            size_t offset
        );

        static VertexArray rectangleVao();

    private:
        size_t m_num_vertices{};
        details::GlObject<details::GlVertexArrayTraits> m_object;
        details::GlObject<details::GlBufferTraits> m_buffer_object;
    };

    class Program
    {
        static std::string preprocessShader(
            const std::filesystem::path& file_path,
            const std::map<std::string, int>& insert_constants,
            const std::string& gl_version);

        static std::string getFormattedCode(const std::string& code);

#ifdef USE_SHADER_PRINTF
        static inline const char* default_gl_version = "430";
#else
        static inline const char* default_gl_version = "330";
#endif

    public:

        Program(
            const std::initializer_list<std::tuple<std::filesystem::path, GLenum>>& shader_list,
            const std::map<std::string, int>& insert_constants = std::map<std::string, int>(),
            const std::string& gl_version = default_gl_version
        );

        Program() = default;

        void uniform(const std::string& name, const glm::mat4& value);

        void uniform(const std::string& name, const glm::mat3& value);

        void uniform(const std::string& name, const GLint& value);

        void uniform(const std::string& name, const GLfloat& value);

        void uniform(const std::string& name, const glm::vec3& value);

        void uniform(const std::string& name, const glm::vec4& value);

        void uniform(const std::string& name, GLsizei count, const glm::mat4* value);

        void uniform(const std::string& name, GLsizei count, const GLint* value);

        void uniform(const std::string& name, GLsizei count, const GLfloat* value);

        void uniform(const std::string& name, GLsizei count, const glm::vec3* value);

        void uniform(const std::string& name, GLsizei count, const glm::vec4* value);

        void uniform(const std::string& name, const Texture2D& texture);

        void uniform(const std::string& name, const Texture2DArray& texture_array);

        void draw(const VertexArray& vertex_array, GLenum mode);

    private:

        void use();

        GLint getUniformLocation(const std::string& name);

        std::map<std::string, GLint> m_uniform_locations;
        details::GlObject<details::GlProgramTraits> m_object;
        int m_texture_unit_counter = 0;
        inline static GLuint s_active_program_id = 0;
        std::map<GLenum, std::pair<std::filesystem::path, std::string>> m_sources;
    };

#define TYPEOF(expression) typename std::remove_const<typename std::remove_reference<decltype(expression)>::type>::type

    template<typename T_MeshData, typename... T_GeneralUniforms, typename... T_SpecificUniforms>
    void renderPass
        (
            const std::vector<T_MeshData>& mesh_vector,
            const gl::VertexArray T_MeshData::*vao,
            gl::Framebuffer* render_target,
            const GLint viewport_x, const GLint viewport_y,
            const GLsizei viewport_width, const GLsizei viewport_height,
            const GLbitfield clear_bits,
            const GLenum mode,
            gl::Program& program,
            const std::tuple<T_GeneralUniforms...>& general_uniforms,
            const std::tuple<T_SpecificUniforms...>& specific_uniforms
        )
    {
        glViewport(viewport_x, viewport_y, viewport_width, viewport_height);
        if (render_target == nullptr)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
        else
        {
            render_target->bind();
        }
        glClear(clear_bits);

        details::tupleForeachConstexpr(general_uniforms, [&program](const auto& x)
        {
            static_assert(details::IsTuple<TYPEOF(x)>::value, "expecting a std::tuple of size 2 or 3");
            static_assert(std::tuple_size<TYPEOF(x)>::value == 2 || std::tuple_size<TYPEOF(x)>::value == 3,
                          "tuple needs to have 2 or 3 elements");

            if constexpr(std::tuple_size<TYPEOF(x)>::value == 3)
            {
                program.uniform(std::get<0>(x), std::get<1>(x), std::get<2>(x));
            }
            else if constexpr(std::is_pointer<TYPEOF(std::get<1>(x))>::value)
            {
                program.uniform(std::get<0>(x), *std::get<1>(x));
            }
            else
            {
                program.uniform(std::get<0>(x), std::get<1>(x));
            }

        });

        for (const T_MeshData& mesh : mesh_vector)
        {
            details::tupleForeachConstexpr(specific_uniforms, [&program, &mesh](const auto& x)
            {
                static_assert(details::IsTuple<TYPEOF(x)>::value, "expecting a std::tuple of size 2 or 3");
                static_assert(std::tuple_size<TYPEOF(x)>::value == 2 || std::tuple_size<TYPEOF(x)>::value == 3,
                              "tuple needs to have 2 or 3 elements");
                if constexpr(std::tuple_size<TYPEOF(x)>::value == 3)
                {
                    if constexpr (!std::is_pointer<TYPEOF(mesh.*std::get<2>(x))>::value)
                    {
                        program.uniform(std::get<0>(x), std::get<1>(x), &(mesh.*std::get<2>(x)));
                    }
                    else
                    {
                        program.uniform(std::get<0>(x), std::get<1>(x), mesh.*std::get<2>(x));
                    }
                }
                else if constexpr(std::is_pointer<TYPEOF(mesh.*std::get<1>(x))>::value)
                {
                    program.uniform(std::get<0>(x), *(mesh.*std::get<1>(x)));
                }
                else
                {
                    program.uniform(std::get<0>(x), mesh.*std::get<1>(x));
                }
            });

            program.draw(mesh.*vao, mode);
        }
    }

#undef TYPEOF

#define CHECK_FOR_GL_ERRORS() details::checkForErrors(__FILE__, __LINE__)


    void GLAPIENTRY debugOutput(
        GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* message, const void* user_param
    );

}
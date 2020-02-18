#include "gl.hpp"
#include "utility.hpp"
#include <fstream>

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wignored-qualifiers"
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wpedantic"
#pragma GCC diagnostic ignored "-Wshadow"
#pragma GCC diagnostic ignored "-Wempty-body"
#pragma GCC diagnostic ignored "-Wunused-variable"

#include <gli/gli.hpp>

#pragma GCC diagnostic pop

namespace gl
{

    namespace details
    {
        //https://github.com/g-truc/gli/blob/0.8.2/manual.md#section2_2
        void createTexture(const std::filesystem::path& file_path, const GLuint texture_name)
        {
            gli::texture texture = gli::load(file_path.c_str());
            if (texture.empty())
                throw std::runtime_error("Failed to open file: " + file_path.string());

            gli::gl gl(gli::gl::PROFILE_GL33);
            gli::gl::format const format = gl.translate(texture.format(), texture.swizzles());
            GLenum target = gl.translate(texture.target());

            glBindTexture(target, texture_name);
            glTexParameteri(target, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(target, GL_TEXTURE_MAX_LEVEL, static_cast<GLint>(texture.levels() - 1));
            glTexParameteri(target, GL_TEXTURE_SWIZZLE_R, format.Swizzles[0]);
            glTexParameteri(target, GL_TEXTURE_SWIZZLE_G, format.Swizzles[1]);
            glTexParameteri(target, GL_TEXTURE_SWIZZLE_B, format.Swizzles[2]);
            glTexParameteri(target, GL_TEXTURE_SWIZZLE_A, format.Swizzles[3]);

            glm::tvec3<GLsizei> const extent(texture.extent());
            auto const face_total = static_cast<GLsizei>(texture.layers() * texture.faces());

            switch (texture.target())
            {
                case gli::TARGET_1D:
                    glTexStorage1D(
                        target, static_cast<GLint>(texture.levels()), format.Internal, extent.x);
                    break;
                case gli::TARGET_1D_ARRAY:
                case gli::TARGET_2D:
                case gli::TARGET_CUBE:
                    glTexStorage2D(
                        target, static_cast<GLint>(texture.levels()), format.Internal,
                        extent.x, texture.target() == gli::TARGET_2D ? extent.y : face_total);
                    break;
                case gli::TARGET_2D_ARRAY:
                case gli::TARGET_3D:
                case gli::TARGET_CUBE_ARRAY:
                    glTexStorage3D(
                        target, static_cast<GLint>(texture.levels()), format.Internal,
                        extent.x, extent.y,
                        texture.target() == gli::TARGET_3D ? extent.z : face_total);
                    break;
                default:
                    assert(0);
                    break;
            }

            for (std::size_t layer = 0; layer < texture.layers(); ++layer)
            {
                for (std::size_t face = 0; face < texture.faces(); ++face)
                {
                    for (std::size_t level = 0; level < texture.levels(); ++level)
                    {
                        auto const layer_gl = static_cast<GLsizei>(layer);
                        glm::tvec3<GLsizei> layer_extent(texture.extent(level));
                        target = gli::is_target_cube(texture.target())
                                 ? static_cast<GLenum>(GL_TEXTURE_CUBE_MAP_POSITIVE_X + face)
                                 : target;

                        switch (texture.target())
                        {
                            case gli::TARGET_1D:
                                if (gli::is_compressed(texture.format()))
                                    glCompressedTexSubImage1D(
                                        target, static_cast<GLint>(level), 0, layer_extent.x,
                                        format.Internal, static_cast<GLsizei>(texture.size(level)),
                                        texture.data(layer, face, level));
                                else
                                    glTexSubImage1D(
                                        target, static_cast<GLint>(level), 0, layer_extent.x,
                                        format.External, format.Type,
                                        texture.data(layer, face, level));
                                break;
                            case gli::TARGET_1D_ARRAY:
                            case gli::TARGET_2D:
                            case gli::TARGET_CUBE:
                                if (gli::is_compressed(texture.format()))
                                    glCompressedTexSubImage2D(
                                        target, static_cast<GLint>(level),
                                        0, 0,
                                        layer_extent.x,
                                        texture.target() == gli::TARGET_1D_ARRAY ? layer_gl : layer_extent.y,
                                        format.Internal, static_cast<GLsizei>(texture.size(level)),
                                        texture.data(layer, face, level));
                                else
                                    glTexSubImage2D(
                                        target, static_cast<GLint>(level),
                                        0, 0,
                                        layer_extent.x,
                                        texture.target() == gli::TARGET_1D_ARRAY ? layer_gl : layer_extent.y,
                                        format.External, format.Type,
                                        texture.data(layer, face, level));
                                break;
                            case gli::TARGET_2D_ARRAY:
                            case gli::TARGET_3D:
                            case gli::TARGET_CUBE_ARRAY:
                                if (gli::is_compressed(texture.format()))
                                    glCompressedTexSubImage3D(
                                        target, static_cast<GLint>(level),
                                        0, 0, 0,
                                        layer_extent.x, layer_extent.y,
                                        texture.target() == gli::TARGET_3D ? layer_extent.z : layer_gl,
                                        format.Internal, static_cast<GLsizei>(texture.size(level)),
                                        texture.data(layer, face, level));
                                else
                                    glTexSubImage3D(
                                        target, static_cast<GLint>(level),
                                        0, 0, 0,
                                        layer_extent.x, layer_extent.y,
                                        texture.target() == gli::TARGET_3D ? layer_extent.z : layer_gl,
                                        format.External, format.Type,
                                        texture.data(layer, face, level));
                                break;
                            default:
                                assert(0);
                                break;
                        }
                    }
                }
            }
        }
    }

    Texture2D::Texture2D(
        const GLint internal_format,
        const GLsizei width,
        const GLsizei height,
        const GLenum format,
        const GLenum type
    )
    {
        assert(m_object.id() != 0);
        glBindTexture(GL_TEXTURE_2D, m_object.id());
        glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width, height, 0, format, type, nullptr);
    }

    Texture2D::Texture2D(const std::filesystem::path& file_path)
    {
        assert(m_object.id() != 0);
        details::createTexture(file_path, m_object.id());
    }

    void Texture2D::setParameter(const GLenum pname, const GLint param)
    {
        glBindTexture(GL_TEXTURE_2D, m_object.id());
        glTexParameteri(GL_TEXTURE_2D, pname, param);
    }

    Texture2DArray::Texture2DArray(
        const GLenum internal_format,
        const GLsizei width,
        const GLsizei height,
        const GLsizei depth
    )
    {
        assert(m_object.id() != 0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_object.id());
        glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, internal_format, width, height, depth);
    }

    void Texture2DArray::setParameter(const GLenum pname, const GLint param)
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_object.id());
        glTexParameteri(GL_TEXTURE_2D_ARRAY, pname, param);
    }

    void Texture2DArray::setParameter(const GLenum pname, const GLfloat param)
    {
        glBindTexture(GL_TEXTURE_2D_ARRAY, m_object.id());
        glTexParameterf(GL_TEXTURE_2D_ARRAY, pname, param);
    }

    Renderbuffer::Renderbuffer(const GLenum internal_format, const GLsizei width, const GLsizei height)
    {
        assert(m_object.id() != 0);
        glBindRenderbuffer(GL_RENDERBUFFER, m_object.id());
        glRenderbufferStorage(GL_RENDERBUFFER, internal_format, width, height);
    }

    Framebuffer::Framebuffer()
    {
        assert(m_object.id() != 0);
        glBindFramebuffer(GL_FRAMEBUFFER, m_object.id());
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
    }

    void Framebuffer::attach(const Texture2D& texture, const GLenum attachment)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_object.id());
        glFramebufferTexture2D(GL_FRAMEBUFFER, attachment, GL_TEXTURE_2D, texture.m_object.id(), 0);
    }

    void Framebuffer::attach(const Texture2DArray& texture_array, const GLenum attachment, const GLint layer)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_object.id());
        glFramebufferTextureLayer(GL_FRAMEBUFFER, attachment, texture_array.m_object.id(), 0, layer);
    }

    void Framebuffer::attach(const Renderbuffer& renderbuffer, const GLenum attachment)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_object.id());
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, attachment, GL_RENDERBUFFER, renderbuffer.m_object.id());
    }

    void Framebuffer::setDrawBuffer(const std::initializer_list<GLenum>& attachment)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_object.id());
        glDrawBuffers(attachment.size(), attachment.begin());
    }

    void Framebuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_object.id());
    }

    void VertexArray::setVertexAttribPointer(
        const GLuint index,
        const GLint size,
        const GLenum type,
        const GLboolean normalized,
        const size_t size_of_type,
        const size_t offset
    )
    {
        glBindVertexArray(m_object.id());
        glEnableVertexAttribArray(index);
        glVertexAttribPointer(
            index,
            size,
            type,
            normalized,
            size_of_type,
            (void*) offset
        );
    }

    VertexArray VertexArray::rectangleVao()
    {
        static const std::vector<glm::vec2> vertices =
            {
                glm::vec2(-1.0, 1.0), glm::vec2(0.0, 1.0),
                glm::vec2(-1.0, -1.0), glm::vec2(0.0, 0.0),
                glm::vec2(1.0, -1.0), glm::vec2(1.0, 0.0),

                glm::vec2(-1.0, 1.0), glm::vec2(0.0, 1.0),
                glm::vec2(1.0, -1.0), glm::vec2(1.0, 0.0),
                glm::vec2(1.0, 1.0), glm::vec2(1.0, 1.0)
            };
        static VertexArray vao = VertexArray(vertices);
        __attribute__((unused)) static const auto nothing = [&]()
        {
            vao.setVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, 0);
            vao.setVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2) * 2, sizeof(glm::vec2));
            return 0;
        }();
        return vao;
    }

    std::string Program::preprocessShader(
        const std::filesystem::path& file_path,
        const std::map<std::string, int>& insert_constants,
        const std::string& gl_version)
    {
        std::string ret;

        std::ifstream file;
        file.open(file_path);
        if (!file.is_open())
        {
            throw std::runtime_error("Failed to open file: " + file_path.string());
        }
        std::string line;
        while (std::getline(file, line))
        {
            const size_t l = std::string_view("#include_glsl").size();
            if (line.substr(0, l) == "#include_glsl")
            {
                std::string include_path = line.substr(l, line.size() - l);
                include_path.erase(std::remove(include_path.begin(), include_path.end(), '"'), include_path.end());
                while (!include_path.empty() && include_path[0] == ' ')
                {
                    include_path.erase(include_path.begin());
                }
                while (!include_path.empty() && include_path.back() == ' ')
                {
                    include_path.erase(include_path.end() - 1);
                }
                if (include_path.empty())
                {
                    throw std::runtime_error("Misformatted #include_glsl directive: " + line);
                }
                ret += "//Begin include from: " + include_path + "\n";
                ret += preprocessShader(include_path, insert_constants, gl_version) + "\n";
                ret += "//End include from: " + include_path + "\n";
            }
            else if (line.substr(0, std::string("#insert").size()) == "#insert")
            {
                std::vector<std::string> words = utility::split(line, ' ');
                assert(words[0] == "#insert");

                if (words.size() != 2)
                {
                    throw std::runtime_error("Misformatted #insert directive: " + line);
                }
                if (insert_constants.count(words[1]) == 0)
                {
                    throw std::runtime_error("Couldn't find '" + words[1] + "' to insert into line: " + line);
                }
                ret += "#ifndef " + words[1] + "\n";
                ret += "//Inserted constant\n";
                ret += "#define " + words[1] + " " + std::to_string(insert_constants.at(words[1])) + "\n";
                ret += "#endif\n";
            }
            else if (line.substr(0, std::string("#version").size()) == "#version")
            {
                ret += "#version " + gl_version + "\n";
            }
#ifndef USE_SHADER_PRINTF
                else if (
                        line.find("printf") != std::string::npos ||
                        line.find("enablePrintf") != std::string::npos ||
                        line.find("disablePrintf") != std::string::npos
                        )
                    {
                        ret += "/* Info: shader-printf functions ('printf(...)', 'enablePrintf()',\n"
                               "'disablePrintf()') need to be on a single line with no other statements.*/\n";
                    }
#endif
            else
            {
                ret += line + "\n";
            }
        }
        file.close();

        return ret;
    }

    std::string Program::getFormattedCode(const std::string& code)
    {
        std::string ret = "-------------------------------------------------\n";
        std::stringstream ss(code);
        std::string s;
        int line_counter = 1;
        while (std::getline(ss, s, '\n'))
        {
            ret += std::to_string(line_counter) + ":  \t" + s + "\n";
            line_counter += 1;
        }
        ret += "-------------------------------------------------";
        return ret;
    }

    Program::Program(
        const std::initializer_list<std::tuple<std::filesystem::path, GLenum>>& shader_list,
        const std::map<std::string, int>& insert_constants,
        const std::string& gl_version
    )
    {
        assert(m_object.id() != 0);

        struct Shader
        {
            std::filesystem::path file_path;
            std::string code;
            GLuint id;
        };
        std::vector<Shader> shader_ids;

        for (const auto& shader : shader_list)
        {
            const GLuint shader_id = glCreateShader(std::get<1>(shader));
            std::string shader_code = preprocessShader(std::get<0>(shader), insert_constants, gl_version);
            const char* adapter = shader_code.c_str();
#ifdef USE_SHADER_PRINTF
            glShaderSourcePrint(shader_id, 1, &adapter, nullptr);
#else
            glShaderSource(shader_id, 1, &adapter, nullptr);
#endif
            glCompileShader(shader_id);
            glAttachShader(m_object.id(), shader_id);
            shader_ids.push_back(Shader{std::get<0>(shader), shader_code, shader_id});

            m_sources[std::get<1>(shader)] = std::make_pair(std::get<0>(shader), shader_code);
        }

        glLinkProgram(m_object.id());

        std::string error;

        for (const auto& shader : shader_ids)
        {
            GLint success = 0;
            glGetShaderiv(shader.id, GL_COMPILE_STATUS, &success);
            if (success == GL_FALSE)
            {
                GLint max_length = 0;
                glGetShaderiv(shader.id, GL_INFO_LOG_LENGTH, &max_length);
                std::vector<GLchar> error_log(max_length);

                glGetShaderInfoLog(shader.id, max_length, &max_length, &error_log[0]);

                error += "Failed to compile " + shader.file_path.string() + ":\n" +
                         getFormattedCode(shader.code) + "\n" +
                         std::string(error_log.data()) + "\n";
            }

            glDetachShader(m_object.id(), shader.id);
            glDeleteShader(shader.id);
        }

        GLint is_linked = 0;
        glGetProgramiv(m_object.id(), GL_LINK_STATUS, (int*) &is_linked);
        if (is_linked == GL_FALSE)
        {
            GLint max_length = 0;
            glGetProgramiv(m_object.id(), GL_INFO_LOG_LENGTH, &max_length);
            std::vector<GLchar> error_log(max_length);

            glGetProgramInfoLog(m_object.id(), max_length, &max_length, &error_log[0]);

            error += "Failed to link shader program:\n" + std::string(error_log.data()) + "\n";
        }

        if (!error.empty())
        {
            throw std::runtime_error("\n" + error);
        }
    }

    void Program::uniform(const std::string& name, const glm::mat4& value)
    {
        use();
        glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }

    void Program::uniform(const std::string& name, const glm::mat3& value)
    {
        use();
        glUniformMatrix3fv(getUniformLocation(name), 1, GL_FALSE, &value[0][0]);
    }

    void Program::uniform(const std::string& name, const GLint& value)
    {
        use();
        glUniform1i(getUniformLocation(name), value);
    }

    void Program::uniform(const std::string& name, const GLfloat& value)
    {
        use();
        glUniform1f(getUniformLocation(name), value);
    }

    void Program::uniform(const std::string& name, const glm::vec3& value)
    {
        use();
        glUniform3fv(getUniformLocation(name), 1, &value[0]);
    }

    void Program::uniform(const std::string& name, const glm::vec4& value)
    {
        use();
        glUniform4fv(getUniformLocation(name), 1, &value[0]);
    }

    void Program::uniform(const std::string& name, const GLsizei count, const glm::mat4* value)
    {
        use();
        glUniformMatrix4fv(getUniformLocation(name), count, GL_FALSE, &value[0][0][0]);
    }

    void Program::uniform(const std::string& name, const GLsizei count, const GLint* value)
    {
        use();
        glUniform1iv(getUniformLocation(name), count, &value[0]);
    }

    void Program::uniform(const std::string& name, const GLsizei count, const GLfloat* value)
    {
        use();
        glUniform1fv(getUniformLocation(name), count, &value[0]);
    }

    void Program::uniform(const std::string& name, const GLsizei count, const glm::vec3* value)
    {
        use();
        glUniform3fv(getUniformLocation(name), count, &value[0][0]);
    }

    void Program::uniform(const std::string& name, const GLsizei count, const glm::vec4* value)
    {
        use();
        glUniform4fv(getUniformLocation(name), count, &value[0][0]);
    }

    void Program::uniform(const std::string& name, const Texture2D& texture)
    {
        use();
        uniform(name, m_texture_unit_counter);
        glActiveTexture(GL_TEXTURE0 + m_texture_unit_counter);
        glBindTexture(GL_TEXTURE_2D, texture.m_object.id());
        m_texture_unit_counter += 1;
    }

    void Program::uniform(const std::string& name, const Texture2DArray& texture_array)
    {
        use();
        uniform(name, m_texture_unit_counter);
        glActiveTexture(GL_TEXTURE0 + m_texture_unit_counter);
        glBindTexture(GL_TEXTURE_2D_ARRAY, texture_array.m_object.id());
    }

    void Program::draw(const VertexArray& vertex_array, const GLenum mode)
    {
        use();
        glBindVertexArray(vertex_array.m_object.id());

#ifdef USE_SHADER_PRINTF
        GLuint printBuffer = createPrintBuffer();
        bindPrintBuffer(m_object.id(), printBuffer);
#endif
        glDrawArrays(mode, 0, vertex_array.m_num_vertices);
        m_texture_unit_counter = 0;
#ifdef USE_SHADER_PRINTF
        const std::string shader_print_string = getPrintBufferString(printBuffer);
        if (!shader_print_string.empty())
        {
            std::cout << "\nGLSL print:\n" << shader_print_string << std::endl;
        }
        deletePrintBuffer(printBuffer);
#endif
    }

    void Program::use()
    {
        if (m_object.id() != s_active_program_id)
        {
            glUseProgram(m_object.id());
            s_active_program_id = m_object.id();
            m_texture_unit_counter = 0;
        }
    }

    GLint Program::getUniformLocation(const std::string& name)
    {
        auto ret = m_uniform_locations.find(name);
        if (ret == m_uniform_locations.end())
        {
            m_uniform_locations[name] = glGetUniformLocation(m_object.id(), name.c_str());
            if (m_uniform_locations[name] == -1)
            {
                std::string file_names;
                for (const auto& src_params : m_sources)
                {
                    file_names += src_params.second.first.string() + " ";
                }
                std::cout << "Can't find uniform '" + name + "' in shader [ " + file_names + "]" << std::endl;
            }
            ret = m_uniform_locations.find(name);
        }
        assert(ret != m_uniform_locations.end());
        return ret->second;
    }

    void details::checkForErrors(const char* filename, int line)
    {
        std::string error_message;
        GLenum error;
        do
        {
            error = glGetError();
            switch (error)
            {
                case GL_INVALID_ENUM:
                    error_message += "GL_INVALID_ENUM\n";
                    break;
                case GL_INVALID_VALUE:
                    error_message += "GL_INVALID_VALUE\n";
                    break;
                case GL_INVALID_OPERATION:
                    error_message += "GL_INVALID_OPERATION\n";
                    break;
                case GL_INVALID_FRAMEBUFFER_OPERATION:
                    error_message += "GL_INVALID_FRAMEBUFFER_OPERATION\n";
                    break;
                case GL_OUT_OF_MEMORY:
                    error_message += "GL_OUT_OF_MEMORY\n";
                    break;
                case GL_STACK_UNDERFLOW:
                    error_message += "GL_STACK_UNDERFLOW\n";
                    break;
                case GL_STACK_OVERFLOW:
                    error_message += "GL_STACK_OVERFLOW\n";
                    break;
                case GL_NO_ERROR:
                    break;
                default:
                    error_message += "unknown error: " + std::to_string(error) + "\n";
                    break;
            }
        } while (error != GL_NO_ERROR);
        if (!error_message.empty())
        {
            if (filename != nullptr)
            {
                std::cout << "OpenGL error: " << filename << ":" << line << ": ";
            }
            else
            {
                std::cout << "OpenGL error: ";
            }
            std::cout << error_message << std::endl;
        }
    }

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"

    void GLAPIENTRY debugOutput(
        GLenum source, GLenum type, GLuint id,
        GLenum severity, GLsizei length, const GLchar* message, const void* user_param
    )
    {
        // ignore non-significant error/warning codes
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204) return;

        std::cout << "---------------" << std::endl;
        std::cout << "Debug message (" << id << "): " << message << std::endl;

        switch (source)
        {
            case GL_DEBUG_SOURCE_API:
                std::cout << "Source: API";
                break;
            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                std::cout << "Source: Window System";
                break;
            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                std::cout << "Source: Shader Compiler";
                break;
            case GL_DEBUG_SOURCE_THIRD_PARTY:
                std::cout << "Source: Third Party";
                break;
            case GL_DEBUG_SOURCE_APPLICATION:
                std::cout << "Source: Application";
                break;
            case GL_DEBUG_SOURCE_OTHER:
                std::cout << "Source: Other";
                break;
            default:
                std::cout << "Source: Unknown: " << source;
        }
        std::cout << std::endl;

        switch (type)
        {
            case GL_DEBUG_TYPE_ERROR:
                std::cout << "Type: Error";
                break;
            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                std::cout << "Type: Deprecated Behaviour";
                break;
            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                std::cout << "Type: Undefined Behaviour";
                break;
            case GL_DEBUG_TYPE_PORTABILITY:
                std::cout << "Type: Portability";
                break;
            case GL_DEBUG_TYPE_PERFORMANCE:
                std::cout << "Type: Performance";
                break;
            case GL_DEBUG_TYPE_MARKER:
                std::cout << "Type: Marker";
                break;
            case GL_DEBUG_TYPE_PUSH_GROUP:
                std::cout << "Type: Push Group";
                break;
            case GL_DEBUG_TYPE_POP_GROUP:
                std::cout << "Type: Pop Group";
                break;
            case GL_DEBUG_TYPE_OTHER:
                std::cout << "Type: Other";
                break;
            default:
                std::cout << "Type: Unknown: " << type;
        }
        std::cout << std::endl;

        switch (severity)
        {
            case GL_DEBUG_SEVERITY_HIGH:
                std::cout << "Severity: high";
                break;
            case GL_DEBUG_SEVERITY_MEDIUM:
                std::cout << "Severity: medium";
                break;
            case GL_DEBUG_SEVERITY_LOW:
                std::cout << "Severity: low";
                break;
            case GL_DEBUG_SEVERITY_NOTIFICATION:
                std::cout << "Severity: notification";
                break;
            default:
                std::cout << "Severity: Unknown: " << severity;
        }
        std::cout << std::endl;
        std::cout << "---------------" << std::endl;
    }

#pragma GCC diagnostic pop
}
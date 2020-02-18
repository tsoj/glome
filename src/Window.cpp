#include "Window.hpp"

#include <iostream>

Window::Window(int w, int h, const char* title)
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
#ifndef NDEBUG
    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif
    m_glfw_window = glfwCreateWindow(w, h, title, nullptr, nullptr);
    if (!m_glfw_window)
    {
        glfwTerminate();
        throw std::runtime_error("Failed to initialize Window or context.\n");
    }
    glfwMakeContextCurrent(m_glfw_window);

    GLenum err = glewInit();
    if (GLEW_OK != err)
    {
        throw std::runtime_error("Failed to initialize GLEW: " + std::string((char*) glewGetErrorString(err)) + "\n");
    }

    GLint flags;
    glGetIntegerv(GL_CONTEXT_FLAGS, &flags);
    if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
    {
        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(gl::debugOutput, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);

        glDebugMessageControl(GL_DEBUG_SOURCE_API,
                              GL_DEBUG_TYPE_PERFORMANCE,
                              GL_DEBUG_SEVERITY_MEDIUM,
                              0, nullptr, GL_FALSE);
    }
    else
    {
        std::cout << "Not using OpengGL debug output." << std::endl;
    }

    glfwSetInputMode(m_glfw_window, GLFW_STICKY_KEYS, GLFW_TRUE);

    glfwSwapInterval(1);

    int width, height;
    glfwGetFramebufferSize(m_glfw_window, &width, &height);
    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void Window::drawImage(const gl::Texture2D& image)
{
    static gl::Program program = gl::Program({
                                                 {"shader/texture.vert", GL_VERTEX_SHADER},
                                                 {"shader/texture.frag", GL_FRAGMENT_SHADER}
                                             });

    struct MeshData
    {
        gl::VertexArray vao;
        gl::Texture2D image;
    } mesh_data = {gl::VertexArray::rectangleVao(), image};

    gl::renderPass(
        {mesh_data},
        &MeshData::vao,
        nullptr, 0, 0, width(), height(),
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
        GL_TRIANGLES,
        program,
        std::make_tuple(),
        std::make_tuple
            (
                std::tuple("image", &MeshData::image)
            )
    );

    swapBuffers();
}

[[nodiscard]] bool Window::isKeyPressed(int key)
{
    const auto key_state = glfwGetKey(m_glfw_window, key);
    if (key_state == GLFW_PRESS)
    {
        return true;
    }
    else if (key_state == GLFW_RELEASE)
    {
        return false;
    }
    throw std::runtime_error(
        std::string(__FILE__) + ":" + std::to_string(__LINE__) + ": " +
        "glfwGetKey(m_glfw_window, key) returned unknown key state: " + std::to_string(key_state));
    return false;
}
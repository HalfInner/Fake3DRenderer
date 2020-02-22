//#include <glad/glad.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec4.hpp>

//#include <GL/gl.h>
//#include <GL/glu.h>

#include <iostream>
#include "source/Renderable.hh"
#include "source/ShaderManager.hh"
#include "source/Shader.hh"

struct Windowable {
    virtual void initialize() = 0;
    virtual void run() = 0;

    virtual ~Windowable() {};
};


namespace Graphic {

class StaticCamera : public Camera {
  public:
    Result draw(Renderable *renderable) override {
        return Result::Fail;
    }
};

class Triangle : public Renderable {
  public:
    Result draw() override {
        return Result::Fail;
    }
};

} // namespace Graphic


class SimpleWindow : public Windowable {
  public:
    SimpleWindow() :
            shaderManagerPtr_(std::make_shared<ShaderManager>()), vs_(shaderManagerPtr_),
            fs_(shaderManagerPtr_) {
    }

    void initialize() override {
        // glfw: initialize and configure
        // ------------------------------
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // glfw window_ creation
        // --------------------
        initialScreenWidth_ = 800;
        initialScreenHeight_ = 640;
        window_ = glfwCreateWindow(initialScreenWidth_, initialScreenHeight_, "LearnOpenGL", NULL, NULL);
        if (window_ == NULL) {
            constexpr auto errorMsg = "Failed to create GLFW window_";
            std::cerr << errorMsg;
            glfwTerminate();
            throw std::runtime_error(errorMsg);
        }

        glfwMakeContextCurrent(window_);
        glfwSetFramebufferSizeCallback(window_, [](auto win, auto width, auto height) {
            // make sure the viewport matches the new window_ dimensions; note that width and
            // height will be significantly larger than specified on retina displays.
            glViewport(0, 0, width, height);
        });

        auto err = glewInit();
        if (err != GLEW_OK) {
            constexpr auto errorMsg = "Failed to initialize GLEW library";
            std::cerr << errorMsg;
            glfwTerminate();
            throw std::runtime_error(errorMsg);
        }

        if (!GLEW_VERSION_2_1) { // check that the machine supports the 2.1 API.
            // or handle the error in a nicer way
            constexpr auto errorMsg = "GLEW does not support the latest needed features";
            std::cerr << errorMsg;
            glfwTerminate();
            throw std::runtime_error(errorMsg);
        }

        // build and compile our shader program
        // ------------------------------------
        // vertex shader
        int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, vs_.shader(), NULL);
        glCompileShader(vertexShader);
        // check for shader compile errors
        int success;
        char infoLog[512];
        glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
        // fragment shader
        int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, fs_.shader(), NULL);
        glCompileShader(fragmentShader);
        // check for shader compile errors
        glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        }

        // link shaders
        shaderProgram_ = glCreateProgram();
        glAttachShader(shaderProgram_, vertexShader);
        glAttachShader(shaderProgram_, fragmentShader);
        glLinkProgram(shaderProgram_);
        // check for linking errors
        glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shaderProgram_, 512, NULL, infoLog);
            std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        }

        glDeleteShader(vertexShader);
        glDeleteShader(fragmentShader);

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        float vertices[] = {
                0.5f, 0.5f, 0.5f,  // top right
                0.5f, -0.5f, 0.5f,  // bottom right
                -0.5f, -0.5f, 0.5f,  // bottom left
                -0.5f, 0.5f, 0.5f   // top left
        };
        unsigned int indices[] = {  // note that we start from 0!
                0, 1, 3,  // first Triangle
                1, 2, 3   // second Triangle
        };

        glGenVertexArrays(1, &VAO_);
        glGenBuffers(1, &VBO_);
        glGenBuffers(1, &EBO_);
        // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
        glBindVertexArray(VAO_);

        glBindBuffer(GL_ARRAY_BUFFER, VBO_);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
        glEnableVertexAttribArray(0);

        // note that this is allowed, the call to glVertexAttribPointer registered VBO_ as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // remember: do NOT unbind the EBO_ while a VAO_ is active as the bound element buffer object IS stored in the VAO_; keep the EBO_ bound.
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // You can unbind the VAO_ afterwards so other VAO_ calls won't accidentally modify this VAO_, but this rarely happens. Modifying other
        // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
        glBindVertexArray(0);

        // uncomment this call to draw in wireframe polygons.
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    void run() override {

        // render loop
        // -----------
        while (!glfwWindowShouldClose(window_)) {
            // input
            // -----
            if (glfwGetKey(window_, GLFW_KEY_ESCAPE) == GLFW_PRESS)
                glfwSetWindowShouldClose(window_, true);

            // render
            // ------
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            // draw our first triangle
            glUseProgram(shaderProgram_);
            // seeing as we only have a single VAO_ there's no need to bind it every time, but we'll do so to keep things a bit more organized
            glBindVertexArray(VAO_);
            //glDrawArrays(GL_TRIANGLES, 0, 6);
            glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, 0);
            // glBindVertexArray(0); // no need to unbind it every time

            // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
            // -------------------------------------------------------------------------------
            glfwSwapBuffers(window_);
            glfwPollEvents();
        }
    }

    ~SimpleWindow() {
//        runThread.join();
        // optional: de-allocate all resources once they've outlived their purpose:
        // ------------------------------------------------------------------------
        glDeleteVertexArrays(1, &VAO_);
        glDeleteBuffers(1, &VBO_);
        glDeleteBuffers(1, &EBO_);

        // glfw: terminate, clearing all previously allocated GLFW resources.
        // ------------------------------------------------------------------
        glfwTerminate();
    }

  private:
    unsigned int VAO_ = 0;
    unsigned int VBO_ = 0;
    unsigned int EBO_ = 0;
    GLFWwindow *window_ = nullptr;
    GLuint shaderProgram_;

    ShaderManagerPtr shaderManagerPtr_;
    VertexShaderSource vs_;
    FragmentShaderSource fs_;

    int initialScreenWidth_;
    int initialScreenHeight_;
};

int main() {
    SimpleWindow sw;
    sw.initialize();
    sw.run();
}

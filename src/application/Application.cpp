//EXAMPLE GLFW CODE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <cmath>

static void GLClearError() {
    while (glGetError() != GL_NO_ERROR);
}

static void GLCheckError() {

    while (GLenum errorCode = glGetError()) {
        char* errorType;

        switch (errorCode) {
        case GL_NO_ERROR:
            errorType = "GL_NO_ERROR";
            break;
        case GL_INVALID_ENUM:
            errorType = "GL_INVALID_ENUM";
            break;
        case GL_INVALID_VALUE:
            errorType = "GL_INVALID_VALUE";
            break;
        case GL_INVALID_OPERATION:
            errorType = "GL_INVALID_OPERATION";
            break;
        case GL_INVALID_FRAMEBUFFER_OPERATION:
            errorType = "GL_INVALID_FRAMEBUFFER_OPERATION";
            break;
        case GL_OUT_OF_MEMORY:
            errorType = "GL_OUT_OF_MEMORY";
            break;
        case GL_STACK_UNDERFLOW:
            errorType = "GL_STACK_UNDERFLOW";
            break;
        case GL_STACK_OVERFLOW:
            errorType = "GL_STACK_OVERFLOW";
            break;
        default:
            break;
        }
        std::cout << "OPENGL_ERROR: (" << errorType << ")" << std::endl;

        //TODO: 
        //https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/glGetError.xhtml
    } 
}

static std::string ParseShader(const std::string& filepath) {
    std::ifstream file(filepath);
    
    std::string line;
    std::stringstream stream;

    while (getline(file, line)) {
        stream << line << '\n';
    }
    return stream.str();
}


static int CompileShader(const std::string& source, unsigned int type) {
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int status;
    glGetShaderiv(id, GL_COMPILE_STATUS, &status);

    char* shaderType;
    switch (type) {
        case GL_VERTEX_SHADER:
            shaderType = "Vertex";
            break;
        case GL_FRAGMENT_SHADER:
            shaderType = "Fragment";
            break;
        default:
            shaderType = "INVALID";
    }


    if (status != GL_TRUE) {
        std::cout << shaderType << " Shader failed to compile." << std::endl;

        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    } else {
        std::cout << shaderType << " Shader successfully compiled." << std::endl;
    }

    return id;
}

static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {

    unsigned int program = glCreateProgram();

    unsigned int vs = CompileShader(vertexShader, GL_VERTEX_SHADER);
    unsigned int fs = CompileShader(fragmentShader, GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);

    int status;
    //LINK
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    if (status != GL_TRUE) {
        //failed to link successfully
        std::cout << "OpenGL program failed to link." << std::endl;
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*) alloca(length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);

        std::cout << message << std::endl;
    } else {
        std::cout << "Shader successfully linked." << std::endl;
    }

    //VALIDATE
    glValidateProgram(program);
    glGetProgramiv(program, GL_VALIDATE_STATUS, &status);
    if (status != GL_TRUE) {
        //failed to validate successfully
        std::cout << "OpenGL program failed to validate" << std::endl;
        int length;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetProgramInfoLog(program, length, &length, message);
        std::cout << message << std::endl;
    } else {
        std::cout << "Shader successfully validated." << std::endl;
    }

    //delete intermediate data
    glDeleteShader(vs);
    glDeleteShader(fs);


    return program;
}


int main(void)
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "OpenGL", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    // Needs a valid OpenGL context ^
    if (glewInit() != GLEW_OK) {
        std::cout << "Error!" << std::endl;
    }

    std::cout << glGetString(GL_VERSION) << std::endl;

    float positions[] = {
        -0.5f, -0.5f,
         0.5f, -0.5f,
         0.5f,  0.5f,
        -0.5f,  0.5f
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };


    //generate a vertex buffer and store some data
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

    //enable and specify the layout of the buffer (only 1 attribute)
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    //generate an index buffer
    unsigned int indexBuffer;
    glGenBuffers(1, &indexBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

    //load in vertex and fragment shaders
    std::string vertexShader = ParseShader("src/shaders/basic.vert");
    std::string fragmentShader = ParseShader("src/shaders/basic.frag");

    //compile shader code, push to GPU, and tell the context to use
    unsigned int shaderProgram = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    //uniform variable access and value set in the fragment shader
    int location = glGetUniformLocation(shaderProgram, "u_Color");

    //dynamic adjustments
    float r = 0.0f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        r = fmod(r + .01, 360);
        
        glUniform4f(location, sin(r * 3.1415/180), 0.2f, 0.5f, 1.0f);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);



        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glDeleteProgram(shaderProgram);

    glfwTerminate();
    return 0;
}

//GLM EXAMPLE CODE TO CHECK FOR CORRECT PROJECT SETUP

/*
#include <glm/vec3.hpp> // glm::vec3
#include <glm/vec4.hpp> // glm::vec4
#include <glm/mat4x4.hpp> // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective


glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
{
	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.f);
	glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
	View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
	View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
	return Projection * View * Model;
}
*/


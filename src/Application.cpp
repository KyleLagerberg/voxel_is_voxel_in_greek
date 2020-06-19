//EXAMPLE GLFW CODE

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <iostream>


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

    float positions[6] = {
        -0.5f, -0.5f,
         0.0f,  0.5f,
         0.5f, -0.5f
    };


    //generate a vertex buffer and store some data
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    glBindBuffer(GL_ARRAY_BUFFER, buffer);
    glBufferData(GL_ARRAY_BUFFER,  6 * sizeof(float), positions, GL_STATIC_DRAW);

    //enable and specify the layout of the buffer
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    //create vertex and fragment shaders
    std::string vertexShader =
        "#version 330 core\n"
        "\n"
        "layout(location = 0)in vec4 position;\n"
        "void main() {\n"
        "   gl_Position = position;"
        "}\n";

    std::string fragmentShader =
        "#version 330 core"
        "\n"
        "layout(location = 0)out vec4 color;\n"
        "void main() {\n"
        "   color = vec4(1.0, 1.0, 0.0, 1.0);\n"
        "}\n";

    unsigned int shaderProgram = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        glDrawArrays(GL_TRIANGLES, 0, 3);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

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


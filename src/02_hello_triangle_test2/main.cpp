#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

int main()
{
//————————————————————————————————————————————初始化代码————————————————————————————————————————————//
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //创建窗口
    GLFWwindow *window = glfwCreateWindow(800, 600, "Test", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create a window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    //初始化glad：加载所有OpenGL相关的指针
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }
//————————————————————————————————————————————初始化代码————————————————————————————————————————————//

//————————————————————————————————————————————着色器代码————————————————————————————————————————————//

    //顶点着色器
    unsigned int vertexShader;
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    //把这个着色器源码附加到着色器对象上，然后编译它
    //第一个参数为要编译的着色器对象
    //第二参数指定了传递的源码字符串数量
    //第三个参数是顶点着色器真正的源码
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    //检查是否编译成功
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    //片段着色器
    unsigned int fragmentShader;
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    //检查是否编译成功
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    unsigned int shaderProgram;
    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    //检查是否链接成功
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    //激活这个程序对象
    glUseProgram(shaderProgram);
    //删除着色器对象
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

//————————————————————————————————————————————着色器代码————————————————————————————————————————————//

//————————————————————————————————————————————数据配置代码————————————————————————————————————————————//
    //设置顶点数据（和缓冲区）并配置顶点属性
    float firstTriangle[] = {
        -0.9f, -0.5f, 0.0f,  // left 
        -0.0f, -0.5f, 0.0f,  // right
        -0.45f, 0.5f, 0.0f,  // top 
    };
    float secondTriangle[] = {
        0.0f, -0.5f, 0.0f,  // left
        0.9f, -0.5f, 0.0f,  // right
        0.45f, 0.5f, 0.0f   // top 
    };

    //使用glGenBuffers函数和一个缓冲ID生成一个VBO对象
    unsigned int VAO[2], VBO[2];
    glGenVertexArrays(2, VAO);
    glGenBuffers(2, VBO);
    // glGenBuffers(1, &EBO);
    //把新创建的缓冲绑定到GL_ARRAY_BUFFER目标上
    //把之前定义的顶点数据复制到缓冲的内存中
    glBindVertexArray(VAO[0]);

    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(firstTriangle), firstTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glBindVertexArray(VAO[1]);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(secondTriangle), secondTriangle, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
//————————————————————————————————————————————数据配置代码————————————————————————————————————————————//

//——————————————————————————————————————————————渲染代码——————————————————————————————————————————————//
    while (!glfwWindowShouldClose(window)) //检查一次GLFW是否被要求退出
    {
        processInput(window); //交互
        //渲染指令
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //颜色设置函数
        glClear(GL_COLOR_BUFFER_BIT);         //颜色使用or调用函数
        //画三角形
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO[0]);
        glDrawArrays(GL_TRIANGLES,0,3);
        glBindVertexArray(VAO[1]);
        glDrawArrays(GL_TRIANGLES,0,3);
        
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        //渲染指令
        glfwSwapBuffers(window); //交换颜色缓冲
        glfwPollEvents();        //检查有没有触发什么事件
    }
//——————————————————————————————————————————————渲染代码——————————————————————————————————————————————//

//——————————————————————————————————————————————解构代码——————————————————————————————————————————————//
    glDeleteVertexArrays(2, VAO);
    glDeleteBuffers(2, VBO);
    // glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);
    glfwTerminate();
//——————————————————————————————————————————————解构代码——————————————————————————————————————————————//
    return 0;
}

//处理所有输入：查询GLFW是否按下相关按键关闭窗口并做出相应反应
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) //这个函数将会返回这个按键是否正在被按下
        glfwSetWindowShouldClose(window, true);
}

//设置窗口的维度，左下角坐标为（0，0），宽长为800和600
//设置回调函数来调整窗口大小
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
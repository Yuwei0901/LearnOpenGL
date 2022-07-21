#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <tool/shader.h>

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void processInput(GLFWwindow *window);

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;
int main()
{
//————————————————————————————————————————————初始化代码————————————————————————————————————————————//
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    //创建窗口
    GLFWwindow *window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Test", NULL, NULL);
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

    Shader ourShader("C:/Users/Yuwei/Desktop/code/OpenGL/vscode/src/03_shader_class/shader/shader.vs", "C:/Users/Yuwei/Desktop/code/OpenGL/vscode/src/03_shader_class/shader/shader.fs");
    // Shader ourShader("./shader/shader.vs", "./shader/shader.fs");
//————————————————————————————————————————————着色器代码————————————————————————————————————————————//

//————————————————————————————————————————————数据配置代码————————————————————————————————————————————//
    //设置顶点数据（和缓冲区）并配置顶点属性
    float vertices[] = {
        // 位置              // 颜色
        0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // 右下
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // 左下
        0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // 顶部
    };


    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 设置顶点位置属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // 设置顶点颜色属性指针
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 设置线框绘制模式
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
//————————————————————————————————————————————数据配置代码————————————————————————————————————————————//

//——————————————————————————————————————————————渲染代码——————————————————————————————————————————————//
    while (!glfwWindowShouldClose(window)) //检查一次GLFW是否被要求退出
    {
        processInput(window); //交互

        //渲染指令
        glClearColor(0.5f, 0.5f, 0.5f, 1.0f); //颜色设置函数
        glClear(GL_COLOR_BUFFER_BIT);         //颜色使用or调用函数
        //画三角形
        ourShader.use(); 
        float offset = 0.5f;
        ourShader.setFloat("xOffset", offset);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,3);
        // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        //渲染指令

        glfwSwapBuffers(window); //交换颜色缓冲
        glfwPollEvents();        //检查有没有触发什么事件
    }
//——————————————————————————————————————————————渲染代码——————————————————————————————————————————————//

//——————————————————————————————————————————————解构代码——————————————————————————————————————————————//
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
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
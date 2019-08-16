#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include "App.h"
#include "test.h"

static void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
        return;
    }

    App* app = (App*)glfwGetWindowUserPointer(window);
    if (app)
    {
        app->handleKeyInput(key, scancode, action, mods);
    }

}

static void resize_callback(GLFWwindow* window, int width, int height)
{
    App* app = (App*)glfwGetWindowUserPointer(window);
    if (app)
    {
        app->resize( width, height);
    }

}

static void mousebutton_callback(GLFWwindow* window, int button, int action, int mods)
{
    App* app = (App*)glfwGetWindowUserPointer(window);

    if (app)
    {
        app->handleMouseInput(button, action, mods);
    }

}

static void mousemove_callback(GLFWwindow* window, double xpos, double ypos)
{
    App* app = (App*)glfwGetWindowUserPointer(window);

    if (app)
    {
        app->handleMouseMove(xpos, ypos);
    }
}

static void mousescroll_callback(GLFWwindow* window, double x, double y)
{
    App* app = (App*)glfwGetWindowUserPointer(window);

    if (app)
    {
        app->handleMouseScroll(x, y);
    }
}

int main()
{
    runTestMethod();

    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

   
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    glfwSetKeyCallback(window, key_callback);
    glfwSetWindowSizeCallback(window,resize_callback );
    glfwSetMouseButtonCallback(window, mousebutton_callback);
    glfwSetCursorPosCallback(window, mousemove_callback);
    glfwSetScrollCallback(window, mousescroll_callback);

    glfwMakeContextCurrent(window);
    //[in]	interval	The minimum number of screen updates to wait for until the buffers are swapped by glfwSwapBuffers.
    glfwSwapInterval(1);

    App app{ window };
    glfwSetWindowUserPointer(window, (void*)(&app));
    app.init();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        app.update();

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    app.destory();
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
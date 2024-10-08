#include <glad/glad.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "GLFW/glfw3.h"
#include "graphics/shader.h"
#include "graphics/model.h"
#include "graphics/camera.h"
#include "inih/INIReader.h"


void process_keypresses(GLFWwindow *window, float deltaTime);
void mouse_callback(GLFWwindow *window, double x_pos, double y_pos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);

Camera camera;

float last_x, last_y;
bool first_mouse = true;

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main(void)
{
    INIReader config("config.ini");

    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_SAMPLES, 4);
    // We need to explicitly ask for a 4.1, core profile context on OS X
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open fullscreen or windowed context based on config.ini
    GLFWmonitor *monitor;
    int window_width, window_height = 0;
    if (config.GetBoolean("Screen", "Fullscreen", true))
    {
        monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        window_width = mode->width;
        window_height = mode->height;
    }
    else
    {
        monitor = NULL;
        window_width = SCR_WIDTH;
        window_height = SCR_HEIGHT;
    }
    GLFWwindow *window = glfwCreateWindow(window_width, window_height, "OBJ Model", monitor, nullptr);
    camera.setScreenDimensions(window_width, window_height);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    // Set input callbacks
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);
    // Set vsync
    glfwSwapInterval(1);

    glEnable(GL_DEPTH_TEST);   // Depth testing
    glEnable(GL_CULL_FACE);    // Rear face culling
    glEnable(GL_MULTISAMPLE);  // MSAA
    if (config.GetBoolean("Graphics", "Wireframe", false))
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // Set up input sensitivities
    float mouse_sensitivity = config.GetFloat("Input", "MouseSensitivity", 0.3);
    float zoom_sensitivity = config.GetFloat("Input", "ZoomSensitivity", 0.5);
    float fov_sensitivity = config.GetFloat("Input", "FovSensitivity", 0.4);
    camera.setSensitivities(mouse_sensitivity, zoom_sensitivity, fov_sensitivity);

    // Load shader
    Shader shader(
            "shaders/vertex.glsl",
            "shaders/fragment.glsl",
            "shaders/geometry.glsl"
    );
    shader.use(); 
    
    // ---------------------LOAD MODEL---------------------

    
    Model ourModel("assets/cow.obj"); // You can change the model by changing the path here
    
    // ----------------------------------------------------

   
    glm::mat4 model = glm::mat4(1.0f);
    glm::vec3 model_translate_vec = glm::vec3(
            config.GetFloat("Model", "TranslateX", 0.0),
            config.GetFloat("Model", "TranslateY", 0.0),
            config.GetFloat("Model", "TranslateZ", 0.0)
    );
    glm::vec3 model_scale = glm::vec3(
            config.GetFloat("Model", "ScaleXYZ", 1.0)
    );
    float model_rotate_angle = glm::radians(
            config.GetFloat("Model", "RotateAngle", 0.0)
    );
    glm::vec3 model_rotate_axis = glm::vec3(
            config.GetFloat("Model", "RotateAxisX", 0.0),
            config.GetFloat("Model", "RotateAxisY", 0.0),
            config.GetFloat("Model", "RotateAxisZ", 0.0)
    );
    model = glm::translate(model, model_translate_vec);
    model = glm::scale(model, model_scale);
    model = glm::rotate(model, model_rotate_angle, model_rotate_axis);
    // Send model matrix to vertex shader as it remains constant
    shader.setMat4("model", model);

    // Set fragment shader uniforms
    // Material lighting properties
    glm::vec3 material_ambient = glm::vec3(
            config.GetFloat("Material", "AmbientColorR", 1.0),
            config.GetFloat("Material", "AmbientColorG", 1.0),
            config.GetFloat("Material", "AmbientColorB", 1.0)
    );
    shader.setVec3("material.ambient", material_ambient);
    glm::vec3 material_diffuse = glm::vec3(
            config.GetFloat("Material", "DiffuseColorR", 1.0),
            config.GetFloat("Material", "DiffuseColorG", 1.0),
            config.GetFloat("Material", "DiffuseColorB", 1.0)
    );
    shader.setVec3("material.diffuse", material_diffuse);
    glm::vec3 material_specular = glm::vec3(
            config.GetFloat("Material", "SpecularColorR", 1.0),
            config.GetFloat("Material", "SpecularColorG", 1.0),
            config.GetFloat("Material", "SpecularColorB", 1.0)
    );
    shader.setVec3("material.specular", material_specular);
    float material_shininess = config.GetFloat("Material", "Shininess", 32.0);
    shader.setFloat("material.shininess", material_shininess);

    // Light lighting properties
    glm::vec3 light_ambient = glm::vec3(
            config.GetFloat("Light", "AmbientColorR", 1.0),
            config.GetFloat("Light", "AmbientColorG", 1.0),
            config.GetFloat("Light", "AmbientColorB", 1.0));
    shader.setVec3("light.ambient", light_ambient);
    glm::vec3 light_diffuse = glm::vec3(
            config.GetFloat("Light", "DiffuseColorR", 1.0),
            config.GetFloat("Light", "DiffuseColorG", 1.0),
            config.GetFloat("Light", "DiffuseColorB", 1.0));
    shader.setVec3("light.diffuse", light_diffuse);
    glm::vec3 light_specular = glm::vec3(
            config.GetFloat("Light", "SpecularColorR", 1.0),
            config.GetFloat("Light", "SpecularColorG", 1.0),
            config.GetFloat("Light", "SpecularColorB", 1.0));
    shader.setVec3("light.specular", light_specular);

    // Background color
    glm::vec3 background_color = glm::vec3(
            config.GetFloat("Graphics", "BackgroundColorR", 0.0),
            config.GetFloat("Graphics", "BackgroundColorG", 0.0),
            config.GetFloat("Graphics", "BackgroundColorB", 0.0)
    );
    
    float deltaTime,
            currentFrame, lastFrame = 0;
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // render
        // ------
        glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // don't forget to enable shader before setting uniforms
        shader.use();

        shader.setMat4("view", camera.getViewMatrix());
        shader.setMat4("projection", camera.getProjMatrix());
        shader.setVec3("light.position", camera.getPosition());
        shader.setVec3("view_pos", camera.getPosition());

        //render loaded model
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// it's a bit too big for our scene, so scale it down
        shader.setMat4("model", model);
        ourModel.draw(shader);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    

    glDeleteProgram(shader.get_id());
    glfwTerminate();
    return 0;
}

void process_keypresses(GLFWwindow *window, float delta_time)
{
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, true);
    }

    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS)
    {
        camera.narrowFov();
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS)
    {
        camera.widenFov();
    }
}

void mouse_callback(GLFWwindow *window, double x_pos, double y_pos)
{
    if (first_mouse)
    {
        last_x = x_pos;
        last_y = y_pos;
        first_mouse = false;
    }

    float x_offset = x_pos - last_x;
    float y_offset = last_y - y_pos;

    last_x = x_pos;
    last_y = y_pos;

    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        camera.orbit(x_offset, y_offset);
    }
}

void scroll_callback(GLFWwindow *window, double x_offset, double y_offset)
{
    camera.zoom(y_offset);
}
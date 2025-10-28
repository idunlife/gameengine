// ==================================================
// Includes & Globals
// ==================================================
#include <cmath>
#include <iostream>
#include <GLFW/glfw3.h>

// ==================================================
// Engine Config
// ==================================================
const int WINDOW_WIDTH  = 800;
const int WINDOW_HEIGHT = 600;
const char* WINDOW_TITLE = "Monolithic Engine";

// ==================================================
// Global State
// ==================================================
GLFWwindow* window;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

// ==================================================
// Initialization
// ==================================================
bool init() {
    if (!glfwInit()) return false;
    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE, nullptr, nullptr);
    if (!window) return false;
    glfwMakeContextCurrent(window);
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    return true;
}

// ==================================================
// Input Handling
// ==================================================
void processInput() {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// ==================================================
// Physics
// ==================================================
struct Object {
    float y, velocity;
};
Object cube = {0.0f, 0.0f};

void updatePhysics(float dt) {
    cube.velocity -= 9.8f * dt;
    cube.y += cube.velocity * dt;
    if (cube.y < -1.0f) { cube.y = -1.0f; cube.velocity *= -0.5f; }
}

// ==================================================
// Rendering
// ==================================================
void render() {
    glClearColor(0.1f, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    // Render cube (for now, just color change by position)
    float intensity = std::abs(cube.y) / 5.0f;
    glClearColor(0.1f + intensity, 0.2f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

// ==================================================
// Main Loop
// ==================================================
void run() {
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = (float)glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();
        updatePhysics(deltaTime);
        render();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

// ==================================================
// Shutdown
// ==================================================
void shutdown() {
    glfwDestroyWindow(window);
    glfwTerminate();
}

// ==================================================
// Entry Point
// ==================================================
int main() {
    if (!init()) {
        std::cerr << "Failed to initialize engine!\n";
        return -1;
    }
    run();
    shutdown();
    return 0;
}

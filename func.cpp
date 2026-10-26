#include "func.h"
string LoadShader(const char* filename) {
 std::ifstream file(filename);
 if(!file) { file.clear(); file.open(std::string(LAB_SHADER_DIR) + "/" + filename); }
 if(!file) throw std::runtime_error(std::string("Cannot open shader: ") + filename);
 std::stringstream data; data << file.rdbuf(); return data.str();
}
GLFWwindow* InitAll(int w, int h, bool fullscreen) {
 glfwSetErrorCallback([](int code, const char* message) { std::cerr << code << ": " << message << '\n'; });
 if(!glfwInit()) return nullptr;
 glfwWindowHint(GLFW_SAMPLES, 4);
 glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
 glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
 glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
 glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
 GLFWwindow* window = glfwCreateWindow(w, h, "OpenGL Lab 2", fullscreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);
 if(!window) return nullptr;
 glfwMakeContextCurrent(window);
 glewExperimental = GL_TRUE;
 if(glewInit() != GLEW_OK) { glfwDestroyWindow(window); return nullptr; }
 while(glGetError() != GL_NO_ERROR) {}
 glfwSetWindowSizeLimits(window,1000,760,GLFW_DONT_CARE,GLFW_DONT_CARE);
 glfwSwapInterval(1);
 glEnable(GL_MULTISAMPLE);glEnable(GL_PROGRAM_POINT_SIZE);
 glEnable(GL_BLEND);glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
 GLfloat range[2];glGetFloatv(GL_ALIASED_POINT_SIZE_RANGE,range);
 maxPointSize=range[1];
 std::cout << "Point size range (framebuffer pixels): " << range[0] << " .. " << range[1] << '\n';
 std::cout << "Select tasks with the mouse or Space/Backspace. E: point experiment.\n";
 glfwGetFramebufferSize(window, &WinWidth, &WinHeight);
 glfwSetFramebufferSizeCallback(window, window_size_callback);
 return window;
}
void window_size_callback(GLFWwindow*, int w, int h) { WinWidth = w; WinHeight = h; }
void key_callback(GLFWwindow*, int, int, int, int) {}
void EndAll() { glfwTerminate(); }

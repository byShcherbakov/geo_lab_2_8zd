#pragma once
#include "libs.h"
#include <algorithm>

using namespace std;

string LoadShader(const char* filename);
GLFWwindow* InitAll(int w = 640, int h = 480, bool Fullscreen = false);
void window_size_callback(GLFWwindow * window, int width, int height);
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mods);
void EndAll();

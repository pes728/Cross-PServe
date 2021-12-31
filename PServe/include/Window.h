#pragma once

#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <imgui.h>
#include <imgui_impl_opengl2.h>
#include <imgui_impl_glfw.h>
#include "Render.h"

class Window {
public:
	int createWindow(unsigned int width, unsigned int height);
	void begin();

	GLuint texID;
	GLFWwindow* windowPtr;
	void uploadTexture(uint8_t* data, bool flip);
private:
	bool texUploaded;
	unsigned int width, height;
};
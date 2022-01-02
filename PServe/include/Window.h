#pragma once

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <imgui_impl_opengl2.h>
#include <imgui_impl_glfw.h>
#include "Render.h"
#include <RenderSettings.h>

class Window {
public:
	Window(RenderSettings settings, const char* saveFile);
	int createWindow();
	void begin();
	void uploadImage(bool flip);
	
	GLuint texID;
	GLFWwindow* windowPtr;
	RenderSettings settings;
	const char* saveFile;

private:
	uint8_t* image;
	bool texUploaded;
};
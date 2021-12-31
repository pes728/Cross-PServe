#include "Window.h"

int Window::createWindow(unsigned int width, unsigned int height) {
    this->width = width;
    this->height = height;
    
    if (!glfwInit()) return -1;
    windowPtr = glfwCreateWindow(width, height, "Hello World", NULL, NULL);
    if (!windowPtr)
    {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(windowPtr);

    glEnable(GL_TEXTURE_2D);

    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    
    ImGui::StyleColorsDark();

    ImGui_ImplGlfw_InitForOpenGL(windowPtr, true);
    ImGui_ImplOpenGL2_Init();

    return 0;
}

void Window::uploadTexture(uint8_t* data, bool flip) {
    uint8_t* texture = data;
    if (flip) {
        texture = (uint8_t*)malloc(width * height * 3 * sizeof(uint8_t));
        for(int y = 0; y < height; y++){
            for (int x = 0; x < width; x++) {
                int newIndex = 3 * (x + (height - 1 - y) * width);
                texture[newIndex + 0] = data[3 * (x + y * width) + 0];
                texture[newIndex + 1] = data[3 * (x + y * width) + 1];
                texture[newIndex + 2] = data[3 * (x + y * width) + 2];
            }
        }
    }

    texUploaded = true; 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

    if (flip) free(texture);
}



void Window::begin() {
    if (texUploaded) {
        while (!glfwWindowShouldClose(windowPtr))
        {
            glfwPollEvents();
            
            ImGui_ImplOpenGL2_NewFrame();
            ImGui_ImplGlfw_NewFrame();
            ImGui::NewFrame();
            
            
            
            static float f = 0.0f;

            ImGui::Begin("Hello, world!");


            if (ImGui::Button("Render")) 
                render(image_width, image_height, samples, max_depth, cam, bvh, frame);
             
            if (ImGui::Button("Save Image")) {
                imageToColor(image_width, image_height, image, frame->color);
                saveImage("", std::string(FILENAME), image_width, image_height, image);
            }

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();
            ImGui::Render();

            
            glClear(GL_COLOR_BUFFER_BIT);

            glBegin(GL_TRIANGLE_STRIP);

            glTexCoord2f(0.0, 0.0);
            glVertex2f(-1.0, -1.0);

            glTexCoord2f(1.0, 0.0);
            glVertex2f(1.0, -1.0);

            glTexCoord2f(0.0, 1.0);
            glVertex2f(-1.0, 1.0);

            glTexCoord2f(1.0, 1.0);
            glVertex2f(1.0, 1.0);


            glEnd();
           
            ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());

            glfwMakeContextCurrent(windowPtr);
            glfwSwapBuffers(windowPtr);
        }

        ImGui_ImplOpenGL2_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();

        glfwTerminate();
        glDeleteTextures(1, &texID);
    }
}
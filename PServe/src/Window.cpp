#include "Window.h"

Window::Window(RenderSettings settings, const char* saveFile) {
    this->settings = settings;
    this->saveFile = saveFile;
    this->image = (uint8_t*)malloc(settings.width * settings.height * 3 * sizeof(uint8_t));
}

int Window::createWindow() {
    
    if (!glfwInit()) return -1;
    windowPtr = glfwCreateWindow(settings.width, settings.height, "Hello World", NULL, NULL);
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

void Window::uploadImage(bool flip) {
    uint8_t* texture;
    if (flip) {
        texture = (uint8_t*)malloc(settings.width * settings.height * 3 * sizeof(uint8_t));
        for(int y = 0; y < settings.height; y++){
            for (int x = 0; x < settings.width; x++) {
                int newIndex = 3 * (x + (settings.height - 1 - y) * settings.width);
                texture[newIndex + 0] = image[3 * (x + y * settings.width) + 0];
                texture[newIndex + 1] = image[3 * (x + y * settings.width) + 1];
                texture[newIndex + 2] = image[3 * (x + y * settings.width) + 2];
            }
        }
    }

    texUploaded = true; 
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, settings.width, settings.height, 0, GL_RGB, GL_UNSIGNED_BYTE, flip? texture: image);

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


            if (ImGui::Button("Render")) {
                render(settings);
                imageToColor(settings.width, settings.height, image, settings.framebuffer->color);
                uploadImage(true);
            }
             
            if (ImGui::Button("Save Image")) {
                saveImage("", std::string(saveFile), settings.width, settings.height, image);
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
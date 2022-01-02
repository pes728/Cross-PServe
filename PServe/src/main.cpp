//
//  main.cpp
//  PRender
//
//  Created by Peter Sandquist on 9/26/20.
//

#include <string>
#include <iostream>
#include "RenderSettings.h"
#include "Window.h"
//#include <OpenImageDenoise/oidn.hpp>


int main(int argc, char** argv){
    const char* saveFile;
    if (argc < 2) {
        saveFile = "../../../out";
    } else {
        saveFile = argv[1];
    }

    const auto aspect_ratio = 16.0 / 9.0;
    const unsigned int image_width = 2560;
    const unsigned int image_height = static_cast<unsigned int>(image_width / aspect_ratio);
    const unsigned int samples = 1;
    const unsigned int max_depth = 50;
   

    hittable_list w = random_scene();

    bvh_node bvh(w, 0, 1);

    // Camera
    vec3 lookfrom(13,2,3);
    vec3 lookat(0,0,0);
    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0, 1.0);
    
    FrameBuffer* frame = new FrameBuffer(image_width, image_height);


    RenderSettings settings(image_width, image_height, samples, max_depth, cam, &bvh, frame);
    
    //oidn::DeviceRef device = oidn::newDevice();
    //device.commit();
    
    //oidn::FilterRef filter = device.newFilter("RT");


    

    //float* outColor = (float*)malloc(image_width * image_height * 3 * sizeof(float));


    //std::cout << "denoising" << std::endl;

    // Create a filter for denoising a beauty (color) image using prefiltered auxiliary images too
    /*  filter.setImage("color", frame->color, oidn::Format::Float3, image_width, image_height); // beauty
    filter.setImage("albedo", frame->albedo, oidn::Format::Float3, image_width, image_height); // auxiliary
    filter.setImage("normal", frame->normal, oidn::Format::Float3, image_width, image_height); // auxiliary
    filter.setImage("output", outColor, oidn::Format::Float3, image_width, image_height); // denoised beauty
    filter.set("hdr", true); // beauty image is HDR
    filter.set("cleanAux", true); // auxiliary images will be prefiltered
    filter.commit();

    // Create a separate filter for denoising an auxiliary albedo image (in-place)
    oidn::FilterRef albedoFilter = device.newFilter("RT"); // same filter type as for beauty
    albedoFilter.setImage("albedo", frame->albedo, oidn::Format::Float3, image_width, image_height);
    albedoFilter.setImage("output", frame->albedo, oidn::Format::Float3, image_width, image_height);
    albedoFilter.commit();

    // Create a separate filter for denoising an auxiliary normal image (in-place)
    oidn::FilterRef normalFilter = device.newFilter("RT"); // same filter type as for beauty
    normalFilter.setImage("normal", frame->normal, oidn::Format::Float3, image_width, image_height);
    normalFilter.setImage("output", frame->normal, oidn::Format::Float3, image_width, image_height);
    normalFilter.commit();

    // Prefilter the auxiliary images
    albedoFilter.execute();
    normalFilter.execute();

    // Filter the beauty image
    filter.execute();
        
    const char* errorMessage;
    if (device.getError(errorMessage) != oidn::Error::None)
        std::cout << "Error: " << errorMessage << std::endl;


    std::cout << "writing images" << std::endl;

    //albedo
    imageToColor(image_width, image_height, image, frame->albedo);
    saveImage("", std::string(argv[1]) + "-albedo", image_width, image_height, image);

    //normal
    imageToColor(image_width, image_height, image, frame->normal, true);
    saveImage("", std::string(argv[1]) + "-normal", image_width, image_height, image);
*/
    //noisy
    //imageToColor(image_width, image_height, image, frame->color);
    //saveImage("", std::string(FILENAME), image_width, image_height, image);


    //denoised
    //imageToColor(image_width, image_height, image, outColor);
    // saveImage("", std::string(argv[1]), image_width, image_height, image);
    
    //Window
    Window window(settings, saveFile);
    window.createWindow();
    window.begin();
    
    
    //free(outColor);
    delete frame;

    return 0;
}



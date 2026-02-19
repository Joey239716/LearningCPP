#include "rtweekend.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

int main(int argc, char* argv[]) {
    // 1. Scene Setup
    hittable_list world;

    auto material_ground = make_shared<lambertian>(color(0.5, 0.5, 0.5));
    auto material_center = make_shared<lambertian>(color(0.1, 0.2, 0.5));
    auto material_left   = make_shared<dielectric>(1.5);
    auto material_right  = make_shared<metal>(color(0.8, 0.6, 0.2), 0.0);

    world.add(make_shared<sphere>(point3( 0.0, -100.5, -1.0), 100.0, material_ground));
    world.add(make_shared<sphere>(point3( 0.0,    0.0, -1.2),   0.5, material_center));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),   0.5, material_left));
    world.add(make_shared<sphere>(point3(-1.0,    0.0, -1.0),  -0.4, material_left));
    world.add(make_shared<sphere>(point3( 1.0,    0.0, -1.0),   0.5, material_right));

    // 2. Camera Configuration
    camera cam;
    cam.aspect_ratio      = 16.0 / 9.0;
    cam.image_width       = 400;
    cam.samples_per_pixel = 1;  // Start with 1 sample for speed
    cam.max_depth         = 8;
    cam.vfov              = 20;
    cam.lookfrom          = point3(-2, 2, 1);
    cam.lookat            = point3(0, 0, -1);
    cam.vup               = vec3(0, 1, 0);
    cam.defocus_angle     = 0.6;
    cam.focus_dist        = 3.4;

    // 3. SDL2 Initialization
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return 1;
    }

    int render_width = cam.image_width;
    int render_height = int(render_width / cam.aspect_ratio);

    SDL_Window* window = SDL_CreateWindow(
        "CPU Raytracing Engine", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        render_width, render_height, 
        SDL_WINDOW_SHOWN
    );

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture = SDL_CreateTexture(
        renderer, 
        SDL_PIXELFORMAT_ARGB8888, 
        SDL_TEXTUREACCESS_STREAMING, 
        render_width, render_height
    );

    std::vector<uint32_t> pixels(render_width * render_height);
    bool running = true;
    SDL_Event event;

    std::clog << "Engine running. Use WASD to move, ESC to quit.\n";

    // 4. Engine Loop
    while (running) {
        bool moved = false;

        // Handle Events
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
            if (event.type == SDL_KEYDOWN) {
                moved = true; // Any keypress triggers a re-render
                switch (event.key.keysym.sym) {
                    case SDLK_w:      cam.lookfrom += vec3(0, 0, -0.2); break;
                    case SDLK_s:      cam.lookfrom += vec3(0, 0, 0.2);  break;
                    case SDLK_a:      cam.lookfrom += vec3(-0.2, 0, 0); break;
                    case SDLK_d:      cam.lookfrom += vec3(0.2, 0, 0);  break;
                    case SDLK_q:      cam.lookfrom += vec3(0, 0.2, 0);  break;
                    case SDLK_e:      cam.lookfrom += vec3(0, -0.2, 0); break;
                    case SDLK_ESCAPE: running = false; break;
                }
            }
        }

        // 5. Render to Memory Buffer
        cam.render(world, pixels.data());

        // 6. Push Buffer to Screen
        SDL_UpdateTexture(texture, NULL, pixels.data(), render_width * sizeof(uint32_t));
        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        
        // Small delay to prevent CPU from screaming
        SDL_Delay(1); 
    }

    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
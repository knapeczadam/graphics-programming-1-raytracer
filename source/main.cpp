//External includes
#include "vld.h"
#include "SDL.h"
#include "SDL_surface.h"
#undef main

//Standard includes
#include <iostream>

//Project includes
#include "Timer.h"
#include "Renderer.h"
#include "Scene.h"
#include "Vector3.h"
#include "Macros.h"

using namespace dae;

void ShutDown(SDL_Window* pWindow)
{
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
}

void DoVectorTests()
{
    // Dot product
    float dotResult{};
    dotResult = Vector3::Dot(Vector3::UnitX, Vector3::UnitX);
    std::cout << "(1) Same direction: " << dotResult << '\n';
    dotResult = Vector3::Dot(Vector3::UnitX, -Vector3::UnitX);
    std::cout << "(-1) Opposite direction: " << dotResult << '\n';
    dotResult = Vector3::Dot(Vector3::UnitX, Vector3::UnitY);
    std::cout << "(0) Perpendicular: " << dotResult << '\n';

    // Cross product
    Vector3 crossResult{}; // Left-Handed!
    crossResult = Vector3::Cross(Vector3::UnitZ, Vector3::UnitX);
    std::cout << crossResult << '\n';
    crossResult = Vector3::Cross(Vector3::UnitX, Vector3::UnitZ);
    std::cout << crossResult << '\n';
}

int main(int argc, char* args[])
{
    //Unreferenced parameters
    (void)argc;
    (void)args;

    // Test cases
    // DoVectorTests();

    //Create window + surfaces
    SDL_Init(SDL_INIT_VIDEO);

    const uint32_t width = 640;
    const uint32_t height = 480;

    SDL_Window* pWindow = SDL_CreateWindow(
        "RayTracer - **Ádám Knapecz (2DAE09)**",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height, 0);

    if (!pWindow)
        return 1;

    //Initialize "framework"
    const auto pTimer = new Timer();
    const auto pRenderer = new Renderer(pWindow);
#if DYNAMIC_RENDER
#if SCENE_W1
        const auto pScene = new Scene_W1();
#elif SCENE_W2
        const auto pScene = new Scene_W2();
#elif SCENE_W3
        const auto pScene = new Scene_W3();
#elif SCENE_W4
        const auto pScene = new Scene_W4();
#elif SCENE_W5
        const auto pScene = new Scene_W5();
#endif
#else
    const auto pScene = new Scene_W4();
#endif

    pScene->Initialize();

    //Start loop
    pTimer->Start();

    // Start Benchmark
    // pTimer->StartBenchmark();

    float printTimer = 0.f;
    bool isLooping = true;
    bool takeScreenshot = false;
    while (isLooping)
    {
        //--------- Get input events ---------
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            switch (e.type)
            {
            case SDL_QUIT:
                isLooping = false;
                break;
            case SDL_KEYUP:
                if (e.key.keysym.scancode == SDL_SCANCODE_X)
                    takeScreenshot = true;
                if (e.key.keysym.scancode == SDL_SCANCODE_F2)
                    pRenderer->ToggleShadow();
                if (e.key.keysym.scancode == SDL_SCANCODE_F3)
                    pRenderer->SwitchLightingMode();
                if (e.key.keysym.scancode == SDL_SCANCODE_F6)
                    pTimer->StartBenchmark();
                if (e.key.keysym.scancode == SDL_SCANCODE_E)
                    pScene->GetCamera().IncreaseFOV();
                if (e.key.keysym.scancode == SDL_SCANCODE_Q)
                    pScene->GetCamera().DecreaseFOV();
                break;
            case SDL_MOUSEWHEEL:
                pScene->GetCamera().Scroll(e.wheel);
                break;
            }
        }

        //--------- Update ---------
        pScene->Update(pTimer);

        //--------- Render ---------
#if DYNAMIC_RENDER
        pRenderer->DyanmicRender(pScene);
#else
        pRenderer->Render(pScene);
#endif

        //--------- Timer ---------
        pTimer->Update();
        printTimer += pTimer->GetElapsed();
        if (printTimer >= 1.f)
        {
            printTimer = 0.f;
            std::cout << "dFPS: " << pTimer->GetdFPS() << std::endl;
        }

        //Save screenshot after full render
        if (takeScreenshot)
        {
            if (!pRenderer->SaveBufferToImage())
                std::cout << "Screenshot saved!" << std::endl;
            else
                std::cout << "Something went wrong. Screenshot not saved!" << std::endl;
            takeScreenshot = false;
        }
    }
    pTimer->Stop();

    //Shutdown "framework"
    delete pScene;
    delete pRenderer;
    delete pTimer;

    ShutDown(pWindow);
    return 0;
}

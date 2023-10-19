//External includes
#include "SDL.h"
#include "SDL_surface.h"

//Project includes
#include "Renderer.h"
#include "Math.h"
#include "Matrix.h"
#include "Material.h"
#include "Scene.h"
#include "Utils.h"
#include "Macros.h"

#include <execution>
#include <numeric>

namespace dae
{
    Renderer::Renderer(SDL_Window* pWindow) :
        m_pWindow(pWindow),
        m_pBuffer(SDL_GetWindowSurface(pWindow))
    {
        //Initialize
        SDL_GetWindowSize(pWindow, &m_Width, &m_Height);
        m_pBufferPixels = static_cast<uint32_t*>(m_pBuffer->pixels);
        m_HorizontalIter.resize(m_Width);
        m_VerticalIter.resize(m_Height);
        std::iota(m_HorizontalIter.begin(), m_HorizontalIter.end(), 0);
        std::iota(m_VerticalIter.begin(), m_VerticalIter.end(), 0);
    }

    void Renderer::Render(Scene* pScene) const
    {
        if (dynamic_cast<Scene_W1*>(pScene))
        {
            RenderScene_W1(pScene);
        }
        else if (dynamic_cast<Scene_W2*>(pScene))
        {
            RenderScene_W2(pScene);
        }
        else if (dynamic_cast<Scene_W3*>(pScene))
        {
            RenderScene_W3(pScene);
        }
        else if (dynamic_cast<Scene_W4*>(pScene))
        {
            RenderScene_W4(pScene);
        }
    }

    bool Renderer::SaveBufferToImage() const
    {
        return SDL_SaveBMP(m_pBuffer, "RayTracing_Buffer.bmp");
    }

    void Renderer::ToggleShadow()
    {
        m_ShadowsEnabled = not m_ShadowsEnabled;
    }

    void Renderer::SwitchLightingMode()
    {
        m_CurrentLightingMode = static_cast<LightingMode>((static_cast<int>(m_CurrentLightingMode) + 1) % (static_cast<
            int>(LightingMode::Combined) + 1));
        std::cout << "LIGHTING MODE: ";
        switch (m_CurrentLightingMode)
        {
        case LightingMode::ObservedArea:
            std::cout << "OBSERVED_AREA" << std::endl;
            break;
        case LightingMode::Radiance:
            std::cout << "RADIANCE" << std::endl;
            break;
        case LightingMode::BRDF:
            std::cout << "BRDF" << std::endl;
            break;
        case LightingMode::Combined:
            std::cout << "COMBINED" << std::endl;
            break;
        }
    }

    void Renderer::UpdateColor(ColorRGB& finalColor, int px, int py) const
    {
        //Update Color in Buffer
        finalColor.MaxToOne();

        m_pBufferPixels[static_cast<uint32_t>(px) + (static_cast<uint32_t>(py) * m_Width)] = SDL_MapRGB(
            m_pBuffer->format,
            static_cast<uint8_t>(finalColor.r * 255),
            static_cast<uint8_t>(finalColor.g * 255),
            static_cast<uint8_t>(finalColor.b * 255));
    }

#pragma region Week 1
    void Renderer::RenderScene_W1(Scene* pScene) const
    {
        switch (W1_Todo::Todo6)
        {
        case W1_Todo::Todo2:
            RenderScene_W1_Todo2(pScene);
            break;
        case W1_Todo::Todo3:
            RenderScene_W1_Todo3(pScene);
            break;
        case W1_Todo::Todo4:
            RenderScene_W1_Todo4(pScene);
            break;
        case W1_Todo::Todo5:
            RenderScene_W1_Todo5(pScene);
            break;
        case W1_Todo::Todo6:
            RenderScene_W1_Todo6(pScene);
            break;
        case W1_Todo::Todo7:
            RenderScene_W1_Todo7(pScene);
            break;
        case W1_Todo::Todo8:
            RenderScene_W1_Todo8(pScene);
            break;
        }
    }

    void Renderer::RenderScene_W1_Todo2(Scene* pScene) const
    {
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDir{};
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDir.x = rx * aspectRatio;
                rayDir.y = ry;
                rayDir.z = 1.0f;
                rayDir.Normalize();
                ColorRGB finalColor{rayDir.x, rayDir.y, rayDir.z};
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W1_Todo3(Scene* pScene) const
    {
        auto& materials = pScene->GetMaterials();
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDir{};
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDir.x = rx * aspectRatio;
                rayDir.y = ry;
                rayDir.z = 1.0f;
                rayDir.Normalize();
                Ray viewRay{{0.0f, 0.0f, 0.0f}, rayDir};
                ColorRGB finalColor{};
                Sphere testSphere{{0.0f, 0.0f, 100.0f}, 50.0f, 0};
                HitRecord closestHit{};
                GeometryUtils::HitTest_Sphere(testSphere, viewRay, closestHit);
                if (closestHit.didHit)
                {
                    finalColor = materials[closestHit.materialIndex]->Shade();
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W1_Todo4(Scene* pScene) const
    {
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDir{};
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDir.x = rx * aspectRatio;
                rayDir.y = ry;
                rayDir.z = 1.0f;
                rayDir.Normalize();
                Ray viewRay{{0.0f, 0.0f, 0.0f}, rayDir};
                ColorRGB finalColor{};
                Sphere testSphere{{0.0f, 0.0f, 100.0f}, 50.0f, 0};
                HitRecord closestHit{};
                GeometryUtils::HitTest_Sphere(testSphere, viewRay, closestHit);
                if (closestHit.didHit)
                {
                    const float scaled_t{(closestHit.t - 50.0f) / 40.0f};
                    finalColor = {scaled_t, scaled_t, scaled_t};
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W1_Todo5(Scene* pScene) const
    {
        auto& materials = pScene->GetMaterials();
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDir{};
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDir.x = rx * aspectRatio;
                rayDir.y = ry;
                rayDir.z = 1.0f;
                rayDir.Normalize();
                Ray viewRay{{0.0f, 0.0f, 0.0f}, rayDir};
                ColorRGB finalColor{};
                Sphere testSphere{{0.0f, 0.0f, 100.0f}, 50.0f, 0};
                HitRecord closestHit{};
                pScene->GetClosestHitSphere(viewRay, closestHit);
                if (closestHit.didHit)
                {
                    finalColor = materials[closestHit.materialIndex]->Shade();
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W1_Todo6(Scene* pScene) const
    {
        auto& materials = pScene->GetMaterials();
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDir{};
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDir.x = rx * aspectRatio;
                rayDir.y = ry;
                rayDir.z = 1.0f;
                rayDir.Normalize();
                Ray viewRay{{0.0f, 0.0f, 0.0f}, rayDir};
                ColorRGB finalColor{};
                HitRecord closestHit{};
                pScene->GetClosestHit(viewRay, closestHit);
                if (closestHit.didHit)
                {
                    finalColor = materials[closestHit.materialIndex]->Shade();
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W1_Todo7(Scene* pScene) const
    {
        auto& materials = pScene->GetMaterials();
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDir{};
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDir.x = rx * aspectRatio;
                rayDir.y = ry;
                rayDir.z = 1.0f;
                rayDir.Normalize();
                Ray viewRay{{0.0f, 0.0f, 0.0f}, rayDir};
                ColorRGB finalColor{};
                HitRecord closestHit{};
                Plane testPlane{{0.0f, -50.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 0};
                GeometryUtils::HitTest_Plane(testPlane, viewRay, closestHit);
                if (closestHit.didHit)
                {
                    finalColor = materials[closestHit.materialIndex]->Shade();
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W1_Todo8(Scene* pScene) const
    {
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDir{};
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDir.x = rx * aspectRatio;
                rayDir.y = ry;
                rayDir.z = 1.0f;
                rayDir.Normalize();
                Ray viewRay{{0.0f, 0.0f, 0.0f}, rayDir};
                ColorRGB finalColor{};
                HitRecord closestHit{};
                Plane testPlane{{0.0f, -50.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 0};
                GeometryUtils::HitTest_Plane(testPlane, viewRay, closestHit);
                if (closestHit.didHit)
                {
                    const float scaled_t{closestHit.t / 500.0f};
                    finalColor = {scaled_t, scaled_t, scaled_t};
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }
#pragma endregion

#pragma region Week 2
    void Renderer::RenderScene_W2(Scene* pScene) const
    {
        switch (W2_Todo::Todo5)
        {
        case W2_Todo::Todo1_1:
            RenderScene_W2_Todo1_1(pScene);
            break;
        case W2_Todo::Todo1_2:
            RenderScene_W2_Todo1_2(pScene);
            break;
        case W2_Todo::Todo2:
            RenderScene_W2_Todo2(pScene);
            break;
        case W2_Todo::Todo4_1:
            RenderScene_W2_Todo4_1(pScene);
            break;
        case W2_Todo::Todo4_2:
            RenderScene_W2_Todo4_2(pScene);
            break;
        case W2_Todo::Todo5:
            RenderScene_W2_Todo5(pScene);
            break;
        }
    }

    void Renderer::RenderScene_W2_Todo1_1(Scene* pScene) const
    {
        auto& materials = pScene->GetMaterials();
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDirection;
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDirection.x = rx * aspectRatio;
                rayDirection.y = ry;
                rayDirection.z = 1.0f;
                rayDirection.Normalize();

                Ray viewRay{{}, rayDirection};

                HitRecord closestHit{};
                pScene->GetClosestHit(viewRay, closestHit);

                ColorRGB finalColor{};
                if (closestHit.didHit)
                {
                    finalColor = materials[closestHit.materialIndex]->Shade();
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W2_Todo1_2(Scene* pScene) const
    {
        Camera& camera = pScene->GetCamera();
        auto& materials = pScene->GetMaterials();
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDirection;
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDirection.x = rx * aspectRatio;
                rayDirection.y = ry;
                rayDirection.z = 1.0f;
                rayDirection.Normalize();

                Ray viewRay{camera.origin, rayDirection};

                HitRecord closestHit{};
                pScene->GetClosestHit(viewRay, closestHit);

                ColorRGB finalColor{};
                if (closestHit.didHit)
                {
                    finalColor = materials[closestHit.materialIndex]->Shade();
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W2_Todo2(Scene* pScene) const
    {
        Camera& camera = pScene->GetCamera();
        const float FOV{camera.GetFOV()};
        auto& materials = pScene->GetMaterials();
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDirection;
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDirection.x = rx * aspectRatio * FOV;
                rayDirection.y = ry * FOV;
                rayDirection.z = 1.0f;
                rayDirection.Normalize();

                Ray viewRay{camera.origin, rayDirection};

                HitRecord closestHit{};
                pScene->GetClosestHit(viewRay, closestHit);

                ColorRGB finalColor{};
                if (closestHit.didHit)
                {
                    finalColor = materials[closestHit.materialIndex]->Shade();
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W2_Todo4_1(Scene* pScene) const
    {
        Camera& camera = pScene->GetCamera();
        camera.forward = {0.266f, -0.453f, 0.860f};
        const Matrix cameraToWorld{camera.CalculateCameraToWorld()};
        const float FOV{camera.GetFOV()};
        auto& materials = pScene->GetMaterials();
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDirection;
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDirection.x = rx * aspectRatio * FOV;
                rayDirection.y = ry * FOV;
                rayDirection.z = 1.0f;
                rayDirection = cameraToWorld.TransformVector(rayDirection);
                rayDirection.Normalize();

                Ray viewRay{camera.origin, rayDirection};

                HitRecord closestHit{};
                pScene->GetClosestHit(viewRay, closestHit);

                ColorRGB finalColor{};
                if (closestHit.didHit)
                {
                    finalColor = materials[closestHit.materialIndex]->Shade();
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W2_Todo4_2(Scene* pScene) const
    {
        Camera& camera = pScene->GetCamera();
        const Matrix cameraToWorld{camera.CalculateCameraToWorld()};
        const float FOV{camera.GetFOV()};
        auto& materials = pScene->GetMaterials();
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDirection;
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDirection.x = rx * aspectRatio * FOV;
                rayDirection.y = ry * FOV;
                rayDirection.z = 1.0f;
                rayDirection = cameraToWorld.TransformVector(rayDirection);
                rayDirection.Normalize();

                Ray viewRay{camera.origin, rayDirection};

                HitRecord closestHit{};
                pScene->GetClosestHit(viewRay, closestHit);

                ColorRGB finalColor{};
                if (closestHit.didHit)
                {
                    finalColor = materials[closestHit.materialIndex]->Shade();
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W2_Todo5(Scene* pScene) const
    {
        Camera& camera = pScene->GetCamera();
        const Matrix cameraToWorld{camera.CalculateCameraToWorld()};
        const float FOV{camera.GetFOV()};
        auto& materials = pScene->GetMaterials();
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDirection;
        auto& lights = pScene->GetLights();
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDirection.x = rx * aspectRatio * FOV;
                rayDirection.y = ry * FOV;
                rayDirection.z = 1.0f;
                rayDirection = cameraToWorld.TransformVector(rayDirection);
                rayDirection.Normalize();

                Ray viewRay{camera.origin, rayDirection};

                HitRecord closestHit{};
                pScene->GetClosestHit(viewRay, closestHit);

                ColorRGB finalColor{};
                if (closestHit.didHit)
                {
                    finalColor = materials[closestHit.materialIndex]->Shade();
                    for (const auto& light : lights)
                    {
                        closestHit.origin += closestHit.normal * 0.001f;
                        const Vector3 dirToLight{LightUtils::GetDirectionToLight(light, closestHit.origin)};
                        const float lightDistance{dirToLight.Magnitude()};
                        Ray shadowRay{closestHit.origin, dirToLight / lightDistance, 0.0001f, lightDistance};
                        if (pScene->DoesHit(shadowRay))
                        {
                            finalColor *= 0.5f;
                        }
                    }
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }
#pragma endregion

#pragma region Week 3
    void Renderer::RenderScene_W3(Scene* pScene) const
    {
        switch (W3_Todo::Todo6)
        {
        case W3_Todo::Todo1:
            RenderScene_W3_Todo1(pScene);
            break;
        case W3_Todo::Todo3:
            RenderScene_W3_Todo3(pScene);
            break;
        case W3_Todo::Todo4:
            RenderScene_W3_Todo4(pScene);
            break;
        case W3_Todo::Todo6:
            RenderScene_W3_Todo6(pScene);
            break;
        }
    }

    void Renderer::RenderScene_W3_Todo1(Scene* pScene) const
    {
        Camera& camera = pScene->GetCamera();
        const Matrix cameraToWorld{camera.CalculateCameraToWorld()};
        const float FOV{camera.GetFOV()};
        auto& materials = pScene->GetMaterials();
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDirection;
        auto& lights = pScene->GetLights();
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDirection.x = rx * aspectRatio * FOV;
                rayDirection.y = ry * FOV;
                rayDirection.z = 1.0f;
                rayDirection = cameraToWorld.TransformVector(rayDirection);
                rayDirection.Normalize();

                Ray viewRay{camera.origin, rayDirection};

                HitRecord closestHit{};
                pScene->GetClosestHit(viewRay, closestHit);

                ColorRGB finalColor{};
                if (closestHit.didHit)
                {
                    finalColor = materials[closestHit.materialIndex]->Shade();
                    for (const auto& light : lights)
                    {
                        closestHit.origin += closestHit.normal * 0.001f;
                        const Vector3 dirToLight{LightUtils::GetDirectionToLight(light, closestHit.origin)};
                        const float lightDistance{dirToLight.Magnitude()};
                        Ray shadowRay{closestHit.origin, dirToLight / lightDistance, 0.0001f, lightDistance};
                        if (pScene->DoesHit(shadowRay))
                        {
                            finalColor *= 0.5f;
                        }
                    }
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W3_Todo3(Scene* pScene) const
    {
        Camera& camera = pScene->GetCamera();
        const Matrix cameraToWorld{camera.CalculateCameraToWorld()};
        const float FOV{camera.GetFOV()};
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDirection;
        auto& lights = pScene->GetLights();
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDirection.x = rx * aspectRatio * FOV;
                rayDirection.y = ry * FOV;
                rayDirection.z = 1.0f;
                rayDirection = cameraToWorld.TransformVector(rayDirection);
                rayDirection.Normalize();

                Ray viewRay{camera.origin, rayDirection};

                HitRecord closestHit{};
                pScene->GetClosestHit(viewRay, closestHit);

                ColorRGB finalColor{};
                if (closestHit.didHit)
                {
                    for (const auto& light : lights)
                    {
                        closestHit.origin += closestHit.normal * 0.001f;
                        const Vector3 dirToLight{LightUtils::GetDirectionToLight(light, closestHit.origin)};
                        const float lightDistance{dirToLight.Magnitude()};
                        const Vector3 dirToLightNormalized{dirToLight / lightDistance};
                        Ray shadowRay{closestHit.origin, dirToLight / lightDistance, 0.0001f, lightDistance};
                        const float observedArea{Vector3::Dot(dirToLightNormalized, closestHit.normal)};
                        switch (m_CurrentLightingMode)
                        {
                        case LightingMode::ObservedArea:
                            if (observedArea < 0) continue;
                            if (m_ShadowsEnabled and pScene->DoesHit(shadowRay)) continue;
                            finalColor += ColorRGB{observedArea, observedArea, observedArea};
                            break;
                        case LightingMode::Radiance:
                            break;
                        case LightingMode::BRDF:
                            break;
                        case LightingMode::Combined:
                            break;
                        }
                    }
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W3_Todo4(Scene* pScene) const
    {
        Camera& camera = pScene->GetCamera();
        const Matrix cameraToWorld{camera.CalculateCameraToWorld()};
        const float FOV{camera.GetFOV()};
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
        Vector3 rayDirection;
        auto& lights = pScene->GetLights();
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDirection.x = rx * aspectRatio * FOV;
                rayDirection.y = ry * FOV;
                rayDirection.z = 1.0f;
                rayDirection = cameraToWorld.TransformVector(rayDirection);
                rayDirection.Normalize();

                Ray viewRay{camera.origin, rayDirection};

                HitRecord closestHit{};
                pScene->GetClosestHit(viewRay, closestHit);

                ColorRGB finalColor{};
                if (closestHit.didHit)
                {
                    for (const auto& light : lights)
                    {
                        closestHit.origin += closestHit.normal * 0.001f;
                        const Vector3 dirToLight{LightUtils::GetDirectionToLight(light, closestHit.origin)};
                        const float lightDistance{dirToLight.Magnitude()};
                        const Vector3 dirToLightNormalized{dirToLight / lightDistance};
                        Ray shadowRay{closestHit.origin, dirToLight / lightDistance, 0.0001f, lightDistance};
                        const float observedArea{Vector3::Dot(dirToLightNormalized, closestHit.normal)};
                        switch (m_CurrentLightingMode)
                        {
                        case LightingMode::ObservedArea:
                            if (observedArea < 0) continue;
                            if (m_ShadowsEnabled and pScene->DoesHit(shadowRay)) continue;
                            finalColor += ColorRGB{observedArea, observedArea, observedArea};
                            break;
                        case LightingMode::Radiance:
                            if (m_ShadowsEnabled and pScene->DoesHit(shadowRay)) continue;
                            finalColor += LightUtils::GetRadiance(light, closestHit.origin);
                            break;
                        case LightingMode::BRDF:
                            break;
                        case LightingMode::Combined:
                            if (observedArea < 0) continue;
                            if (m_ShadowsEnabled and pScene->DoesHit(shadowRay)) continue;
                            finalColor += LightUtils::GetRadiance(light, closestHit.origin) * observedArea;
                            break;
                        }
                    }
                }
                UpdateColor(finalColor, px, py);
            }
        }
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }

    void Renderer::RenderScene_W3_Todo6(Scene* pScene) const
    {
        Camera& camera = pScene->GetCamera();
        const Matrix cameraToWorld{camera.CalculateCameraToWorld()};
        const float FOV{camera.GetFOV()};
        const auto& lights = pScene->GetLights();
        const auto& materials = pScene->GetMaterials();
        const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
#if MT
        std::for_each(std::execution::par, m_VerticalIter.begin(), m_VerticalIter.end(),
                      [this, FOV, camera, cameraToWorld, pScene, lights, materials, aspectRatio](int py)
                      // [&](int py)
                      {
                          Vector3 rayDirection;
                          for (int px{}; px < m_Width; ++px)
                          {
                              const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                              const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                              rayDirection.x = rx * aspectRatio * FOV;
                              rayDirection.y = ry * FOV;
                              rayDirection.z = 1.0f;
                              rayDirection = cameraToWorld.TransformVector(rayDirection);
                              rayDirection.Normalize();

                              Ray viewRay{camera.origin, rayDirection};

                              HitRecord closestHit{};
                              pScene->GetClosestHit(viewRay, closestHit);

                              ColorRGB finalColor{};
                              if (closestHit.didHit)
                              {
                                  for (const auto& light : lights)
                                  {
                                      const Vector3 dirToLight{LightUtils::GetDirectionToLight(light, closestHit.origin)};
                                      const float lightDistance{dirToLight.Magnitude()};
                                      const Vector3 dirToLightNormalized{dirToLight / lightDistance};
                                      Ray shadowRay{closestHit.origin + closestHit.normal * 0.001f, dirToLightNormalized, 0.0001f, lightDistance};
                                      const float observedArea{Vector3::Dot(dirToLightNormalized, closestHit.normal)};
                                      switch (m_CurrentLightingMode)
                                      {
                                      case LightingMode::ObservedArea:
                                          if (observedArea < 0) continue;
                                          if (m_ShadowsEnabled and pScene->DoesHit(shadowRay)) continue;
                                          finalColor += observedArea;
                                          break;
                                      case LightingMode::Radiance:
                                          if (m_ShadowsEnabled and pScene->DoesHit(shadowRay)) continue;
                                          finalColor += LightUtils::GetRadiance(light, closestHit.origin);
                                          break;
                                      case LightingMode::BRDF:
                                          if (m_ShadowsEnabled and pScene->DoesHit(shadowRay)) continue;
                                          finalColor += materials[closestHit.materialIndex]->Shade(closestHit, dirToLightNormalized, -viewRay.direction);
                                          break;
                                      case LightingMode::Combined:
                                          if (observedArea < 0) continue;
                                          if (m_ShadowsEnabled and pScene->DoesHit(shadowRay)) continue;
                                          finalColor +=
                                              LightUtils::GetRadiance(light, closestHit.origin)
                                              *
                                              materials[closestHit.materialIndex]->Shade(closestHit, dirToLightNormalized, -viewRay.direction)
                                              *
                                              observedArea;
                                          break;
                                      }
                                  }
                              }
                              UpdateColor(finalColor, px, py);
                          }
                      });
#else
        Vector3 rayDirection;
        for (int px{}; px < m_Width; ++px)
        {
            for (int py{}; py < m_Height; ++py)
            {
                const float rx{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
                const float ry{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
                rayDirection.x = rx * aspectRatio * FOV;
                rayDirection.y = ry * FOV;
                rayDirection.z = 1.0f;
                rayDirection = cameraToWorld.TransformVector(rayDirection);
                rayDirection.Normalize();

                Ray viewRay{camera.origin, rayDirection};

                HitRecord closestHit{};
                pScene->GetClosestHit(viewRay, closestHit);

                ColorRGB finalColor{};
                if (closestHit.didHit)
                {
                    for (const auto& light : lights)
                    {
                        const Vector3 dirToLight{LightUtils::GetDirectionToLight(light, closestHit.origin)};
                        const float lightDistance{dirToLight.Magnitude()};
                        const Vector3 dirToLightNormalized{dirToLight / lightDistance};
                        Ray shadowRay{closestHit.origin + closestHit.normal * 0.001f, dirToLightNormalized, 0.0001f, lightDistance};
                        const float observedArea{Vector3::Dot(dirToLightNormalized, closestHit.normal)};
                        switch (m_CurrentLightingMode)
                        {
                        case LightingMode::ObservedArea:
                            if (observedArea < 0) continue;
                            if (m_ShadowsEnabled and pScene->DoesHit(shadowRay)) continue;
                            finalColor += observedArea;
                            break;
                        case LightingMode::Radiance:
                            if (m_ShadowsEnabled and pScene->DoesHit(shadowRay)) continue;
                            finalColor += LightUtils::GetRadiance(light, closestHit.origin);
                            break;
                        case LightingMode::BRDF:
                            if (m_ShadowsEnabled and pScene->DoesHit(shadowRay)) continue;
                            finalColor += materials[closestHit.materialIndex]->Shade(closestHit, dirToLightNormalized, -viewRay.direction);
                            break;
                        case LightingMode::Combined:
                            if (observedArea < 0) continue;
                            if (m_ShadowsEnabled and pScene->DoesHit(shadowRay)) continue;
                            finalColor +=
                                LightUtils::GetRadiance(light, closestHit.origin)
                                *
                                materials[closestHit.materialIndex]->Shade(closestHit, dirToLightNormalized, -viewRay.direction)
                                *
                                observedArea;
                            break;
                        }
                    }
                }
                UpdateColor(finalColor, px, py);
            }
        }
#endif
        //@END
        //Update SDL Surface
        SDL_UpdateWindowSurface(m_pWindow);
    }
#pragma endregion

#pragma region Week 4
    void Renderer::RenderScene_W4(Scene* pScene) const
    {
        RenderScene_W3_Todo6(pScene);
    }
#pragma endregion
}

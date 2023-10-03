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

using namespace dae;

Renderer::Renderer(SDL_Window* pWindow) :
    m_pWindow(pWindow),
    m_pBuffer(SDL_GetWindowSurface(pWindow))
{
    //Initialize
    SDL_GetWindowSize(pWindow, &m_Width, &m_Height);
    m_pBufferPixels = static_cast<uint32_t*>(m_pBuffer->pixels);
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
}

bool Renderer::SaveBufferToImage() const
{
    return SDL_SaveBMP(m_pBuffer, "RayTracing_Buffer.bmp");
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

float Renderer::CalculateFOV(float angle) const
{
    const float halfAlpha{(angle / 2.0f) * TO_RADIANS};
    return std::tanf(halfAlpha);
}

#pragma region Week 1
void Renderer::RenderScene_W1(Scene* pScene) const
{
    const int todo{8};
    switch (todo)
    {
    case 2:
        RenderScene_W1_Todo2(pScene);
        break;
    case 3:
        RenderScene_W1_Todo3(pScene);
        break;
    case 4:
        RenderScene_W1_Todo4(pScene);
        break;
    case 5:
        RenderScene_W1_Todo5(pScene);
        break;
    case 6:
        RenderScene_W1_Todo6(pScene);
        break;
    case 7:
        RenderScene_W1_Todo7(pScene);
        break;
    case 8:
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
    Camera& camera = pScene->GetCamera();
    auto& materials = pScene->GetMaterials();
    auto& lights = pScene->GetLights();
    const Matrix cameraToWorld{camera.CalculateCameraToWorld()};
    //std::cout << cameraToWorld << std::endl;
    const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
    Vector3 rayDirection;
    for (int px{}; px < m_Width; ++px)
    {
        for (int py{}; py < m_Height; ++py)
        {
            /*
            float gradient = px / m_fWidth;
            gradient += py / m_fWidth;
            gradient /= 2.0f;
            */

            // + 0.5f: we need the middle of a pixel
            // rayDirection between almost -1 and 1
            const float rayDirectionX{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
            const float rayDirectionY{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
            const float FOV{CalculateFOV(camera.fovAngle)};
            rayDirection.x = rayDirectionX * aspectRatio * FOV;
            rayDirection.y = rayDirectionY * FOV;
            rayDirection.z = 1.0f;
            rayDirection = cameraToWorld.TransformVector(rayDirection);
            //rayDirection = Matrix::CreateRotation(0.0f,camera.totalPitch * TO_RADIANS,  camera.totalYaw * TO_RADIANS).TransformVector(rayDirection);
            //rayDirection = Matrix::CreateRotationX(camera.totalPitch * TO_RADIANS).TransformVector(rayDirection);
            //rayDirection = Matrix::CreateRotationY(camera.totalYaw * TO_RADIANS).TransformVector(rayDirection);
            rayDirection.Normalize();

            Ray viewRay{camera.origin, rayDirection};
            Sphere testSphere{{0.0f, 0.0f, 100.0f}, 50.0f, 0};
            Plane testPlane{{0.0f, -50.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 0};

            HitRecord closestHit{};

            //GeometryUtils::HitTest_Sphere(testSphere, viewRay, closestHit);
            //GeometryUtils::HitTest_Plane(testPlane, viewRay, closestHit);
            pScene->GetClosestHit(viewRay, closestHit);

            ColorRGB finalColor{0, 0, 0};
            if (closestHit.didHit)
            {
                finalColor = materials[closestHit.materialIndex]->Shade();
                //finalColor = materials[closestHit.materialIndex]->Shade() * closestHit.t;
                //const float scaled_t{ (closestHit.t - 50.0f) / 40.0f };
                //finalColor = { scaled_t, scaled_t, scaled_t };

                //const float scaled_t = closestHit.t / 500.0f;
                //finalColor = { scaled_t, scaled_t, scaled_t };
                if (camera.toggleShadow)
                {
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
            }

            // Test rayDirection's colors
            //ColorRGB finalColor{ rayDirection.x, rayDirection.y , rayDirection.z };

            //Update Color in Buffer
            finalColor.MaxToOne();

            m_pBufferPixels[static_cast<uint32_t>(px) + (static_cast<uint32_t>(py) * m_Width)] = SDL_MapRGB(
                m_pBuffer->format,
                static_cast<uint8_t>(finalColor.r * 255),
                static_cast<uint8_t>(finalColor.g * 255),
                static_cast<uint8_t>(finalColor.b * 255));
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
    Camera& camera = pScene->GetCamera();
    auto& materials = pScene->GetMaterials();
    auto& lights = pScene->GetLights();
    const Matrix cameraToWorld{camera.CalculateCameraToWorld()};
    //std::cout << cameraToWorld << std::endl;
    const float aspectRatio{static_cast<float>(m_Width) / static_cast<float>(m_Height)};
    Vector3 rayDirection;
    for (int px{}; px < m_Width; ++px)
    {
        for (int py{}; py < m_Height; ++py)
        {
            /*
            float gradient = px / m_fWidth;
            gradient += py / m_fWidth;
            gradient /= 2.0f;
            */

            // + 0.5f: we need the middle of a pixel
            // rayDirection between almost -1 and 1
            const float rayDirectionX{(static_cast<float>(px) + 0.5f) / static_cast<float>(m_Width) * 2.0f - 1.0f};
            const float rayDirectionY{1.0f - (static_cast<float>(py) + 0.5f) / static_cast<float>(m_Height) * 2.0f};
            const float FOV{CalculateFOV(camera.fovAngle)};
            rayDirection.x = rayDirectionX * aspectRatio * FOV;
            rayDirection.y = rayDirectionY * FOV;
            rayDirection.z = 1.0f;
            rayDirection = cameraToWorld.TransformVector(rayDirection);
            //rayDirection = Matrix::CreateRotation(0.0f,camera.totalPitch * TO_RADIANS,  camera.totalYaw * TO_RADIANS).TransformVector(rayDirection);
            //rayDirection = Matrix::CreateRotationX(camera.totalPitch * TO_RADIANS).TransformVector(rayDirection);
            //rayDirection = Matrix::CreateRotationY(camera.totalYaw * TO_RADIANS).TransformVector(rayDirection);
            rayDirection.Normalize();

            Ray viewRay{camera.origin, rayDirection};
            Sphere testSphere{{0.0f, 0.0f, 100.0f}, 50.0f, 0};
            Plane testPlane{{0.0f, -50.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 0};

            HitRecord closestHit{};

            //GeometryUtils::HitTest_Sphere(testSphere, viewRay, closestHit);
            //GeometryUtils::HitTest_Plane(testPlane, viewRay, closestHit);
            pScene->GetClosestHit(viewRay, closestHit);

            ColorRGB finalColor{0, 0, 0};
            if (closestHit.didHit)
            {
                finalColor = materials[closestHit.materialIndex]->Shade();
                //finalColor = materials[closestHit.materialIndex]->Shade() * closestHit.t;
                //const float scaled_t{ (closestHit.t - 50.0f) / 40.0f };
                //finalColor = { scaled_t, scaled_t, scaled_t };

                //const float scaled_t = closestHit.t / 500.0f;
                //finalColor = { scaled_t, scaled_t, scaled_t };
                if (camera.toggleShadow)
                {
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
            }

            // Test rayDirection's colors
            //ColorRGB finalColor{ rayDirection.x, rayDirection.y , rayDirection.z };

            //Update Color in Buffer
            finalColor.MaxToOne();

            m_pBufferPixels[static_cast<uint32_t>(px) + (static_cast<uint32_t>(py) * m_Width)] = SDL_MapRGB(
                m_pBuffer->format,
                static_cast<uint8_t>(finalColor.r * 255),
                static_cast<uint8_t>(finalColor.g * 255),
                static_cast<uint8_t>(finalColor.b * 255));
        }
    }

    //@END
    //Update SDL Surface
    SDL_UpdateWindowSurface(m_pWindow);
}
#pragma endregion


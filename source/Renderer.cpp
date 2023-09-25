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

Renderer::Renderer(SDL_Window * pWindow, uint32_t width, uint32_t height) :
	m_pWindow(pWindow),
	m_pBuffer(SDL_GetWindowSurface(pWindow)),
	m_fWidth{ static_cast<float>(width) },
	m_fHeight{static_cast<float>(height)}
{
	//Initialize
	SDL_GetWindowSize(pWindow, &m_Width, &m_Height);
	m_pBufferPixels = static_cast<uint32_t*>(m_pBuffer->pixels);
}

void Renderer::Render(Scene* pScene) const
{
	Camera& camera = pScene->GetCamera();
	auto& materials = pScene->GetMaterials();
	auto& lights = pScene->GetLights();

	const float aspectRatio{ m_fWidth / m_fHeight };
	Vector3 rayDirection;
	for (float px{}; px < m_fWidth; ++px)
	{
		for (float py{}; py < m_fHeight; ++py)
		{
			/*
			float gradient = px / m_fWidth;
			gradient += py / m_fWidth;
			gradient /= 2.0f;
			*/

			// + 0.5f: we need the middle of a pixel
			// rayDirection between almost -1 and 1
			const float rayDirectionX { (px + 0.5f) / m_fWidth * 2.0f - 1.0f };
			const float rayDirectionY{ 1.0f - (py + 0.5f) / m_fHeight * 2.0f };
			rayDirection.x = rayDirectionX * aspectRatio;
			rayDirection.y = rayDirectionY;
			rayDirection.z = 1.0f;
			rayDirection.Normalize();

			Ray viewRay{ {0.0f, 0.0f, 0.0f}, rayDirection };
			Sphere testSphere{ {0.0f, 0.0f, 100.0f}, 50.0f , 0 };
			Plane testPlane{ {0.0f, -50.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, 0 };

			HitRecord closestHit{};

			//GeometryUtils::HitTest_Sphere(testSphere, viewRay, closestHit);
			//GeometryUtils::HitTest_Plane(testPlane, viewRay, closestHit);
			pScene->GetClosestHit(viewRay, closestHit);

            ColorRGB finalColor{ 0, 0, 0 };
			if (closestHit.didHit)
			{
				finalColor = materials[closestHit.materialIndex]->Shade();
				 //finalColor = materials[closestHit.materialIndex]->Shade() * closestHit.t;
				 //const float scaled_t{ (closestHit.t - 50.0f) / 40.0f };
				 //finalColor = { scaled_t, scaled_t, scaled_t };

				//const float scaled_t = closestHit.t / 500.0f;
				//finalColor = { scaled_t, scaled_t, scaled_t };
			}

			// Test rayDirection's colors
			//ColorRGB finalColor{ rayDirection.x, rayDirection.y , rayDirection.z };

			//Update Color in Buffer
			finalColor.MaxToOne();

			m_pBufferPixels[static_cast<uint32_t>(px) + (static_cast<uint32_t>(py) * m_Width)] = SDL_MapRGB(m_pBuffer->format,
				static_cast<uint8_t>(finalColor.r * 255),
				static_cast<uint8_t>(finalColor.g * 255),
				static_cast<uint8_t>(finalColor.b * 255));
		}
	}

	//@END
	//Update SDL Surface
	SDL_UpdateWindowSurface(m_pWindow);
}

bool Renderer::SaveBufferToImage() const
{
	return SDL_SaveBMP(m_pBuffer, "RayTracing_Buffer.bmp");
}

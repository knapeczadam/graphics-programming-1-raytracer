#pragma once
#include <cassert>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include "Math.h"
#include "Timer.h"

namespace dae
{
	struct Camera
	{
		Camera() = default;

		Camera(const Vector3& _origin, float _fovAngle):
			origin{_origin},
			fovAngle{_fovAngle}
		{
		}


		Vector3 origin{};
		float fovAngle{90.f};

		//Vector3 forward{Vector3::UnitZ};
		Vector3 forward{0.266f, -0.453f, 0.860f};
		Vector3 up{Vector3::UnitY};
		Vector3 right{Vector3::UnitX};

		float totalPitch{0.f};
		float totalYaw{0.f};

		Matrix cameraToWorld{};


		Matrix CalculateCameraToWorld()
		{
			Matrix out;
			//forward.Normalize();
			//std::cout << "forward: " << forward.Magnitude() << std::endl;
			right = Vector3{forward.z, 0.f, -forward.x};
			up = Vector3::Cross(forward, right);
			out[0] = Vector4{right, 0.f};
			out[1] = Vector4{up, 0.f};
			out[2] = Vector4{forward, 0.f};
			out[3] = Vector4{origin, 1.f};
			return out;
		}

		void Update(Timer* pTimer)
		{
			const float deltaTime = pTimer->GetElapsed();
			const float speed{10.0f};
			const float rotationSpeed{100.0f};

			//Keyboard Input
			const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
			if (pKeyboardState[SDL_SCANCODE_A])
			{
				origin -= right * deltaTime * speed;
				//forward = Matrix::CreateRotation(totalPitch * TO_RADIANS, totalYaw * TO_RADIANS, 0.0f).TransformVector(Vector3::UnitZ);
				//forward = (Matrix::CreateRotationY(totalYaw * TO_RADIANS) * Matrix::CreateRotationX(totalPitch * TO_RADIANS)).TransformVector(Vector3::UnitZ);
				//forward.Normalize();
			}
			else if (pKeyboardState[SDL_SCANCODE_D])
			{
				origin += right * deltaTime * speed;
				//forward = Matrix::CreateRotation(totalPitch * TO_RADIANS, totalYaw * TO_RADIANS, 0.0f).TransformVector(Vector3::UnitZ);
				//forward = (Matrix::CreateRotationY(totalYaw * TO_RADIANS) * Matrix::CreateRotationX(totalPitch * TO_RADIANS)).TransformVector(Vector3::UnitZ);
				//forward.Normalize();
			}
			if (pKeyboardState[SDL_SCANCODE_W])
			{
				origin += forward * deltaTime * speed;
			}
			else if (pKeyboardState[SDL_SCANCODE_S])
			{
				origin -= forward * deltaTime * speed;
			}

			//Mouse Input
			int mouseX{}, mouseY{};
			const int threshold{1};
			const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);
			mouseX = mouseX > threshold ? 1 : mouseX < -threshold ? -1 : 0; 
			mouseY = mouseY > threshold ? 1 : mouseY < -threshold ? -1 : 0;
			const bool leftMouseButtonDown = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT);
			const bool rightMouseButtonDown = mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT);
			if (leftMouseButtonDown and rightMouseButtonDown)
			{
				origin += up * static_cast<float>(mouseY * -1) * deltaTime * speed;
			}
			else if (leftMouseButtonDown)
			{
				origin += forward * static_cast<float>(mouseY * -1) * deltaTime * speed;
				totalYaw += static_cast<float>(mouseX) * rotationSpeed * deltaTime;
			}
			else if (rightMouseButtonDown)
			{
				totalYaw += static_cast<float>(mouseX) * rotationSpeed* deltaTime;
				totalPitch += static_cast<float>(mouseY) * rotationSpeed* deltaTime;
			}
			if (mouseX or mouseY)
			{
				forward = Matrix::CreateRotation(totalPitch * TO_RADIANS, totalYaw * TO_RADIANS, 0.0f).TransformVector(Vector3::UnitZ);
				//forward = (Matrix::CreateRotationY(totalYaw * TO_RADIANS) * Matrix::CreateRotationX(totalPitch * TO_RADIANS)).TransformVector(Vector3::UnitZ);
				forward.Normalize();
			}
		}
	};
}

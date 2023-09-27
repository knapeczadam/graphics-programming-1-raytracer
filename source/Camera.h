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
			const Vector3 _right{forward.z, 0.f, -forward.x};
			const Vector3 _up = Vector3::Cross(forward, _right);
			out[0] = Vector4{_right.Normalized(), 0.f};
			out[1] = Vector4{_up.Normalized(), 0.f};
			out[2] = Vector4{forward.Normalized(), 0.f};
			out[3] = Vector4{origin, 1.f};
			return out;
		}

		void Update(Timer* pTimer)
		{
			const float deltaTime = pTimer->GetElapsed();
			const float speed{5.0f};

			//Keyboard Input
			const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
			if (pKeyboardState[SDL_SCANCODE_A])
			{
				origin -= right * deltaTime * speed;
			}
			else if (pKeyboardState[SDL_SCANCODE_D])
			{
				origin += right * deltaTime * speed;
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
			const uint32_t mouseState = SDL_GetRelativeMouseState(&mouseX, &mouseY);
			mouseX = mouseX > 0 ? 1 : mouseX < 0 ? -1 : 0;
			mouseY = mouseY > 0 ? 1 : mouseY < 0 ? -1 : 0;
			const bool leftMouseButtonDown = mouseState & SDL_BUTTON(SDL_BUTTON_LEFT);
			const bool rightMouseButtonDown = mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT);
			if (leftMouseButtonDown && rightMouseButtonDown)
			{
				origin += up * static_cast<float>(mouseY * -1) * deltaTime * speed;
			}
			else if (leftMouseButtonDown)
			{
				origin += forward * static_cast<float>(mouseY * -1) * deltaTime * speed;
				totalYaw += static_cast<float>(mouseX) * speed * deltaTime;
				forward = (Matrix::CreateRotationY(totalYaw * TO_RADIANS)).TransformVector(forward);
			}
			else if (rightMouseButtonDown)
			{
				totalYaw += static_cast<float>(mouseX) * speed * deltaTime;
				totalPitch += static_cast<float>(mouseY) * speed * deltaTime;
				forward = (Matrix::CreateRotationY(totalYaw * TO_RADIANS) * Matrix::CreateRotationX(totalPitch * TO_RADIANS)).TransformVector(forward);
			}

			
			
		}
	};
}

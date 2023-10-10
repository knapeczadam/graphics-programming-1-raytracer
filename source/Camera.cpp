#include "Camera.h"

#include <SDL_events.h>

namespace dae
{
    Matrix Camera::CalculateCameraToWorld()
    {
        Matrix out;
        right = Vector3{forward.z, 0.f, -forward.x}.Normalized();
        up = Vector3::Cross(forward, right);
        out[0] = Vector4{right, 0.f};
        out[1] = Vector4{up, 0.f};
        out[2] = Vector4{forward, 0.f};
        out[3] = Vector4{origin, 1.f};
        return out;
    }

    void Camera::Update(Timer* pTimer)
    {
        const float deltaTime = pTimer->GetElapsed();

        //Keyboard Input
        const uint8_t* pKeyboardState = SDL_GetKeyboardState(nullptr);
        MoveCamera(pKeyboardState, deltaTime);
        RotateCamera(deltaTime);
    }

    float Camera::GetFOV() const
    {
        return CalculateFOV(fovAngle);
    }

    void Camera::Scroll(SDL_MouseWheelEvent wheel)
    {
        if (wheel.y > 0) // scroll up
        {
            origin += forward * m_ScrollSpeed;
        }
        else if (wheel.y < 0) // scroll down
        {
            origin -= forward * m_ScrollSpeed;
        }
    }

    void Camera::IncreaseFOV()
    {
        ++fovAngle;
    }

    void Camera::DecreaseFOV()
    {
        --fovAngle;
    }

    float Camera::CalculateFOV(float angle) const
    {
        const float halfAlpha{(angle / 2.0f) * TO_RADIANS};
        return std::tanf(halfAlpha);
    }

    void Camera::MoveCamera(const uint8_t* pKeyboardState, float deltaTime)
    {
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
    }

    void Camera::RotateCamera(float deltaTime)
    {
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
            totalYaw += static_cast<float>(mouseX) * rotationSpeed * deltaTime;
            totalPitch += static_cast<float>(mouseY) * rotationSpeed * deltaTime;
        }
        if (mouseX or mouseY)
        {
            forward = Matrix::CreateRotation(totalPitch * TO_RADIANS, totalYaw * TO_RADIANS, 0.0f).TransformVector(
                Vector3::UnitZ);
            // TODO: verify if this is needed
            //forward.Normalize();
        }
    }
}

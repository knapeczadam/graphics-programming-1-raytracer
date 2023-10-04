#pragma once
#include <cassert>
#include <SDL_keyboard.h>
#include <SDL_mouse.h>

#include "Math.h"
#include "Timer.h"

namespace dae
{
    class Camera
    {
    public:
        Camera() = default;

        Camera(const Vector3& _origin, float _fovAngle):
            origin{_origin},
            fovAngle{_fovAngle}
        {
        }


        Matrix CalculateCameraToWorld();
        void Update(Timer* pTimer);
        float GetFOV() const;

    private:
        float CalculateFOV(float angle) const;
        void MoveCamera(const uint8_t* pKeyboardState, float deltaTime);
        void RotateCamera(float deltaTime);

    public:
        Vector3 origin{};
        float fovAngle{90.f};
        Vector3 forward{Vector3::UnitZ};
        Vector3 up{Vector3::UnitY};
        Vector3 right{Vector3::UnitX};

    private:
        float totalPitch{0.f};
        float totalYaw{0.f};
        Matrix cameraToWorld{};
        float speed{10.f};
        float rotationSpeed{100.f};
    };
}

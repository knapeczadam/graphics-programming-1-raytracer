#pragma once

#include <cstdint>

struct SDL_Window;
struct SDL_Surface;

namespace dae
{
    struct ColorRGB;
    class Scene;

    class Renderer final
    {
    public:
        Renderer(SDL_Window* pWindow);
        ~Renderer() = default;

        Renderer(const Renderer&) = delete;
        Renderer(Renderer&&) noexcept = delete;
        Renderer& operator=(const Renderer&) = delete;
        Renderer& operator=(Renderer&&) noexcept = delete;

        void Render(Scene* pScene) const;
        bool SaveBufferToImage() const;
        void ToggleShadow();
        void SwitchLightingMode();

    private:
        void RenderScene_W1(Scene* pScene) const;
        void RenderScene_W1_Todo2(Scene* pScene) const;
        void RenderScene_W1_Todo3(Scene* pScene) const;
        void RenderScene_W1_Todo4(Scene* pScene) const;
        void RenderScene_W1_Todo5(Scene* pScene) const;
        void RenderScene_W1_Todo6(Scene* pScene) const;
        void RenderScene_W1_Todo7(Scene* pScene) const;
        void RenderScene_W1_Todo8(Scene* pScene) const;

        void RenderScene_W2(Scene* pScene) const;
        void RenderScene_W2_Todo1_1(Scene* pScene) const;
        void RenderScene_W2_Todo1_2(Scene* pScene) const;
        void RenderScene_W2_Todo2(Scene* pScene) const;
        void RenderScene_W2_Todo4_1(Scene* pScene) const;
        void RenderScene_W2_Todo4_2(Scene* pScene) const;
        void RenderScene_W2_Todo5(Scene* pScene) const;

        void RenderScene_W3(Scene* pScene) const;
        void RenderScene_W3_Todo1(Scene* pScene) const;
        void RenderScene_W3_Todo3_1(Scene* pScene) const;
        void RenderScene_W3_Todo3_2(Scene* pScene) const;
        void RenderScene_W3_Todo4(Scene* pScene) const;
        void RenderScene_W3_Todo6(Scene* pScene) const;
        void RenderScene_W3_Todo7(Scene* pScene) const;
        void RenderScene_W3_Todo9(Scene* pScene) const;
        void RenderScene_W3_Todo10(Scene* pScene) const;
        void RenderScene_W3_Todo11(Scene* pScene) const;
        void RenderScene_W3_Todo12(Scene* pScene) const;
        void RenderScene_W3_Todo13(Scene* pScene) const;

        void UpdateColor(ColorRGB& finalColor, int px, int py) const;

    private:
        enum class LightingMode
        {
            ObservedArea, // Lambert Cosine Law
            Radiance, // Incident Radiance
            BRDF, // Scattering of the light
            Combined // Observes Area * Radiance * BRDF
        };

        enum class W1_Todo
        {
            Todo2,
            Todo3,
            Todo4,
            Todo5,
            Todo6,
            Todo7,
            Todo8
        };

        enum class W2_Todo
        {
            Todo1_1,
            Todo1_2,
            Todo2,
            Todo4_1,
            Todo4_2,
            Todo5
        };

        enum class W3_Todo
        {
            Todo1,
            Todo3_1,
            Todo3_2,
            Todo4,
            Todo6,
            Todo7,
            Todo9,
            Todo10,
            Todo11,
            Todo12,
            Todo13
        };

    private:
        SDL_Window* m_pWindow{};

        SDL_Surface* m_pBuffer{};
        uint32_t* m_pBufferPixels{};

        int m_Width{};
        int m_Height{};

        LightingMode m_CurrentLightingMode{LightingMode::Combined};
        bool m_ShadowsEnabled{true};
    };
}

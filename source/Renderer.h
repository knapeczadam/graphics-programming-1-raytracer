#pragma once

#include <vector>

struct SDL_Window;
struct SDL_Surface;

namespace dae
{
    struct ColorRGB;
    struct Matrix;
    struct Vector3;
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
        void DyanmicRender(Scene* pScene) const;
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
        void RenderScene_W3_Todo3(Scene* pScene) const;
        void RenderScene_W3_Todo4(Scene* pScene) const;
        void RenderScene_W3_Todo6(Scene* pScene) const;

        void RenderScene_W4(Scene* pScene) const;

        void RenderScene_W5(Scene* pScene) const;
        void RenderPixel(Scene* pScene, uint32_t pixelIndex, float FOV, float aspectRatio, const Matrix& cameraToWorld, const Vector3& cameraOrigin) const;

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
            Todo3,
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
        SDL_Window*  m_pWindow       {nullptr};
        SDL_Surface* m_pBuffer       {nullptr};
        uint32_t*    m_pBufferPixels {nullptr};

        int m_Width  {0};
        int m_Height {0};

        LightingMode m_CurrentLightingMode {LightingMode::Combined};
        
        bool m_ShadowsEnabled {true};

        std::vector<int>      m_HorizontalIter {};
        std::vector<int>      m_VerticalIter   {};
        std::vector<uint32_t> m_PixelIndices   {};
    };
}

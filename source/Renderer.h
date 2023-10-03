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
        float CalculateFOV(float angle) const;

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
        void RenderScene_W3(Scene* pScene) const;

        void UpdateColor(ColorRGB& finalColor, int px, int py) const;


    private:
        SDL_Window* m_pWindow{};

        SDL_Surface* m_pBuffer{};
        uint32_t* m_pBufferPixels{};

        int m_Width{};
        int m_Height{};
    };
}

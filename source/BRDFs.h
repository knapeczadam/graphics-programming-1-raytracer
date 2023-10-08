#pragma once
#include <algorithm>
#include <cassert>
#include "Math.h"

namespace dae
{
    namespace BRDF
    {
        /**
         * \param kd Diffuse Reflection Coefficient
         * \param cd Diffuse Color
         * \return Lambert Diffuse Color
         */
        static ColorRGB Lambert(float kd, const ColorRGB& cd)
        {
            return cd * kd / PI;
        }

        static ColorRGB Lambert(const ColorRGB& kd, const ColorRGB& cd)
        {
            return kd * cd / PI;
        }

        /**
         * \brief todo
         * \param ks Specular Reflection Coefficient
         * \param exp Phong Exponent
         * \param l Incoming (incident) Light Direction
         * \param v View Direction
         * \param n Normal of the Surface
         * \return Phong Specular Color
         */
        static ColorRGB Phong(float ks, float exp, const Vector3& l, const Vector3& v, const Vector3& n)
        {
            const Vector3 r = Vector3::Reflect(l, n);
            const float cosAlpha = std::max(0.0f, Vector3::Dot(r, -v));
            return colors::White * ks * std::powf(cosAlpha, exp);
        }

        /**
         * \brief BRDF Fresnel Function >> Schlick
         * \param h Normalized Halfvector between View and Light directions
         * \param v Normalized View direction
         * \param f0 Base reflectivity of a surface based on IOR (Indices Of Refrection), this is different for Dielectrics (Non-Metal) and Conductors (Metal)
         * \return
         */
        static ColorRGB FresnelFunction_Schlick(const Vector3& h, const Vector3& v, const ColorRGB& f0)
        {
            return f0 + (1.0f - f0) * std::powf(1.0f - Vector3::Dot(h, v), 5.0f);
        }

        /**
         * \brief BRDF NormalDistribution >> Trowbridge-Reitz GGX (UE4 implemetation - squared(roughness))
         * \param n Surface normal
         * \param h Normalized half vector
         * \param roughness Roughness of the material
         * \return BRDF Normal Distribution Term using Trowbridge-Reitz GGX
         */
        static float NormalDistribution_GGX(const Vector3& n, const Vector3& h, float roughness)
        {
            const float aSquared = roughness * roughness * roughness * roughness;
            return aSquared / (PI * std::powf(std::powf(Vector3::Dot(n, h), 2.0f) * (aSquared - 1.0f) + 1.0f, 2.0f));
        }


        /**
         * \brief BRDF Geometry Function >> Schlick GGX (Direct Lighting + UE4 implementation - squared(roughness))
         * \param n Normal of the surface
         * \param v Normalized view direction
         * \param roughness Roughness of the material
         * \return BRDF Geometry Term using SchlickGGX
         */
        static float GeometryFunction_SchlickGGX(const Vector3& n, const Vector3& v, float roughness)
        {
            const float viewAngle =  std::max(0.0f, Vector3::Dot(n, v));
            return viewAngle / (viewAngle * (1.0f - roughness) + roughness);
        }

        /**
         * \brief BRDF Geometry Function >> Smith (Direct Lighting)
         * \param n Normal of the surface
         * \param v Normalized view direction
         * \param l Normalized light direction
         * \param roughness Roughness of the material
         * \return BRDF Geometry Term using Smith (> SchlickGGX(n,v,roughness) * SchlickGGX(n,l,roughness))
         */
        static float GeometryFunction_Smith(const Vector3& n, const Vector3& v, const Vector3& l, float roughness)
        {
            const float alpha{roughness * roughness};
            const float k{std::powf(alpha + 1.0f, 2.0f) / 8.0f};
            return GeometryFunction_SchlickGGX(n, v, k) * GeometryFunction_SchlickGGX(n, l, k);
        }
    }
}

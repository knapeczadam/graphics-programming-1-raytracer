#pragma once
#include "Math.h"
#include "DataTypes.h"
#include "Macros.h"

#include <fstream>

namespace dae
{
    namespace GeometryUtils
    {
#pragma region Sphere HitTest
        //SPHERE HIT-TESTS

        inline float Remap(float a, float b, float t) { return (t - a) / (b - a); }

        inline bool HitTest_Sphere(const Sphere& sphere, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
        {
            // https://gamedev.stackexchange.com/questions/96459/fast-ray-sphere-collision-code
#if SPHERE_INTERSECTION_ANALYTIC
            const Vector3 L{ray.origin - sphere.origin};
            const float B{Vector3::Dot(ray.direction, L)};
            const float C{Vector3::Dot(L, L) - sphere.radius * sphere.radius};
            if (C > 0.0f and B > 0.0f) return false;
            const float discriminant{B * B - C};
            if (discriminant < 0.0f) return false;
            const float t0{(-B - std::sqrt(discriminant))};
            if (t0 < ray.min or t0 > ray.max) return false;
            if (not ignoreHitRecord)
            {
                hitRecord.didHit = true;
                hitRecord.t = t0;
                hitRecord.origin = ray.origin + ray.direction * hitRecord.t;
                hitRecord.normal = (hitRecord.origin - sphere.origin) / sphere.radius;
                hitRecord.materialIndex = sphere.materialIndex;
            }
            return true;
#else
            const Vector3 L{sphere.origin - ray.origin}; // vector from ray's origin to sphere's center
            const float tca{Vector3::Dot(L, ray.direction)}; // project vector L onto ray's direction vector
            if (tca < 0.0f) return false; // sphere is behind ray's origin (and direction
            const float d2{Vector3::Dot(L, L) - tca * tca}; // distance from sphere center to ray
            const float radius2{sphere.radius * sphere.radius};
            if (d2 > radius2) return false;
            const float thc{std::sqrt(radius2 - d2)}; // distance from ray to sphere surface
            float t0{tca - thc}; // distance from ray's origin to sphere surface
            //float t1{tca + thc}; // distance from ray's origin to sphere surface
            if (t0 < ray.min or t0 > ray.max) return false;
            if (not ignoreHitRecord)
            {
                hitRecord.didHit = true;
                hitRecord.t = t0;
                hitRecord.origin = ray.origin + ray.direction * hitRecord.t;
                hitRecord.normal = (hitRecord.origin - sphere.origin) / sphere.radius;
                hitRecord.materialIndex = sphere.materialIndex;
            }
            return true;
#endif
        }

        inline bool HitTest_Sphere(const Sphere& sphere, const Ray& ray)
        {
            HitRecord temp{};
            return HitTest_Sphere(sphere, ray, temp, true);
        }
#pragma endregion
#pragma region Plane HitTest
        //PLANE HIT-TESTS
        inline bool HitTest_Plane(const Plane& plane, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
        {
            const float denom{Vector3::Dot(plane.normal, ray.direction)};
            if (denom >= 0.0f) return false;
            const float t{Vector3::Dot(plane.origin - ray.origin, plane.normal) / denom};
            if (t < ray.min or t > ray.max) return false;
            if (not ignoreHitRecord)
            {
                hitRecord.didHit = true;
                hitRecord.t = t;
                hitRecord.origin = ray.origin + ray.direction * hitRecord.t;
                hitRecord.normal = plane.normal;
                hitRecord.materialIndex = plane.materialIndex;
            }
            return true;
        }

        inline bool HitTest_Plane(const Plane& plane, const Ray& ray)
        {
            HitRecord temp{};
            return HitTest_Plane(plane, ray, temp, true);
        }
#pragma endregion
#pragma region Triangle HitTest
        //TRIANGLE HIT-TESTS
        inline bool HitTest_Triangle(const Triangle& triangle, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
        {
#if MOLLER_TRUMBORE
            const Vector3 e1{triangle.v1 - triangle.v0};
            const Vector3 e2{triangle.v2 - triangle.v0};

            const Vector3 P{Vector3::Cross(ray.direction, e2)};
            const float det{Vector3::Dot(e1, P)};

            if (triangle.cullMode == TriangleCullMode::BackFaceCulling)
            {
                if (det < 0.0f) return false;
            }
            else if (triangle.cullMode == TriangleCullMode::FrontFaceCulling)
            {
                if (det > 0.0f) return false;
            }

            if (AreEqual(det, 0.0f)) return false;
            const float invDet{1.0f / det};
            const Vector3 T{ray.origin - triangle.v0};

            const float u{Vector3::Dot(T, P) * invDet};
            if (u < 0.0f or u > 1.0f) return false;

            const Vector3 Q{Vector3::Cross(T, e1)};
            const float v{Vector3::Dot(ray.direction, Q) * invDet};

            if (v < 0.0f or u + v > 1.0f) return false;

            const float t{Vector3::Dot(e2, Q) * invDet};

            if (t < ray.min or t > ray.max) return false;

            if (not ignoreHitRecord)
            {
                hitRecord.didHit = true;
                hitRecord.t = t;
                hitRecord.origin = ray.origin + ray.direction * hitRecord.t;
                hitRecord.normal = triangle.normal;
                hitRecord.materialIndex = triangle.materialIndex;
            }
            return true;
#else
            const float denom{Vector3::Dot(triangle.normal, ray.direction)};
            if (AreEqual(denom, 0.0f)) return false;

            if (ignoreHitRecord) // shadow ray
            {
                if (triangle.cullMode == TriangleCullMode::FrontFaceCulling)
                {
                    if (denom > 0.0f) return false;
                }
                else if (triangle.cullMode == TriangleCullMode::BackFaceCulling)
                {
                    if (denom < 0.0f) return false;
                }
            }
            else
            {
                if (triangle.cullMode == TriangleCullMode::FrontFaceCulling)
                {
                    if (denom < 0.0f) return false;
                }
                else if (triangle.cullMode == TriangleCullMode::BackFaceCulling)
                {
                    if (denom > 0.0f) return false;
                }
            }

            const Vector3 L{triangle.v0 - ray.origin};
            const float t{Vector3::Dot(L, triangle.normal) / denom};
            if (t < ray.min or t > ray.max) return false;
            const Vector3 P{ray.origin + ray.direction * t};

            const Vector3 a{triangle.v1 - triangle.v0};
            const Vector3 b{triangle.v2 - triangle.v1};
            const Vector3 c{triangle.v0 - triangle.v2};

            if (Vector3::Dot(Vector3::Cross(a, P - triangle.v0), triangle.normal) < 0.0f) return false;
            if (Vector3::Dot(Vector3::Cross(b, P - triangle.v1), triangle.normal) < 0.0f) return false;
            if (Vector3::Dot(Vector3::Cross(c, P - triangle.v2), triangle.normal) < 0.0f) return false;

            if (not ignoreHitRecord)
            {
                hitRecord.didHit = true;
                hitRecord.t = t;
                hitRecord.origin = ray.origin + ray.direction * hitRecord.t;
                hitRecord.normal = triangle.normal;
                hitRecord.materialIndex = triangle.materialIndex;
            }
            return true;
#endif
        }

        inline bool HitTest_Triangle(const Triangle& triangle, const Ray& ray)
        {
            HitRecord temp{};
            return HitTest_Triangle(triangle, ray, temp, true);
        }
#pragma endregion
#pragma region TriangeMesh HitTest
        inline bool SlabTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray)
        {
            const float tx1{(mesh.transformedMinAABB.x - ray.origin.x) / ray.direction.x};
            const float tx2{(mesh.transformedMaxAABB.x - ray.origin.x) / ray.direction.x};

            float tmin{std::min(tx1, tx2)};
            float tmax{std::max(tx1, tx2)};

            const float ty1{(mesh.transformedMinAABB.y - ray.origin.y) / ray.direction.y};
            const float ty2{(mesh.transformedMaxAABB.y - ray.origin.y) / ray.direction.y};

            tmin = std::max(tmin, std::min(ty1, ty2));
            tmax = std::min(tmax, std::max(ty1, ty2));

            const float tz1{(mesh.transformedMinAABB.z - ray.origin.z) / ray.direction.z};
            const float tz2{(mesh.transformedMaxAABB.z - ray.origin.z) / ray.direction.z};

            tmin = std::max(tmin, std::min(tz1, tz2));
            tmax = std::min(tmax, std::max(tz1, tz2));

            return tmax > 0 and tmax >= tmin;
        }

        inline bool HitTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray, HitRecord& hitRecord, bool ignoreHitRecord = false)
        {
#if SLAB_TEST
            if (not SlabTest_TriangleMesh(mesh, ray)) return false;
#endif
#if TRIANGLE_MESH_WITH_FUNCTION_CALL
            HitRecord hit;
            for (size_t idx{0}, normIdx{0}; idx < mesh.indices.size(); idx += 3, ++normIdx)
            {
                const Vector3& v0{mesh.transformedPositions[mesh.indices[idx]]};
                const Vector3& v1{mesh.transformedPositions[mesh.indices[idx + 1]]};
                const Vector3& v2{mesh.transformedPositions[mesh.indices[idx + 2]]};
                
                Triangle triangle{v0, v1, v2, mesh.transformedNormals[normIdx]};
                triangle.cullMode = mesh.cullMode;
                triangle.materialIndex = mesh.materialIndex;
                
                if (HitTest_Triangle(triangle, ray, hit, ignoreHitRecord))
                {
                    hitRecord.didHit = true;
                    if (hit.t < hitRecord.t)
                    {
                        hitRecord.t = hit.t;
                        hitRecord.origin = hit.origin;
                        hitRecord.normal = hit.normal;
                        hitRecord.materialIndex = triangle.materialIndex;
                    }
                }
            }
            return hitRecord.didHit;
#else
            HitRecord tempHit{};

            for (size_t idx{0}, normIdx{0}; idx < mesh.indices.size(); idx += 3, normIdx++)
            {
                const Vector3& v0 { mesh.transformedPositions[mesh.indices[idx]]};
                const Vector3& v1 { mesh.transformedPositions[mesh.indices[idx + 1]]};
                const Vector3& v2 { mesh.transformedPositions[mesh.indices[idx + 2]]};

                const Vector3 e1{ v1 - v0};
                const Vector3 e2 { v2 - v0};

                const Vector3 P { Vector3::Cross(ray.direction, e2)};
                const float det {Vector3::Dot(e1, P)};

                if (mesh.cullMode == TriangleCullMode::BackFaceCulling)
                {
                    if (det < 0.0f) continue;
                }
                else if (mesh.cullMode == TriangleCullMode::FrontFaceCulling)
                {
                    if (det > 0.0f) continue;
                }

                if (AreEqual(det, 0.0f)) continue;

                const float invDet {1.0f / det};
                const Vector3 T { ray.origin - v0};
                const float u { Vector3::Dot(T, P) * invDet};

                if (u < 0.0f or u > 1.0f) continue;

                const Vector3 Q { Vector3::Cross(T, e1)};
                const float v { invDet * Vector3::Dot(ray.direction, Q)};

                if (v < 0.0f or u + v > 1.0f) continue;

                const float t {Vector3::Dot(e2, Q) * invDet};

                if (t < ray.min or t > ray.max)
                    continue;

                if (t < tempHit.t)
                {
                    tempHit.t = t;
                    hitRecord.didHit = true;

                    if (not ignoreHitRecord)
                    {
                        hitRecord.t = t;
                        hitRecord.origin = ray.origin + ray.direction * t;
                        // hitRecord.normal = Vector3::Cross(e1, e2).Normalized();
                        hitRecord.normal = mesh.transformedNormals[normIdx];
                        hitRecord.materialIndex = mesh.materialIndex;
                    }
                }
            }
            return hitRecord.didHit;
#endif
        }

        inline bool HitTest_TriangleMesh(const TriangleMesh& mesh, const Ray& ray)
        {
            HitRecord temp{};
            return HitTest_TriangleMesh(mesh, ray, temp, true);
        }

#pragma endregion
    }

    namespace LightUtils
    {
        //Direction from target to light
        inline Vector3 GetDirectionToLight(const Light& light, const Vector3 origin)
        {
            return light.origin - origin;
        }

        inline ColorRGB GetRadiance(const Light& light, const Vector3& target)
        {
            ColorRGB radiance;
            switch (light.type)
            {
            case LightType::Point:
                radiance = light.color * (light.intensity / (light.origin - target).SqrMagnitude());
                break;
            case LightType::Directional:
                radiance = light.color * light.intensity;
                break;
            }
            return radiance;
        }
    }

    namespace Utils
    {
        //Just parses vertices and indices
#pragma warning(push)
#pragma warning(disable : 4505) //Warning unreferenced local function
        static bool ParseOBJ(const std::string& filename, std::vector<Vector3>& positions,
                             std::vector<Vector3>& normals, std::vector<int>& indices)
        {
            std::ifstream file(filename);
            if (!file)
                return false;

            std::string sCommand;
            // start a while iteration ending when the end of file is reached (ios::eof)
            while (!file.eof())
            {
                //read the first word of the string, use the >> operator (istream::operator>>) 
                file >> sCommand;
                //use conditional statements to process the different commands	
                if (sCommand == "#")
                {
                    // Ignore Comment
                }
                else if (sCommand == "v")
                {
                    //Vertex
                    float x, y, z;
                    file >> x >> y >> z;
                    positions.push_back({x, y, z});
                }
                else if (sCommand == "f")
                {
                    float i0, i1, i2;
                    file >> i0 >> i1 >> i2;

                    indices.push_back((int)i0 - 1);
                    indices.push_back((int)i1 - 1);
                    indices.push_back((int)i2 - 1);
                }
                //read till end of line and ignore all remaining chars
                file.ignore(1000, '\n');

                if (file.eof())
                    break;
            }

            //Precompute normals
            for (uint64_t index = 0; index < indices.size(); index += 3)
            {
                uint32_t i0 = indices[index];
                uint32_t i1 = indices[index + 1];
                uint32_t i2 = indices[index + 2];

                Vector3 edgeV0V1 = positions[i1] - positions[i0];
                Vector3 edgeV0V2 = positions[i2] - positions[i0];
                Vector3 normal = Vector3::Cross(edgeV0V1, edgeV0V2);

                if (isnan(normal.x))
                {
                    int k = 0;
                }

                normal.Normalize();
                if (isnan(normal.x))
                {
                    int k = 0;
                }

                normals.push_back(normal);
            }

            return true;
        }
#pragma warning(pop)
    }
}

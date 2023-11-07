#pragma once

/**
 * \brief Use Quake's fast inverse square root \n
 * Slower than std::sqrt()?
 */
#define QRSQRT 0

#define MULTITHREADING 1
#define MOLLER_TRUMBORE 1
#define SPHERE_INTERSECTION_ANALYTIC 1
#define TRIANGLE_MESH_WITH_FUNCTION_CALL 0

/**
 * \brief For testing purposes: switch between weeks - can be slower because of dynamic cast \n\n
 * If 0, then REFERENCE scene is applied with 6 spheres and 3 triangles (Week 4)
 */
#define DYNAMIC_RENDER 0

#if DYNAMIC_RENDER
    #define SCENE_W1 0
    #define SCENE_W2 0
    #define SCENE_W3 0
    #define SCENE_W4 0 // REFERENCE: 6 spheres, 3 triangles
    #define SCENE_W5 1 // For testing various .obj files
    #if SCENE_W5 // Low-poly bunny is set by default, for better result SLAB_TEST should be set to 1
        #define SLAB_TEST 0 // AABB
        #define SIMPLE_CUBE 0
        #define SIMPLE_OBJECT 0
        #define SIMPLE_QUAD 0
    #endif
#endif

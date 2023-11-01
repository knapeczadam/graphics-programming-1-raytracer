#include "MathHelpers.h"

/**
 * \brief Fast inverse square root
 * https://www.youtube.com/watch?v=p8u_k2LIZyo
 */
float dae::Q_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *reinterpret_cast<long*>(&y); // evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1);
    y = *reinterpret_cast<float*>(&i);
    y = y * (threehalfs - (x2 * y * y)); // 1st iteration
    y = y * (threehalfs - (x2 * y * y)); // 2nd iteration, this can be removed
    return y;
}

/**
 * \brief Fast reciprocal square root approximation for number > 0.25
 * https://youtu.be/k12BJGSc2Nc?t=2142
 * \param number 
 * \return 
 */
float dae::Q_rsqrtV2(float number)
{
    const int tmp {((0x3f800000 << 1) + 0x3f800000 - *(long*)&number) >> 1 };
    const auto y {*(float*)&tmp};
    return y * (1.47f - 0.47f * number * y * y);
}

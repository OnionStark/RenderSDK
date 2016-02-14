#pragma once

#include "Common/Common.h"
#include <cmath>

class Radian;

static const f32 PI = 3.141592654f;
static const f32 TWO_PI = 6.283185307f;
static const f32 RCP_PI = 0.318309886f;
static const f32 RCP_TWO_PI = 0.159154943f;
static const f32 PI_DIV_TWO = 1.570796327f;
static const f32 PI_DIV_FOUR = 0.785398163f;
static const f32 EPSILON = 1e-8f;

template <typename T>
bool IsInRange(T minValue, T maxValue, T value)
{
	return ((minValue <= value) && (value <= maxValue));
}

template <typename T>
T Max(T left, T right)
{
    return (left > right) ? left : right;
}

template <typename T>
T Min(T left, T right)
{
    return (left < right) ? left : right;
}

template <typename T>
T Sqrt(T value)
{
    return std::sqrt(value);
}

template <typename T>
T Sqr(T value)
{
    return value * value;
}

template <typename T>
T Abs(T value)
{
    return std::abs(value);
}

template <typename T>
T Clamp(T lowerBound, T upperBound, T value)
{
    return Max(Min(upperBound, value), lowerBound);
}

template <typename T>
bool IsEqual(T left, T right, T epsilon)
{
    return (Abs(left - right) < epsilon);
}

template <typename T>
T Rcp(T value)
{
    return T(1) / value;
}

f32 RadiansToDegrees(f32 radians);
f32 DegreesToRadians(f32 degrees);

f32 Sin(const Radian& angle);

f32 Cos(const Radian& angle);
const Radian ACos(f32 cosAngle);

void SinCos(f32& sinAngle, f32& cosAngle, const Radian& angle);
f32 Tan(const Radian& angle);

f32 Ceil(f32 value);
f64 Ceil(f64 value);

f32 Floor(f32 value);
f64 Floor(f64 value);

f32 Pow(f32 base, f32 exponent);
f64 Pow(f64 base, f64 exponent);
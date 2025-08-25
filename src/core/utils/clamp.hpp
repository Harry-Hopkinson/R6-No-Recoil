#pragma once

template<typename T>
constexpr const T& clamp(const T& value, const T& low, const T& high)
{
    if (value < low)
        return low;
    if (value > high)
        return high;
    return value;
}

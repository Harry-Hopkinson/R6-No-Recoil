#pragma once

template<typename T>
constexpr const T& maximum(const T& a, const T& b)
{
    return a > b ? a : b;
}

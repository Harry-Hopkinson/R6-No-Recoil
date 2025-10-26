#pragma once

namespace MathUtils
{

    /**
     * @brief Returns the maximum of two values
     * @param a First value
     * @param b Second value
     * @return The larger of the two values
     */
    template<typename T>
    constexpr T Max(T a, T b)
    {
        return (a > b) ? a : b;
    }

    /**
     * @brief Returns the minimum of two values
     * @param a First value
     * @param b Second value
     * @return The smaller of the two values
     */
    template<typename T>
    constexpr T Min(T a, T b)
    {
        return (a < b) ? a : b;
    }

    /**
     * @brief Clamps a value between min and max
     * @param value The value to clamp
     * @param min Minimum value
     * @param max Maximum value
     * @return The clamped value
     */
    template<typename T>
    constexpr T Clamp(T value, T min, T max)
    {
        return (value < min) ? min : (value > max) ? max : value;
    }

} // namespace MathUtils

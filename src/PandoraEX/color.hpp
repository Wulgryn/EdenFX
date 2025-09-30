#ifndef PANDORAEX_COLOR_HPP
#define PANDORAEX_COLOR_HPP

#include "object.hpp"
#include <cstddef>

namespace PandoraEX
{
    /// @brief Represents a color with red, green, blue, and alpha components.
    /// @details Each component is a floating-point value in the range of 0.0 to 1.0.
    /// @note The color components can also be set using byte values (0 to 255), which will be converted to the range of 0.0 to 1.0.
    Struct(Color)
    {
        /// @brief Red component of the color (0.0 to 1.0).
        /// @note This is a floating-point value, not a byte.
        double r;
        /// @brief Green component of the color (0.0 to 1.0).
        /// @note This is a floating-point value, not a byte.
        double g;
        /// @brief Blue component of the color (0.0 to 1.0).
        /// @note This is a floating-point value, not a byte.
        double b;
        /// @brief Alpha component of the color (0.0 to 1.0).
        /// @note This is a floating-point value, not a byte.
        double a;

        /// @brief Constructs a Color object.
        /// @param r Red component (0.0 to 1.0).
        /// @param g Green component (0.0 to 1.0).
        /// @param b Blue component (0.0 to 1.0).
        /// @param a Alpha component (0.0 to 1.0). Defaults to 1.0 (opaque).
        /// @throws Throws an exception if any component is out of range (0.0 to 1.0) or (0 to 255).
        /// @note If a component is greater than 1.0, it will be divided by 255.0.
        Color(double r, double g, double b, double a = 1.0f);

        /// @brief Gets the red component of the color.
        /// @return The red component (0 to 255).
        std::byte getR() const { return (std::byte)(r * 255); }
        /// @brief Gets the green component of the color.
        /// @return The green component (0 to 255).
        std::byte getG() const { return (std::byte)(g * 255); }
        /// @brief Gets the blue component of the color.
        /// @return The blue component (0 to 255).
        std::byte getB() const { return (std::byte)(b * 255); }
        /// @brief Gets the alpha component of the color.
        /// @return The alpha component (0 to 255).
        std::byte getA() const { return (std::byte)(a * 255); }

        /// @brief Sets the red component of the color.
        /// @param r The red component (0 to 255).
        void setR(std::byte r) { this->r = (int)r / 255.0f; }
        /// @brief Sets the green component of the color.
        /// @param g The green component (0 to 255).
        void setG(std::byte g) { this->g = (int)g / 255.0f; }
        /// @brief Sets the blue component of the color.
        /// @param b The blue component (0 to 255).
        void setB(std::byte b) { this->b = (int)b / 255.0f; }
        /// @brief Sets the alpha component of the color.
        /// @param a The alpha component (0 to 255).
        void setA(std::byte a) { this->a = (int)a / 255.0f; }
    };
}

#endif // PANDORAEX_COLOR_HPP
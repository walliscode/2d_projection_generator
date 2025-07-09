/////////////////////////////////////////////////
/// @file
/// @brief Implementation of the Vertex3 class.
/////////////////////////////////////////////////

#include "Vertex3.h"
#include "glm/ext/vector_float3.hpp"

namespace projection_generator {

/////////////////////////////////////////////////
Vertex3::Vertex3(glm::vec3 position, sf::Color color)
    : m_position(position), m_color(color) {}

} // namespace projection_generator

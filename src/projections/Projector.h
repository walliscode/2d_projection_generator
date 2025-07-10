/////////////////////////////////////////////////
/// @file
/// @brief Declaration of the Projector class.
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/// Preprocessor Directives
/////////////////////////////////////////////////
#pragma once

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include "Fragment3D.h"
#include "glm/ext/matrix_float4x4.hpp"
#include <SFML/Graphics/VertexArray.hpp>
#include <glm/mat4x4.hpp>
#include <vector>
namespace projection_generator {

class Projector {

private:
  std::vector<sf::VertexArray> m_projected_shapes;

  void RotateAndSnapshotFragment(const Fragment3D &fragment,
                                 const float tilt_angle,
                                 const glm::vec3 tilt_axis,
                                 const size_t rotation_intervals,
                                 const glm::vec3 rotation_axis);

  sf::VertexArray ProjectToVertexArray(const Fragment3D &fragment,
                                       const glm::mat4 &model_matrix);

public:
  Projector() = default;

  const std::vector<sf::VertexArray> &GetProjectedShapes() const;

  void RotateFragmentAboutY(const Fragment3D &fragment,
                            const size_t rotation_intervals);
};
} // namespace projection_generator

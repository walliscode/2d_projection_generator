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

  void ProjectShape(Fragment3D &fragment);

  Fragment3D RotateFragmentAboutY(const Fragment3D &, glm::mat4 &model_matrix);

  void RotateAndSnapshotFragment(const Fragment3D &fragment,
                                 const float tilt_angle,
                                 const glm::vec3 tilt_axis,
                                 const size_t rotation_intervals,
                                 const glm::vec3 rotation_axis);

public:
  Projector(Fragment3D &original_fragment);

  const std::vector<sf::VertexArray> &GetProjectedShapes() const;
};
} // namespace projection_generator

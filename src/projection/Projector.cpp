/////////////////////////////////////////////////
/// @file
/// @brief Implementation of the Projector class.
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include "Projector.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
namespace projection_generator {

/////////////////////////////////////////////////
void Projector::ProjectShape(Fragment3D &fragment) {}

/////////////////////////////////////////////////
Fragment3D Projector::RotateFragmentAboutY(const Fragment3D &fragment,
                                           glm::mat4 &model_matrix) {

  // Define rotation axis
  glm::vec3 rotation_axis(0.0f, 1.0f, 0.0f);
}

/////////////////////////////////////////////////

void Projector::RotateAndSnapshotFragment(const Fragment3D &fragment,
                                          const float tilt_angle,
                                          const glm::vec3 tilt_axis,
                                          const size_t rotation_intervals,
                                          const glm::vec3 rotation_axis) {
  // Compute center of fragment
  glm::vec3 centre(0.0f);
  for (const auto &vertex : fragment.GetVertices()) {
    centre += vertex.m_position;
  }
  centre /= static_cast<float>(fragment.GetVertices().size());

  glm::mat4 translate_to_origin = glm::translate(glm::mat4(1.0f), -centre);
  glm::mat4 tilt =
      glm::rotate(glm::mat4(1.0f), glm::radians(tilt_angle), tilt_axis);

  // Rotate around the object's center at various angles
  for (size_t i = 0; i < rotation_intervals; ++i) {
    float angle = static_cast<float>(i) *
                  (360.0f / static_cast<float>(rotation_intervals));
    glm::mat4 rotation =
        glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotation_axis);

    glm::mat4 model_matrix = rotation * tilt * translate_to_origin;

    // TODO: apply model_matrix to each vertex and project to 2D
    // store result in sf::VertexArray or similar
  }
}

} // namespace projection_generator

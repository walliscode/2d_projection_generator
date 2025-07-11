
/// @brief Implementation of the Projector class.
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include "Projector.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/vector_float3.hpp"
#include <SFML/Graphics/PrimitiveType.hpp>
#include <SFML/Graphics/Vertex.hpp>
namespace projection_generator {

/////////////////////////////////////////////////
void Projector::RotateFragmentAboutY(const Fragment3D &fragment,
                                     const size_t rotation_intervals) {

  // set variables specific to this function
  glm::vec3 rotation_axis(0.0f, 1.0f, 0.0f); // Y-axis
  glm::vec3 tilt_axis(1.0f, 0.0f, 0.0f);     // X-axis
  float tilt_angle = -30.0f;                 // Tilt angle in degrees

  // Rotate and snapshot the fragment
  RotateAndSnapshotFragment(fragment, tilt_angle, tilt_axis, rotation_intervals,
                            rotation_axis);
}
/////////////////////////////////////////////////
sf::VertexArray Projector::ProjectToVertexArray(const Fragment3D &fragment,
                                                const glm::mat4 &model_matrix) {

  std::vector<glm::vec3> transformed;

  size_t num_culled_triangles = 0;
  // Step 1: Transform all vertex positions
  const auto &vertices = fragment.GetVertices();
  for (const auto &v : vertices) {
    glm::vec4 world = model_matrix * glm::vec4(v.m_position, 1.0f);
    transformed.push_back(glm::vec3(world)); // No projection or normalization
  }

  sf::VertexArray result(sf::PrimitiveType::Triangles);

  // Step 2: For each triangle
  for (const auto &tri : fragment.GetTriangles()) {
    const glm::vec2 p0 = glm::vec2(transformed[tri[0]]);
    const glm::vec2 p1 = glm::vec2(transformed[tri[1]]);
    const glm::vec2 p2 = glm::vec2(transformed[tri[2]]);

    // Step 3: Backface culling (screen-space)
    glm::vec2 v0 = p1 - p0;
    glm::vec2 v1 = p2 - p0;
    float cross_z = v0.x * v1.y - v0.y * v1.x;
    if (cross_z <= 0.0f) {
      num_culled_triangles++;
      continue; // Skip this triangle if it is back-facing
    }

    // Step 4: Output raw float 2D triangles with color

    result.append(
        sf::Vertex(sf::Vector2f(p0.x, p0.y), vertices[tri[0]].m_color));
    result.append(
        sf::Vertex(sf::Vector2f(p1.x, p1.y), vertices[tri[1]].m_color));
    result.append(
        sf::Vertex(sf::Vector2f(p2.x, p2.y), vertices[tri[2]].m_color));
  }
  std::cout << "[DEBUG] Projector::ProjectToVertexArray: "
            << "Culled " << num_culled_triangles << " triangles out of "
            << fragment.GetTriangles().size() << " total triangles."
            << std::endl;
  return result;
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

  // add a translation to move it back to the window center
  glm::mat4 translate_to_window_center =
      glm::translate(glm::mat4(1.0f), glm::vec3(400.0f, 300.0f, 0.0f));
  // Rotate around the object's center at various angles
  for (size_t i = 0; i < rotation_intervals; ++i) {
    float angle = static_cast<float>(i) *
                  (360.0f / static_cast<float>(rotation_intervals));
    glm::mat4 rotation =
        glm::rotate(glm::mat4(1.0f), glm::radians(angle), rotation_axis);

    glm::mat4 model_matrix =
        translate_to_window_center * rotation * tilt * translate_to_origin;

    sf::VertexArray projected_shape =
        ProjectToVertexArray(fragment, model_matrix);
    m_projected_shapes.push_back(projected_shape);
  }
}

/////////////////////////////////////////////////
const std::vector<sf::VertexArray> &Projector::GetProjectedShapes() const {
  return m_projected_shapes;
}

} // namespace projection_generator

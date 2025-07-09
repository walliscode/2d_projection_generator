/////////////////////////////////////////////////
/// @file
/// @brief Implementation of the Fragment3D class methods.
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include "Fragment3D.h"
#include "glm/ext/vector_float3.hpp"
#include "happly.h"
#include <SFML/System/Vector3.hpp>
#include <array>
#include <cwchar>
#include <vector>

namespace projection_generator {

/////////////////////////////////////////////////
void Fragment3D::ConfigureFromPlyFile(happly::PLYData &data) {

  // get number of vertices and resize m_vertices
  size_t numVertices = data.getElement("vertex").count;

  std::vector<std::array<double, 3>> vertex_positions =
      data.getVertexPositions();
  std::vector<std::array<unsigned char, 3>> vertex_colors =
      data.getVertexColors();

  // for each vertex, extract the position and color
  for (size_t i = 0; i < numVertices; ++i) {

    glm::vec3 position(vertex_positions[i][0], vertex_positions[i][1],
                       vertex_positions[i][2]);
    sf::Color color(vertex_colors[i][0], vertex_colors[i][1],
                    vertex_colors[i][2]);

    m_vertices[i] = Vertex3(position, color);
  }

  // get number of faces and resize m_faces
  size_t numFaces = data.getElement("face").count;

  std::vector<std::vector<size_t>> face_indices = data.getFaceIndices();

  for (size_t i = 0; i < numFaces; ++i) {
    // Ensure the face has exactly 4 vertices
    if (face_indices[i].size() == 4) {
      m_faces.push_back({face_indices[i][0], face_indices[i][1],
                         face_indices[i][2], face_indices[i][3]});
    } else {
      // Handle cases where the face does not have exactly 4 vertices
      // This could be an error or a warning depending on your needs
      throw std::runtime_error("Face does not have exactly 4 vertices.");
    }
  }
  // Generate triangles from faces
  for (const auto &face : m_faces) {
    m_triangles.push_back({face[0], face[1], face[2]});
    m_triangles.push_back({face[0], face[2], face[3]});
  }
}

/////////////////////////////////////////////////
const std::vector<Vertex3> &Fragment3D::GetVertices() const {
  return m_vertices;
}
} // namespace projection_generator

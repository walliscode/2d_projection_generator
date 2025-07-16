/////////////////////////////////////////////////
/// @file
/// @brief Implementation of the Fragment3D class methods with extensive debug
/// messages.
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include "Fragment3D.h"
#include "glm/ext/vector_float3.hpp"
#include "happly.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/System/Vector3.hpp>
#include <array>
#include <cwchar>
#include <iostream> // For debug messages
#include <vector>

namespace projection_generator {

Fragment3D::Fragment3D(happly::PLYData &data) {
  std::cout << "[DEBUG] Fragment3D constructor called." << std::endl;
  // Configure the fragment from the PLY data
  ConfigureFromPlyFile(data);
  std::cout << "[DEBUG] Fragment3D constructor finished." << std::endl;
}

Fragment3D::Fragment3D(VoxModel &vox_data) {
  std::cout << "[DEBUG] Fragment3D constructor called with VoxModel."
            << std::endl;
  // Configure the fragment from the VoxModel data
  ConfigureFromVoxData(vox_data);
  std::cout << "[DEBUG] Fragment3D constructor finished." << std::endl;
}
/////////////////////////////////////////////////
void Fragment3D::ConfigureFromPlyFile(happly::PLYData &data) {

  std::cout << "[DEBUG] Configuring Fragment3D from PLY file..." << std::endl;

  // get number of vertices and resize m_vertices
  size_t numVertices = data.getElement("vertex").count;
  std::cout << "[DEBUG] Number of vertices in PLY: " << numVertices
            << std::endl;

  std::vector<std::array<double, 3>> vertex_positions =
      data.getVertexPositions();
  std::vector<std::array<unsigned char, 3>> vertex_colors =
      data.getVertexColors();

  if (vertex_positions.size() != numVertices) {
    std::cerr << "[ERROR] vertex_positions size does not match numVertices!"
              << std::endl;
  }
  if (vertex_colors.size() != numVertices) {
    std::cerr << "[ERROR] vertex_colors size does not match numVertices!"
              << std::endl;
  }

  m_vertices.clear();
  m_vertices.reserve(numVertices);

  // for each vertex, extract the position and color
  for (size_t i = 0; i < numVertices; ++i) {
    if (i >= vertex_positions.size() || i >= vertex_colors.size()) {
      std::cerr << "[ERROR] Index " << i << " out of bounds for vertex data!"
                << std::endl;
      break;
    }

    glm::vec3 position(vertex_positions[i][0], vertex_positions[i][1],
                       vertex_positions[i][2]);
    sf::Color color(vertex_colors[i][0], vertex_colors[i][1],
                    vertex_colors[i][2]);

    m_vertices.emplace_back(position, color);
  }

  // get number of faces and resize m_faces
  size_t numFaces = data.getElement("face").count;
  std::cout << "[DEBUG] Number of faces in PLY: " << numFaces << std::endl;

  std::vector<std::vector<size_t>> face_indices = data.getFaceIndices();
  if (face_indices.size() != numFaces) {
    std::cerr << "[ERROR] face_indices size does not match numFaces!"
              << std::endl;
  }

  m_faces.clear();
  m_faces.reserve(numFaces);

  for (size_t i = 0; i < numFaces; ++i) {
    // Ensure the face has exactly 4 vertices
    if (face_indices[i].size() == 4) {
      m_faces.push_back({face_indices[i][0], face_indices[i][1],
                         face_indices[i][2], face_indices[i][3]});
    } else {
      // Handle cases where the face does not have exactly 4 vertices
      std::cerr << "[ERROR] Face " << i
                << " does not have exactly 4 vertices (has "
                << face_indices[i].size() << ")" << std::endl;
      throw std::runtime_error("Face does not have exactly 4 vertices.");
    }
  }

  // Generate triangles from faces
  m_triangles.clear();
  m_triangles.reserve(m_faces.size() * 2);

  for (size_t i = 0; i < m_faces.size(); ++i) {
    const auto &face = m_faces[i];
    // Double-check size for safety
    if (face.size() != 4) {
      std::cerr << "[ERROR] Stored face " << i << " does not have 4 vertices!"
                << std::endl;
      continue;
    }
    m_triangles.push_back({face[0], face[1], face[2]});
    m_triangles.push_back({face[0], face[2], face[3]});
  }
  std::cout << "[DEBUG] Finished configuring Fragment3D from PLY file."
            << std::endl;
}

/////////////////////////////////////////////////
void Fragment3D::ConfigureFromVoxData(VoxModel &vox_data) {
  std::cout << "[DEBUG] Configuring Fragment3D from VoxModel..." << std::endl;
  // Clear existing vertices and triangles
  m_vertices.clear();
  m_triangles.clear();
  // Iterate through voxels and create vertices
  for (const auto &voxel : vox_data.voxels) {
    glm::vec3 position(voxel.x, voxel.y, voxel.z);

    m_vertices.emplace_back(position, voxel.color);
  }
  // Generate triangles from voxels
  size_t numVoxels = vox_data.voxels.size();
  for (size_t i = 0; i < numVoxels; ++i) {
    if (i + 1 < numVoxels && i + 2 < numVoxels) {
      m_triangles.push_back({i, i + 1, i + 2});
    }
    if (i + 1 < numVoxels && i + 2 < numVoxels) {
      m_triangles.push_back({i, i + 2, i + 1});
    }
  }
  std::cout << "[DEBUG] Finished configuring Fragment3D from VoxModel."
            << std::endl;
}
/////////////////////////////////////////////////
const std::vector<Vertex3> &Fragment3D::GetVertices() const {
  return m_vertices;
}

/////////////////////////////////////////////////
const std::vector<std::array<size_t, 3>> &Fragment3D::GetTriangles() const {
  return m_triangles;
}

} // namespace projection_generator

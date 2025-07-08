
#include <SFML/Graphics.hpp>
#include <cmath>
#include <tuple>
#include <vector>

// 3D vertex struct
struct Vertex3D {
  float x, y, z;
};

// Triangle face by indices into vertices
using Triangle = std::tuple<int, int, int>;

// Rotate a 3D point around X axis
Vertex3D rotateX(const Vertex3D &v, float angle) {
  float rad = angle * M_PI / 180.f;
  float cosa = cos(rad), sina = sin(rad);
  return {v.x, v.y * cosa - v.z * sina, v.y * sina + v.z * cosa};
}

// Rotate a 3D point around Y axis
Vertex3D rotateY(const Vertex3D &v, float angle) {
  float rad = angle * M_PI / 180.f;
  float cosa = cos(rad), sina = sin(rad);
  return {v.x * cosa + v.z * sina, v.y, -v.x * sina + v.z * cosa};
}

// Rotate a 3D point around Z axis
Vertex3D rotateZ(const Vertex3D &v, float angle) {
  float rad = angle * M_PI / 180.f;
  float cosa = cos(rad), sina = sin(rad);
  return {v.x * cosa - v.y * sina, v.x * sina + v.y * cosa, v.z};
}

// Cross product of two vectors (3D)
Vertex3D cross(const Vertex3D &a, const Vertex3D &b) {
  return {a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x};
}

// Subtract vectors a - b
Vertex3D subtract(const Vertex3D &a, const Vertex3D &b) {
  return {a.x - b.x, a.y - b.y, a.z - b.z};
}

// Dot product of two vectors
float dot(const Vertex3D &a, const Vertex3D &b) {
  return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Orthographic projection: drop Z, scale and translate to window
sf::Vector2f projectOrthographic(const Vertex3D &v, float scale, float cx,
                                 float cy) {
  return {cx + v.x * scale, cy - v.y * scale}; // flip Y to screen coords
}

// Compute face normal given triangle vertices
Vertex3D computeNormal(const Vertex3D &v0, const Vertex3D &v1,
                       const Vertex3D &v2) {
  Vertex3D e1 = subtract(v1, v0);
  Vertex3D e2 = subtract(v2, v0);
  return cross(e1, e2);
}

int main() {
  // Window setup
  const int width = 800;
  const int height = 600;
  sf::RenderWindow window(sf::VideoMode({width, height}),
                          "Cube Orthographic Projection");
  window.setFramerateLimit(60);

  // Cube vertices (centered at origin)
  std::vector<Vertex3D> cubeVertices = {{-1, -1, -1}, {1, -1, -1}, {1, 1, -1},
                                        {-1, 1, -1},  {-1, -1, 1}, {1, -1, 1},
                                        {1, 1, 1},    {-1, 1, 1}};

  // Cube faces as triangles
  std::vector<Triangle> cubeTriangles = {
      {0, 1, 2}, {0, 2, 3}, {4, 6, 5}, {4, 7, 6}, {0, 3, 7}, {0, 7, 4},
      {1, 5, 6}, {1, 6, 2}, {3, 2, 6}, {3, 6, 7}, {0, 4, 5}, {0, 5, 1}};

  // Rotation intervals in degrees
  float xStep = 0.f;
  float yStep = 30.f;
  float zStep = 0.f;

  // Fixed tilt (degrees)
  float fixedTiltX = -30.f; // forward tilt
  float fixedTiltY = 0.f;
  float fixedTiltZ = 0.f;
  std::vector<float> xAngles, yAngles, zAngles;

  if (xStep > 0.f) {
    for (float x = 0; x < 360; x += xStep)
      xAngles.push_back(x);
  } else {
    xAngles.push_back(0.f);
  }

  if (yStep > 0.f) {
    for (float y = 0; y < 360; y += yStep)
      yAngles.push_back(y);
  } else {
    yAngles.push_back(0.f);
  }

  if (zStep > 0.f) {
    for (float z = 0; z < 360; z += zStep)
      zAngles.push_back(z);
  } else {
    zAngles.push_back(0.f);
  }

  // Store all projections (each projection stores projected 2D points and
  // visible triangles)
  struct Projection {
    std::vector<sf::Vector2f> projectedVertices;
    std::vector<Triangle> visibleTriangles;
  };

  std::vector<Projection> projections;

  // Camera/view direction (orthographic looking down -Z)
  const Vertex3D viewDir = {0, 0, -1};

  // Generate all projections
  for (float xAngle : xAngles) {
    for (float yAngle : yAngles) {
      for (float zAngle : zAngles) {
        // Rotate vertices
        std::vector<Vertex3D> rotated;
        rotated.reserve(cubeVertices.size());
        for (const auto &v : cubeVertices) {
          Vertex3D r = rotateX(v, xAngle);
          r = rotateY(r, yAngle);
          r = rotateZ(r, zAngle);
          rotated.push_back(r);
        }

        // Backface culling & project visible triangles
        std::vector<Triangle> visible;
        for (const auto &tri : cubeTriangles) {
          int i0, i1, i2;
          std::tie(i0, i1, i2) = tri;
          Vertex3D n = computeNormal(rotated[i0], rotated[i1], rotated[i2]);
          if (dot(n, viewDir) < 0) {
            visible.push_back(tri);
          }
        }

        // Project all vertices (even if triangle not visible, for points or
        // edges)
        std::vector<sf::Vector2f> projected;
        float scale = 100.f;
        float cx = width / 2.f;
        float cy = height / 2.f;
        for (const auto &v : rotated) {
          projected.push_back(projectOrthographic(v, scale, cx, cy));
        }

        projections.push_back({projected, visible});
      }
    }
  }

  size_t currentProjection = 0;
  const int ticksPerProjection = 20; // frames per projection
  int tickCounter = 0;

  // Main loop
  while (window.isOpen()) {
    while (const std::optional<sf::Event> event = window.pollEvent()) {

      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    window.clear(sf::Color::Black);

    // Get current projection
    const auto &proj = projections[currentProjection];

    // Draw triangles
    sf::VertexArray triangleArray(sf::PrimitiveType::Triangles);
    for (const auto &tri : proj.visibleTriangles) {
      int i0, i1, i2;
      std::tie(i0, i1, i2) = tri;
      triangleArray.append(
          sf::Vertex(proj.projectedVertices[i0], sf::Color::Green));
      triangleArray.append(
          sf::Vertex(proj.projectedVertices[i1], sf::Color::Green));
      triangleArray.append(
          sf::Vertex(proj.projectedVertices[i2], sf::Color::Green));
    }
    window.draw(triangleArray);

    // Optional: draw points on vertices
    sf::VertexArray pointsArray(sf::PrimitiveType::Points);
    for (const auto &pt : proj.projectedVertices) {
      pointsArray.append(sf::Vertex(pt, sf::Color::White));
    }
    window.draw(pointsArray);

    window.display();

    // Update tick and cycle projection
    tickCounter++;
    if (tickCounter >= ticksPerProjection) {
      tickCounter = 0;
      currentProjection = (currentProjection + 1) % projections.size();
    }
  }

  return 0;
}

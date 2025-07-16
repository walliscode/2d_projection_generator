/////////////////////////////////////////////////
/// Preprocessor Directives
/////////////////////////////////////////////////
#pragma once

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include <SFML/Graphics/Color.hpp>
#include <cstdint>
#include <string>
#include <vector>

// A single voxel (position and color index)
struct Voxel {
  uint8_t x, y, z;    // Grid position
  uint8_t colorIndex; // 1-based, into palette[0..255]
  sf::Color color;
};

// A MagicaVoxel model (single chunk)
struct VoxModel {
  int sizeX = 0, sizeY = 0, sizeZ = 0;
  std::vector<Voxel> voxels; // Voxel grid
  uint32_t palette[256];     // RGBA colors
};

// Loads a MagicaVoxel .vox file into the model. Returns true on success.
bool loadVox(const std::string &filename, VoxModel &model);

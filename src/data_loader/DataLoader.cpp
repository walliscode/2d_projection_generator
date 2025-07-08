/////////////////////////////////////////////////
/// @file
/// @brief Implementation of the DataLoader class
/////////////////////////////////////////////////

/////////////////////////////////////////////////
/// Headers
/////////////////////////////////////////////////
#include "DataLoader.h"
#include "happly.h"

namespace projection_generator {

/////////////////////////////////////////////////
happly::PLYData DataLoader::LoadDataFromPlyFile(const std::string &file_name) {
  // construct file path
  std::string ply_file = file_name + ".ply";

  happly::PLYData data(ply_file);

  return data;
}
} // namespace projection_generator


#include "DataLoader.h"
#include "Fragment3D.h"
#include "Projector.h"
#include "directory_paths.h"
#include "happly.h"
#include <filesystem>
int main() {

  // initiate the DataLoader to read in the .ply data
  projection_generator::DataLoader data_loader;

  // construct the path to the .ply file
  std::filesystem::path ply_file = getDataFolder() / "metal_nut.ply";

  happly::PLYData ply_data = data_loader.LoadDataFromPlyFile(ply_file.string());
  // create a Fragment3D object from the loaded data
  projection_generator::Fragment3D fragment{ply_data};

  projection_generator::Projector projector;

  // Rotate the fragment about the Y-axis with 12
  projector.RotateFragmentAboutY(fragment, 12);
  return 0;
}

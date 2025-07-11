
#include "DataLoader.h"
#include "Fragment3D.h"
#include "Projector.h"
#include "directory_paths.h"
#include "happly.h"
#include <filesystem>
#include <iostream>
int main() {

  // initiate the DataLoader to read in the .ply data
  projection_generator::DataLoader data_loader;
  std::cout << "DataLoader initiated." << std::endl;

  // construct the path to the .ply file
  std::filesystem::path ply_file = getDataFolder() / "test_cube.ply";
  sf::VertexArray test;
  std::cout << "Path to .ply file: " << ply_file.string() << std::endl;

  happly::PLYData ply_data = data_loader.LoadDataFromPlyFile(ply_file.string());
  std::cout << "Data loaded from .ply file." << std::endl;

  // create a Fragment3D object from the loaded data
  projection_generator::Fragment3D fragment{ply_data};
  std::cout << "Fragment3D object created." << std::endl;

  projection_generator::Projector projector;
  std::cout << "Projector object created." << std::endl;

  // // Rotate the fragment about the Y-axis with 12
  projector.RotateFragmentAboutY(fragment, 48);

  // print out the number of vertex arrays produced and the size of each
  const auto &projected_shapes = projector.GetProjectedShapes();
  std::cout << "Number of projected shapes: " << projected_shapes.size()
            << std::endl;
  for (size_t i = 0; i < projected_shapes.size(); ++i) {
    std::cout << "Projected shape " << i << " has "
              << projected_shapes[i].getVertexCount() << " vertices."
              << std::endl;

    std::cout << "Projected shape size: [x: "
              << projected_shapes[i].getBounds().size.x
              << ", y: " << projected_shapes[i].getBounds().size.y << "]"
              << std::endl;

    // print the position of the bounds
    std::cout << "Projected shape bounds position: [x: "
              << projected_shapes[i].getBounds().position.x
              << ", y: " << projected_shapes[i].getBounds().position.y << "]"
              << std::endl;
  }
  // create the window
  sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");

  // zoom in on the center of the window
  sf::View view = window.getView();
  view.zoom(0.01f); // Zoom in
  window.setView(view);
  // set the frame rate limit
  window.setFramerateLimit(12);
  // run the program as long as the window is open
  while (window.isOpen()) {
    // check all the window's events that were triggered since the last
    // iteration of the loop
    while (const std::optional event = window.pollEvent()) {
      // "close requested" event: we close the window
      if (event->is<sf::Event::Closed>())
        window.close();
    }

    // clear the window with black color
    window.clear(sf::Color::Black);

    // draw once of the projected shapes and keep a counter to cycle through
    // them per frame
    static size_t current_shape_index = 0;
    if (current_shape_index < projected_shapes.size()) {
      const auto &shape = projected_shapes[current_shape_index];
      window.draw(shape);
      current_shape_index = (current_shape_index + 1) % projected_shapes.size();
    } else {
      current_shape_index = 0; // Reset if we exceed the number of shapes
      window.draw(projected_shapes[0]); // Draw the first shape
    }

    // end the current frame
    window.display();
  }
  return 0;
}

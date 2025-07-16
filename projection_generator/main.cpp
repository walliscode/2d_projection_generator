

#include "Fragment3D.h"
#include "Projector.h"
#include "VoxModel.h"
#include "directory_paths.h"
#include <iostream>
int main() {

  // create a VoxModel instance
  VoxModel model;

  // get path to the .vox file
  std::filesystem::path file_path = getDataFolder() / "chr_knight.vox";
  // load the model from a file
  if (!loadVox(file_path.string(), model)) {
    std::cerr << "Failed to load model." << std::endl;
    return 1;
  }
  std::cout << "Model loaded successfully!" << std::endl;

  // create a Fragment3D instance from the VoxModel
  projection_generator::Fragment3D fragment(model);

  // create a Projector instance
  projection_generator::Projector projector;

  // project the Fragment3D onto a 2D plane
  projector.RotateFragmentAboutY(fragment, 48);

  // get the projected shapes
  auto projected_shapes = projector.GetProjectedShapes();

  // print out the number of points in the projected shapes
  std::cout << "Projected shapes count: " << projected_shapes.size()
            << std::endl;
  for (const auto &shape : projected_shapes) {
    std::cout << "[DEBUG] Shape has " << shape.getVertexCount() << " vertices."
              << std::endl;
  }
  // create the window
  sf::RenderWindow window(sf::VideoMode({800, 600}), "My window");

  // zoom in on the center of the window
  sf::View view = window.getView();
  view.zoom(0.8f); // Zoom in
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

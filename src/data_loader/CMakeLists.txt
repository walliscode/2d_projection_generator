add_library(data_loader
DataLoader.cpp)


target_include_directories(data_loader
  PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}



)
target_link_libraries(data_loader
  PUBLIC
  SFML::Graphics
happly
)

add_library(structures
Vertex3.cpp
Fragment3D.cpp
VoxModel.cpp
)

target_include_directories(structures
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}
${CMAKE_SOURCE_DIR}/include/happly/
)




target_link_libraries(structures
PUBLIC
SFML::Graphics
happly
glm::glm
)

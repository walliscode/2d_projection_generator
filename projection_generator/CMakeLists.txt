add_executable(projection_generator
main.cpp
)

target_link_libraries(projection_generator
PUBLIC
SFML::Graphics
projections
structures
data_loader
directory_paths
)

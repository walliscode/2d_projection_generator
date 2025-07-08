add_library(projections
Projector.cpp
)

target_include_directories(projections
PUBLIC
${CMAKE_CURRENT_SOURCE_DIR}

)




target_link_libraries(projections
PUBLIC
SFML::Graphics
structures

)

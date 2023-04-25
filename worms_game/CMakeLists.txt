include("${CMAKE_HOME_DIRECTORY}/cmake/compile_options.cmake")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/../")

set(CMAKE_EXE_LINKER_FLAGS "-lsfml-graphics -lsfml-window -lsfml-system")

add_subdirectory(src)

target_link_libraries(worms
  PUBLIC
    sfml-graphics
    sfml-window
    sfml-system
  )

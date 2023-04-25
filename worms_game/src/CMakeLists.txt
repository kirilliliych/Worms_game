add_executable(worms)

file(GLOB_RECURSE WORMS_INCLUDE *.hpp *.h)
file(GLOB_RECURSE WORMS_SOURCE *.cpp *.c)

target_include_directories(worms
  PUBLIC
    .
  )

target_sources(worms
  PUBLIC
    ${WORMS_INCLUDE}
  PRIVATE
    ${WORMS_SOURCE}
  )
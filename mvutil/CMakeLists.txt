file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

add_library(mvutil ${sources} ${headers})

target_include_directories(mvutil PUBLIC ..)

target_link_libraries(mvutil)

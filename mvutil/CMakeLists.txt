file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

add_library(mvutil ${sources} ${headers} Log.cpp Log.h)

target_include_directories(mvutil PUBLIC ..)

target_link_libraries(mvutil)

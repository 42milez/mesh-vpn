file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

add_library(mvnetwork ${sources} ${headers})

target_include_directories(mvnetwork PUBLIC ..)
target_include_directories(mvcrypt PUBLIC ${CMAKE_BINARY_DIR}/INSTALL/include)
#target_include_directories(mvnetwork SYSTEM PUBLIC)

target_link_libraries(mvnetwork)

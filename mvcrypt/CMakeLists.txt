file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

add_library(mvcrypt ${sources} ${headers})

target_include_directories(mvcrypt PUBLIC ..)
target_include_directories(mvcrypt PUBLIC ${CMAKE_BINARY_DIR}/INSTALL/include)
#target_include_directories(mvcrypt SYSTEM PUBLIC)

add_dependencies(mvcrypt boost)

target_link_libraries(mvcrypt)

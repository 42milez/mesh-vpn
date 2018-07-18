file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

add_library(mvcrypt ${sources} ${headers})

target_include_directories(mvcrypt PUBLIC ..)
#target_include_directories(mvcrypt PUBLIC ${CMAKE_BINARY_DIR}/INSTALL/include)
#target_include_directories(mvcrypt SYSTEM PUBLIC)

#add_dependencies(mvcrypt)

#target_include_directories(mvcrypt SYSTEM PUBLIC ${Boost_INCLUDE_DIR})

target_link_libraries(
  mvcrypt
  PUBLIC
  Boost::boost
)

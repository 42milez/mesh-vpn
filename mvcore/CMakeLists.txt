file(GLOB sources "*.cpp")
file(GLOB headers "*.h")

add_library(mvcore ${sources} ${headers})

add_dependencies(mvcore BuildInfo.h)

# This "local" include path will be propagated to all other libs.
target_include_directories(mvcore PUBLIC ..)

# Needed to prevent including system-level boost headers:
target_include_directories(mvcore SYSTEM PUBLIC)

target_link_libraries(mvcore)

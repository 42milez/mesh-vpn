hunter_add_package(GTest)

add_executable(mvpn_test mvpn_test.cpp)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(mvpn_test GTest::main)

find_package(GMock CONFIG REQUIRED)
target_link_libraries(mvpn_test GMock::gmock)

if(NOT EXISTS "${GTest_LICENSES}")
    message(FATAL_ERROR "File not found: '${GTest_LICENSES}")
endif()

message("License file: '${GTest_LICENSES}'")



#include_directories("${PROJECT_SOURCE_DIR}")
#add_test(NAME mvpn_test COMMAND mvpn_test)

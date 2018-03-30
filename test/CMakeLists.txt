include_directories("${PROJECT_SOURCE_DIR}")

add_executable(mvpn_test mvpn_test.cpp)
target_link_libraries(mvpn_test gtest gmock_main)
#add_test(NAME mvpn_test COMMAND mvpn_test)

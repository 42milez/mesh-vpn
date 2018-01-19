function (create_build_info)

    # OS
    set(BUILD_OS "${CMAKE_SYSTEM_NAME}")

    # Compiler
    if (CMAKE_COMPILER_IS_MINGW)
        set(BUILD_COMPILER "mingw")
    elseif (CMAKE_COMPILER_IS_MSYS)
        set(BUILD_COMPILER "msys")
    elseif (CMAKE_COMPILER_IS_GNUCXX)
        set(BUILD_COMPILER "g++")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        set(BUILD_COMPILER "msvc")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        set(BUILD_COMPILER "clang")
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang")
        set(BUILD_COMPILER "appleclang")
    else ()
        set(BUILD_COMPILER "unknown")
    endif ()

    # Platform
    set(BUILD_PLATFORM "${BUILD_OS}/${BUILD_COMPILER}")

    # Build Type
    if (CMAKE_BUILD_TYPE)
        set(_cmake_build_type ${CMAKE_BUILD_TYPE})
    else ()
        set(_cmake_build_type ${CMAKE_CFG_INDIR})
    endif ()

    # Generate header file
    # cmake Options: https://cmake.org/cmake/help/v3.6/manual/cmake.1.html
    add_custom_target(BuildInfo.h ALL
            WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
            COMMAND ${CMAKE_COMMAND} -DSOURCE_DIR="${PROJECT_SOURCE_DIR}" -DBUILDINFO_IN="${CMAKE_DIR}/templates/BuildInfo.h.in" -DDST_DIR="${PROJECT_BINARY_DIR}/include" -DCMAKE_DIR="${CMAKE_DIR}"
            -DBUILD_TYPE="${_cmake_build_type}"
            -DBUILD_PLATFORM="${BUILD_PLATFORM}"
            -DPROJECT_VERSION="${PROJECT_VERSION}"
            -P "${SCRIPTS_DIR}/buildinfo.cmake"
    )

    include_directories(BEFORE ${PROJECT_BINARY_DIR})

endfunction ()

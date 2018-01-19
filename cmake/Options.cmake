macro (configure_project)

    if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
        set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Build type on single-configuration generators" FORCE)
    endif ()

    include(BuildInfo)

    create_build_info()

endmacro ()

include("cmake/CMakeParseArguments.cmake")

macro (replace_if_different SOURCE DST)

    set(extra_macro_args ${ARGN})
    set(options CREATE)
    set(one_value_args)
    set(multi_value_args)

    cmake_parse_arguments(REPLACE_IF_DIFFERENT "${options}" "${one_value_args}" "${extra_macro_args}")

    if (REPLACE_IF_DIFFERENT_CREATE AND (NOT (EXISTS "${DST}")))
        file(WRITE "${DST}" "")
    endif ()

    execute_process(COMMAND ${CMAKE_COMMAND} -E compare_files "${SOURCE}" "${DST}" RESULT_VARIABLE DIFFERENT OUTPUT_QUIET ERROR_QUIET)

    if (DIFFERENT)
        execute_process(COMMAND ${CMAKE_COMMAND} -E rename "${SOURCE}" "${DST}")
    else ()
        execute_process(COMMAND ${CMAKE_COMMAND} -E remove "${SOURCE}")
    endif ()

endmacro ()

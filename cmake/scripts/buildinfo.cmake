if (NOT BUILD_TYPE)
    set(BUILD_TYPE "unknown")
endif ()

if (NOT BUILD_PLATFORM)
    set(BUILD_PLATFORM "unknown")
endif ()

set(TMPFILE "${DST_DIR}/BuildInfo.h.tmp")
set(OUTFILE "${DST_DIR}/BuildInfo.h")

configure_file("${BUILDINFO_IN}" "${TMPFILE}")

include("${CMAKE_DIR}/Utils.cmake")

replace_if_different("${TMPFILE}" "${OUTFILE}" CREATE)

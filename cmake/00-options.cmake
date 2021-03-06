option(PROFILE "Profile with google perftools" OFF)
#option(COVERAGE "Coverage" OFF)
option(ENABLE_VALGRIND "Tests under valgring (test-valgrind target)" ON)
option(ENABLE_ASAN  "Enable ASAN" OFF)
option(ENABLE_UBSAN "Enable UBSAN" OFF)
option(ENABLE_TSAN  "Enable TSAN" OFF)
option(EXPORT_COMPILE "Export compile database" OFF)
option(BUILD_TESTING "Build tests" ON)

set(allowedBuildDepsTypes "off" "static" "shared")
set(BUILD_DEPS "off" CACHE STRING "Build deps (off/shared/static)")
if(NOT BUILD_DEPS IN_LIST allowedBuildDepsTypes)
    message(FATAL_ERROR "Invalid build dependecies ${BUILD_DEPS}, use one from off/shared/static")
endif()

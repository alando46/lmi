cmake_minimum_required(VERSION 3.21)

project(
    "lmi"
    VERSION 0.1.0
    LANGUAGES CXX)

# Global CMake variables are set here
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Options
option(USE_CPM "Whether to use CPM." ON)
option(ENABLE_WARNINGS "Enable to add warnings to a target." OFF)
option(ENABLE_WARNINGS_AS_ERRORS "Enable to treat warnings as errors." OFF)
option(ENABLE_TESTING "Enable a Unit Testing build." ON)
option(ENABLE_COVERAGE "Enable a Code Coverage build." OFF)
option(ENABLE_CLANG_TIDY "Enable to add clang tidy." OFF)
option(ENABLE_SANITIZE_ADDR "Enable address sanitize." OFF)
option(ENABLE_SANITIZE_UNDEF "Enable undefined sanitize." OFF)
option(ENABLE_SANITIZE_LEAK "Enable leak sanitize (Gcc/Clang only)." OFF)
option(ENABLE_SANITIZE_THREAD "Enable thread sanitize (Gcc/Clang only)." OFF)
option(ENABLE_CLANG_FORMAT "Enable to add clang-format." ON)
option(ENABLE_CMAKE_FORMAT "Enable to add cmake-format." ON)
option(ENABLE_LTO "Enable to add Link Time Optimization." ON)

# Project/Library Names
set(LIBRARY_NAME "lmi")
set(UNIT_TEST_NAME "lmi_unit_tests")

# debug for msvc
foreach(OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${OUTPUTCONFIG} OUTPUTCONFIG)
    set_target_properties(lmi PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/lib"
        LIBRARY_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/lib"
        RUNTIME_OUTPUT_DIRECTORY_${OUTPUTCONFIG} "${CMAKE_BINARY_DIR}/${OUTPUTCONFIG}/bin"
    )
endforeach()

# CMAKE MODULES
set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake/)
include(ConfigSafeGuards)
include(AddGitSubmodule)
include(Docs)
include(Tools)

# if(ENABLE_WARNINGS)
#     include(Warnings)
# endif()
include(Warnings)


if(ENABLE_SANITIZE_ADDR OR ENABLE_SANITIZE_UNDEF)
    include(Sanitizer)
    add_sanitizer_flags()
endif()

if(ENABLE_COVERAGE)
    include(CodeCoverage)
    append_coverage_compiler_flags()
endif()

if(ENABLE_LTO)
    include(LTO)
endif()

# EXTERNAL LIBRARIES
include(FetchContent)
FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/libcpr/cpr.git
                         GIT_TAG 3b15fa82ea74739b574d705fea44959b58142eb8)
FetchContent_MakeAvailable(cpr)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG        v3.5.3
)

FetchContent_MakeAvailable(Catch2)


message(STATUS "Using CPM")
include(CPM)
cpmaddpackage("gh:fmtlib/fmt#10.2.1")
# cpmaddpackage("gh:catchorg/Catch2@3.5.3")
cpmaddpackage("gh:danielaparker/jsoncons@0.173.4")
cpmaddpackage("gh:gabime/spdlog@1.13.0")


# SUB DIRECTORIES
add_subdirectory(configured)
add_subdirectory(include)
add_subdirectory(tests)

# add lmi subdirectory for development only
set(SUBDIR_PATH "${CMAKE_CURRENT_SOURCE_DIR}/lmi")
if(EXISTS ${SUBDIR_PATH})
    # executable is only for development
    set(EXECUTABLE_NAME "main")
    # Add the subdirectory to the build
    add_subdirectory(${SUBDIR_PATH})
else()
    message(STATUS "Subdirectory ${SUBDIR_PATH} not found, skipping.")
endif()


# INSTALL TARGETS

# install(
#     TARGETS ${EXECUTABLE_NAME}
#     ARCHIVE DESTINATION lib
#     LIBRARY DESTINATION lib
#     RUNTIME DESTINATION bin)

# install(
#     TARGETS ${LIBRARY_NAME}
#     ARCHIVE DESTINATION lib
#     LIBRARY DESTINATION lib)

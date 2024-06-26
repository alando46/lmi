cmake_minimum_required(VERSION 3.21)

set(LIBRARY_INCLUDES "${CMAKE_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}" "${CMAKE_BINARY_DIR}/configured_files/include")
# note: header only library must be set to INTERFACE for add_library and target_include_directories
# change back to static if source files are added
# add_library(${LIBRARY_NAME} STATIC ${LIBRARY_HEADERS})
add_library(${LIBRARY_NAME} INTERFACE)
# specify the directories where the compiler should look for header files
# when compiling the target named `lmi`.
target_include_directories(${LIBRARY_NAME} INTERFACE ${LIBRARY_INCLUDES})

# include headers here for ide completion
target_sources(${LIBRARY_NAME} PRIVATE
    ${LIBRARY_NAME}/api.h
    ${LIBRARY_NAME}/lmi.h
    ${LIBRARY_NAME}/validationError.h
)

target_link_libraries(${LIBRARY_NAME}
    INTERFACE
    jsoncons
    cpr::cpr
)

if(${ENABLE_WARNINGS})
    target_set_warnings(
        TARGET
        ${LIBRARY_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})
endif()

if(${ENABLE_LTO})
    set_target_properties(${LIBRARY_NAME} PROPERTIES LINKER_LANGUAGE CXX)

    target_enable_lto(
        TARGET
        ${LIBRARY_NAME}
        ENABLE
        ON)
endif()

if(${ENABLE_CLANG_TIDY})
    add_clang_tidy_to_target(${LIBRARY_NAME})
endif()

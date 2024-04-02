cmake_minimum_required(VERSION 3.21)

set(LIBRARY_INCLUDES "${CMAKE_BINARY_DIR}" "${CMAKE_CURRENT_SOURCE_DIR}/${LIBRARY_NAME}" "${CMAKE_BINARY_DIR}/configured_files/include")

add_library(${LIBRARY_NAME} STATIC ${LIBRARY_HEADERS})

# specify the directories where the compiler should look for header files
# when compiling the target named `lmi`.
target_include_directories(${LIBRARY_NAME} PUBLIC ${LIBRARY_INCLUDES})

# include headers here for ide completion
target_sources(${LIBRARY_NAME} PRIVATE
    ${LIBRARY_NAME}/api.h
    ${LIBRARY_NAME}/lmi.h
)

target_link_libraries(${LIBRARY_NAME}
    PRIVATE
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

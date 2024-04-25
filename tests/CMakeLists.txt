if(ENABLE_TESTING)
    set(TEST_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/main.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/DoSomething.cpp")
    set(TEST_HEADERS "${CMAKE_CURRENT_SOURCE_DIR}/DoSomething.h")

    add_executable(${UNIT_TEST_NAME} ${TEST_SOURCES} ${TEST_HEADERS})

    target_link_libraries(${UNIT_TEST_NAME} PUBLIC ${LIBRARY_NAME})
    target_link_libraries(${UNIT_TEST_NAME} PRIVATE jsoncons Catch2::Catch2WithMain)

    target_include_directories(${UNIT_TEST_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

    if (MSVC)
        add_custom_command(TARGET lmi_unit_tests POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "${CMAKE_BINARY_DIR}/_deps/curl-build/lib/Debug/libcurl-d.dll"
                "${CMAKE_BINARY_DIR}/_deps/cpr-build/cpr/Debug/cpr.dll"
                "${CMAKE_BINARY_DIR}/_deps/catch2-build/src/Debug/Catch2Maind.dll"
                "${CMAKE_BINARY_DIR}/_deps/catch2-build/src/Debug/Catch2d.dll"
                $<TARGET_FILE_DIR:lmi_unit_tests>
        )
    endif()

    target_set_warnings(
        TARGET
        ${UNIT_TEST_NAME}
        ENABLE
        ${ENABLE_WARNINGS}
        AS_ERRORS
        ${ENABLE_WARNINGS_AS_ERRORS})

    if(ENABLE_COVERAGE)
        set(COVERAGE_MAIN "coverage")
        set(COVERAGE_EXCLUDES
            "${PROJECT_SOURCE_DIR}/lmi/*"
            "${PROJECT_SOURCE_DIR}/cmake/*"
            "${PROJECT_SOURCE_DIR}/docs/*"
            "${PROJECT_SOURCE_DIR}/tests/*"
            "${PROJECT_SOURCE_DIR}/build/*"
            "/usr/include/*")
        setup_target_for_coverage_lcov(
            NAME
            ${COVERAGE_MAIN}
            EXECUTABLE
            ${UNIT_TEST_NAME}
            DEPENDENCIES
            ${UNIT_TEST_NAME})
    endif()
endif()

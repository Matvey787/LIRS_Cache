# cmake -S . -B build && cmake --build build
# ./build/lirs
# cmake --build build --target clean
# ctest --test-dir build -V

cmake_minimum_required(VERSION 3.11)
project(LIRS_Cache CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/inc)

set(SOURCE_FILES
    src/main.cpp
    src/cacheRendering.cpp
)

add_executable(lirs ${SOURCE_FILES})

include(CTest)
enable_testing()

function(create_tests_from_pairs)
    file(GLOB TEST_FILES ${CMAKE_SOURCE_DIR}/tests/test*.dat)

    if(!TEST_FILES)
        message("No test files found.")
        return()
    endif()

    foreach(TEST_FILE ${TEST_FILES})

        get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
        string(REGEX REPLACE "^test" "" TEST_NUMBER ${TEST_NAME})
        
        set(ANS_FILE ${CMAKE_SOURCE_DIR}/tests/ans${TEST_NUMBER}.dat)
        
        if(EXISTS ${ANS_FILE})
            file(READ ${ANS_FILE} EXPECTED_OUTPUT)
            string(STRIP "${EXPECTED_OUTPUT}" EXPECTED_OUTPUT)
            
            add_test(NAME test_${TEST_NUMBER}
                COMMAND bash -c "../build/lirs < test${TEST_NUMBER}.dat"
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
            )
            
            set_tests_properties(test_${TEST_NUMBER} PROPERTIES
                PASS_REGULAR_EXPRESSION "^${EXPECTED_OUTPUT}([ \t\n\r]|$)"
            )
        else()
            message(WARNING "Answer file not found for test ${TEST_NUMBER}: ${ANS_FILE}")
        endif()
    endforeach()
endfunction()

create_tests_from_pairs()

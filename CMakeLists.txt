# cmake -S . -B build && cmake --build build                work with LIRS
# cmake -S . -DOPT_M=ON -B build && cmake --build build     work with OPT

# ./build/lirs
# cmake --build build --target clean
# ctest --test-dir build -V
# g++ -g -O2 -fno-omit-frame-pointer your_program.cpp -o your_program
# -DCMAKE_BUILD_TYPE=RelWithDebInfo

cmake_minimum_required(VERSION 3.11)
project(LIRS_Cache CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(OPT_M "Activate and work with OPT Cache (ideal cache)" OFF)

include_directories(${CMAKE_SOURCE_DIR}/inc)

set(SOURCE_FILES
    src/main.cpp
    src/cacheRendering.cpp
)

if (OPT_M)
    SET(EXEC_FILE_NAME "opt")
else()
    SET(EXEC_FILE_NAME "lirs")
endif()

add_executable(${EXEC_FILE_NAME} ${SOURCE_FILES})

if (OPT_M)
    target_compile_definitions(${EXEC_FILE_NAME} PRIVATE OPT_M)
endif()

include(CTest)
enable_testing()

function(create_tests_from_pairs)
    file(GLOB TEST_FILES ${CMAKE_SOURCE_DIR}/tests/test*.dat)

    if(!TEST_FILES)
        message(WARNING "No test files found.")
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
                COMMAND bash -c "../build/${EXEC_FILE_NAME} < test${TEST_NUMBER}.dat"
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

# cmake -S . -B build && cmake --build build

# ./build/lirs
# cmake --build build --target clean
# ctest --test-dir build -V
# g++ -g -O2 -fno-omit-frame-pointer your_program.cpp -o your_program
# -DCMAKE_BUILD_TYPE=RelWithDebInfo

cmake_minimum_required(VERSION 3.11)
project(LIRS_Cache CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(${CMAKE_SOURCE_DIR}/inc)

set(COMMON_SOURCE_FILES
    src/main.cpp
    src/cacheRendering.cpp
)

add_executable(lirs ${COMMON_SOURCE_FILES})

add_executable(opt ${COMMON_SOURCE_FILES})
target_compile_definitions(opt PRIVATE OPT_M)

include(CTest)
enable_testing()

function(create_tests_for_target TARGET_NAME)
    file(GLOB TEST_FILES ${CMAKE_SOURCE_DIR}/tests/test*.dat)

    if(NOT TEST_FILES)
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
            
            add_test(NAME ${TARGET_NAME}_test_${TEST_NUMBER}
                COMMAND bash -c "../build/${TARGET_NAME} < test${TEST_NUMBER}.dat"
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/tests
            )
            
            set_tests_properties(${TARGET_NAME}_test_${TEST_NUMBER} PROPERTIES
                PASS_REGULAR_EXPRESSION "^${EXPECTED_OUTPUT}([ \t\n\r]|$)"
            )
        else()
            message(WARNING "Answer file not found for test ${TEST_NUMBER}: ${ANS_FILE}")
        endif()
    endforeach()
endfunction()

create_tests_for_target(lirs)
create_tests_for_target(opt)
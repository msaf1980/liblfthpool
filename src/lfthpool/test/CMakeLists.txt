# Scan source dir for standart source files and exclude main.cpp

include_directories(${PROJECT_SOURCE_DIR}/src)

set(REQUIRED_SOURCES)

# if(NOT HAVE_PTHREAD_BARRIER)
#     add_definitions(-DNO_PTHREAD_BARRIER=1)
#     list(APPEND REQUIRED_SOURCES ${PROJECT_SOURCE_DIR}/src/pthread_barrier.c)
# endif()

set(TEST_LIBRARIES Threads::Threads)
if (BUILD_SHARED_LIBS)
    list(APPEND TEST_LIBRARIES lfthpool_shared ${LIBCONCURRENT_SHARED})
else()
    list(APPEND TEST_LIBRARIES lfthpool ${LIBCONCURRENT})
endif()

# Build tests
add_executable(test_lfthpool
    lfthpool_test.c
    lfthpool/lfthpool_no_work.c
    lfthpool/lfthpool_api.c
    lfthpool/lfthpool_pause_resume.c
    lfthpool/lfthpool_worker_try_once.c
    ${REQUIRED_SOURCES}
)
target_link_libraries(test_lfthpool ${TEST_LIBRARIES})
#target_compile_options(test_lfthpool PRIVATE -Wdeclaration-after-statement)
add_test(
    NAME test_lfthpool
    COMMAND $<TARGET_FILE:test_lfthpool>
)
set_tests_properties(test_lfthpool PROPERTIES LABELS "lfthpool")

add_executable(bench_lfthpool lfthpool_bench.c ${REQUIRED_SOURCES})
target_link_libraries(bench_lfthpool ${TEST_LIBRARIES})

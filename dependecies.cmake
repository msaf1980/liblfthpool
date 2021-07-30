include(CheckFunctionExists)
include(CheckSymbolExists)

find_package(PkgConfig REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED) # Threads::Threads
set(CMAKE_REQUIRED_LIBRARIES Threads::Threads) # required libs for check_function_exists
check_symbol_exists(pthread_barrier_init pthread.h HAVE_PTHREAD_BARRIER) # for MacOS
check_symbol_exists(pthread_spin_init pthread.h HAVE_PTHREAD_SPIN) # for MacOS
#check_function_exists(pthread_spin_init HAVE_PTHREAD_SPIN) # for MacOS

if ("${BUILD_DEPS}" STREQUAL "off")
    # Use libraries from OS
else()
    # Init cget
    cget_init(CGET_INIT_OPTS "${BUILD_DEPS}")
    cget_install(CGET_INSTALL_OPTS  "${CMAKE_SOURCE_DIR}/requirements.txt")

    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_BINARY_DIR}/cget")
endif() # BUILD_DEPS

pkg_check_modules(LIBCONCURRENT REQUIRED libconcurrent>=0.0.1)
include_directories(${LIBCONCURRENT_INCLUDE_DIRS})
link_directories(${LIBCONCURRENT_LIBRARY_DIRS})
link_libraries(concurrent)

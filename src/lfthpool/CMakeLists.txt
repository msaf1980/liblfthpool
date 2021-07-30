set(LFTHPOOL_VERSION "0.0.1")

set_version(LFTHPOOL_VERSION LFTHPOOL_VERSION_MAJOR LFTHPOOL_VERSION_MINOR LFTHPOOL_VERSION_PATCH LFTHPOOL_VERSION_REVISION)

set(
    lfthpool_SOURCES
    lfthpool.c
)

add_library(lfthpool STATIC ${lfthpool_SOURCES})
target_link_libraries(lfthpool ${LIBCONCURENT})
set_target_properties(lfthpool
    PROPERTIES OUTPUT_NAME lfthpool
                POSITION_INDEPENDENT_CODE ON
)

include(GNUInstallDirs)

if(BUILD_SHARED_LIBS)
    add_library(lfthpool_shared SHARED ${lfthpool_SOURCES})
    target_link_libraries(lfthpool_shared ${LIBCONCURRENT_SHARED})
    set_target_properties(
        lfthpool_shared
        PROPERTIES OUTPUT_NAME lfthpool
                   POSITION_INDEPENDENT_CODE ON
                   SOVERSION "${LFTHPOOL_VERSION_MAJOR}"
                   VERSION "${LFTHPOOL_VERSION}")
                   
    install(TARGETS lfthpool_shared
            LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
            ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
            PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    )       
endif()

install(TARGETS lfthpool
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

# Install headers
install(DIRECTORY "${PROJECT_SOURCE_DIR}/include/lfthpool"
        DESTINATION "include"
)

configure_file(${PROJECT_SOURCE_DIR}/liblfthpool.pc.in ${CMAKE_BINARY_DIR}/liblfthpool.pc @ONLY)
install(FILES ${CMAKE_BINARY_DIR}/liblfthpool.pc DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/pkgconfig)

if (BUILD_TESTING)
   add_subdirectory(test)
endif() # END TEST

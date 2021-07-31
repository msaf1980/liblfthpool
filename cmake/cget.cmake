function(cget_init CGET_OPTS build_deps)
    set(opts
        --cc ${CMAKE_C_COMPILER}
        --cxx ${CMAKE_CXX_COMPILER}
    )
    # default shared/static
    if(build_deps STREQUAL "static" OR build_deps STREQUAL "shared") 
        list(APPEND opts "--${build_deps}")
    endif()

    string(REPLACE ";" " " _opts "${opts}")
    set(${CGET_OPTS} "${_opts}" PARENT_SCOPE)

    if(NOT EXISTS "${CMAKE_BINARY_DIR}/cget/cget/cget.cmake")
        message("CREATE CGET TOOLCHAIN FILE ${CMAKE_BINARY_DIR}/cget/cget/cget.cmake")
        message("  EXECUTE cget init ${_opts}")
        execute_process(
            COMMAND cget init ${opts}
            WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        )
    endif()
endfunction ()

function(cget_install CGET_OPTS requirements_file)
   set(opts
        --build-type ${CMAKE_BUILD_TYPE}
        -f ${requirements_file}
        -G "${CMAKE_GENERATOR}"
    )

    set(${CGET_OPTS} "--build-type ${CMAKE_BUILD_TYPE} -f ${requirements_file} -G '${CMAKE_GENERATOR}'" PARENT_SCOPE)

    execute_process(
        COMMAND cget install ${opts}
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    )
endfunction ()

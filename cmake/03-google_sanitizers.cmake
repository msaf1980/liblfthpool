if(CMAKE_COMPILER_IS_GNU OR CMAKE_COMPILER_IS_CLANG)
	# ASAN
	if(ENABLE_ASAN)
		append_flag(CMAKE_C_FLAGS_${BUILD_TYPE} "-fsanitize=address")
		append_flag(CMAKE_CXX_FLAGS_${BUILD_TYPE} "-fsanitize=address")
		append_flag(CMAKE_C_FLAGS_${BUILD_TYPE} "-fno-omit-frame-pointer")
		append_flag(CMAKE_CXX_FLAGS_${BUILD_TYPE} "-fno-omit-frame-pointer")
		append_flag(CMAKE_C_FLAGS_${BUILD_TYPE} "-fno-optimize-sibling-calls")
		append_flag(CMAKE_CXX_FLAGS_${BUILD_TYPE} "-fno-optimize-sibling-calls")
		append_flag(CMAKE_C_FLAGS_${BUILD_TYPE} "-fno-common")
		append_flag(CMAKE_CXX_FLAGS_${BUILD_TYPE} "-fno-common")
		#append_flag(CMAKE_EXE_LINKER_FLAGS_${BUILD_TYPE} "-lasan")
	    #append_flag(CMAKE_SHARED_LINKER_FLAGS_${BUILD_TYPE} "-lasan")

	    # For enable UBSan
		append_flag(CMAKE_C_FLAGS_${BUILD_TYPE} "-fsanitize=undefined")
		append_flag(CMAKE_CXX_FLAGS_${BUILD_TYPE} "-fsanitize=undefined")
		#append_flag(CMAKE_EXE_LINKER_FLAGS_${BUILD_TYPE} "-lubsan")
	    #append_flag(CMAKE_SHARED_LINKER_FLAGS_${BUILD_TYPE} "-lubsan")

		set(ENABLE_VALGRIND OFF)
	endif()
	
	# TSAN
	if(ENABLE_TSAN)
		append_flag(CMAKE_C_FLAGS_${BUILD_TYPE} "-fsanitize=thread")
		append_flag(CMAKE_CXX_FLAGS_${BUILD_TYPE} "-fsanitize=thread")
		append_flag(CMAKE_C_FLAGS_${BUILD_TYPE} "-fno-omit-frame-pointer")
		append_flag(CMAKE_CXX_FLAGS_${BUILD_TYPE} "-fno-omit-frame-pointer")
		append_flag(CMAKE_C_FLAGS_${BUILD_TYPE} "-fno-optimize-sibling-calls")
		append_flag(CMAKE_CXX_FLAGS_${BUILD_TYPE} "-fno-optimize-sibling-calls")
	    #append_flag(CMAKE_EXE_LINKER_FLAGS_${BUILD_TYPE} "-ltsan")
		#append_flag(CMAKE_SHARED_LINKER_FLAGS_${BUILD_TYPE} "-ltsan")

		set(ENABLE_VALGRIND OFF)
	endif()
endif(CMAKE_COMPILER_IS_GNU OR CMAKE_COMPILER_IS_CLANG)

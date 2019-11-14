cmake_minimum_required(VERSION 2.8)

message("-- MSG: Add dir ${CMAKE_CURRENT_SOURCE_DIR}")

set(PROJ_NAME ba_tester)

project(${PROJ_NAME} C)

if (DEFINED SYNC)
	if (${SYNC} STREQUAL mutex)
		set(EXTRA_SRC "ba_pt_mutex_sync.c")
		set(EXTRA_LIB "-lpthread")
		message("-- MSG: synchronization will be built base on mutex")

	elseif(${SYNC} STREQUAL semaphore)
		set(EXTRA_SRC "ba_sem_sync.c")
		set(EXTRA_LIB "-lpthread")
		message("-- MSG: synchronization will be built base on semaphore")

	else()
		set(EXTRA_SRC "")
		set(EXTRA_LIB "")
		message("-- MSG: there is no synchronization, just sequential execution")
	endif()

else()
	set(EXTRA_SRC "")
	set(EXTRA_LIB "")
	message("-- MSG: there is no synchronization, just sequential execution")
endif()

set(CMAKE_C_FLAGS "-Wall -Werror")

set(C_SRC	"main.c"
			${EXTRA_SRC})

add_subdirectory("blockalloc")

include_directories("blockalloc/include")

add_executable(${PROJ_NAME}  ${C_SRC})

target_link_libraries(${PROJ_NAME} blockalloc ${EXTRA_LIB})
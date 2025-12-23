cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)

set(PROJECT_NAME Pikey)

project(${PROJECT_NAME})

file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp" "${CMAKE_CURRENT_SOURCE_DIR}/*.h")

add_library(${PROJECT_NAME}Lib STATIC ${SRC_FILES})
add_executable(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/app/pikey.cpp)

include(cmake/CPM.cmake)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_custom_target(export-compile-commands ALL
  COMMAND ${CMAKE_COMMAND} -E copy_if_different
  "${CMAKE_BINARY_DIR}/compile_commands.json"
  "${CMAKE_SOURCE_DIR}/compile_commands.json"
  COMMENT "Export compile_commands.json to source root"
)
# Add Discord Bot Library
CPMAddPackage("gh:brainboxdotcc/DPP@10.1.2")

target_link_libraries(${PROJECT_NAME} PRIVATE dpp)
target_link_libraries(${PROJECT_NAME}Lib PRIVATE dpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}Lib)

target_include_directories(${PROJECT_NAME} PRIVATE src)
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_include_directories(${PROJECT_NAME}Lib PRIVATE include)

if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD # Copy every needed dll to the bin dir
        COMMAND ${CMAKE_COMMAND} -E copy -t $<TARGET_FILE_DIR:${PROJECT_NAME}> $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}>
        COMMAND ${CMAKE_COMMAND} -E copy
        "${DPP_BINARY_DIR}/library/zlib1.dll"
        "${DPP_BINARY_DIR}/library/libssl-1_1-x64.dll"
        "${DPP_BINARY_DIR}/library/libcrypto-1_1-x64.dll"
        "${DPP_BINARY_DIR}/library/opus.dll"
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMAND_EXPAND_LISTS
)
endif (WIN32)

if (DPP_ADDED)
    message(STATUS "Applying patch to DPP...")

    execute_process(
        COMMAND git apply ${CMAKE_SOURCE_DIR}/patches/poll-include.patch
        WORKING_DIRECTORY ${DPP_SOURCE_DIR}
        RESULT_VARIABLE PATCH_RESULT
        ERROR_VARIABLE PATCH_ERROR
    )

    if (PATCH_RESULT)
        message(STATUS "Patch does not apply for current version.")
    else()
        message(STATUS "Patch applied successfully.")
    endif()
endif()
# End Discord Bot Library

install(TARGETS ${PROJECT_NAME}
        BUNDLE DESTINATION bin
        LIBRARY DESTINATION lib
        RUNTIME DESTINATION bin
)

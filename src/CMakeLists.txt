add_executable(${PROJECT_NAME} main.c)

add_subdirectory(files)

target_link_libraries(${PROJECT_NAME}
        files-lib)

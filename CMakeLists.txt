cmake_minimum_required(VERSION 3.25)
project(OS-Project)

add_subdirectory(src)
configure_file(${PROJECT_SOURCE_DIR}/src/scripts/score.sh ${PROJECT_BINARY_DIR}/src/score.sh COPYONLY)
configure_file(${PROJECT_SOURCE_DIR}/src/scripts/dir-file.sh ${PROJECT_BINARY_DIR}/src/dir-file.sh COPYONLY)

install(TARGETS ${PROJECT_NAME}
        RUNTIME DESTINATION ${PROJECT_SOURCE_DIR}/bin)
install(FILES ${PROJECT_SOURCE_DIR}/src/scripts/score.sh DESTINATION ${PROJECT_SOURCE_DIR}/bin)
install(FILES ${PROJECT_SOURCE_DIR}/src/scripts/dir-file.sh DESTINATION ${PROJECT_SOURCE_DIR}/bin)

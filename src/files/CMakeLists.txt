add_library(files-lib STATIC files.c)
target_include_directories(files-lib PUBLIC ${PROJECT_SOURCE_DIR}/include/)
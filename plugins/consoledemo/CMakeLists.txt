
aux_source_directory(. srcs)
add_executable(consoledemo ${srcs})
target_link_libraries(consoledemo dl)


#插件文件存放路径
set(PLUGINS_PATH "${PROJECT_BINARY_DIR}/plugins/")
set(LIBRARY_OUTPUT_PATH ${PLUGINS_PATH})
message(STATUS "plugins path: ${LIBRARY_OUTPUT_PATH}")

include_directories(
    .
)

#cmake和程序共享变量
configure_file (
  "x3config.h.in"
  "x3config.h"
  @ONLY
)

add_subdirectory(x3manager)
add_subdirectory(plsimple)
add_subdirectory(pltempl)

aux_source_directory(. srcs)
add_executable(consoledemo ${srcs})
target_link_libraries(consoledemo dl)
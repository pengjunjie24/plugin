
#cmake和程序共享变量
configure_file (
  "config.h.in"
  "config.h"
  @ONLY
)

SUBDIRLIST(subdirs ${CMAKE_CURRENT_SOURCE_DIR})

foreach(subdir ${subdirs})
    add_subdirectory(${subdir})
endforeach()
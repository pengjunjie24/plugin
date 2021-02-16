
configure_file (
  "config.h.in"
  "${CMAKE_CURRENT_SOURCE_DIR}/config.h"
  @ONLY
)

aux_source_directory(. srcs)
add_executable(configuredemo ${srcs})


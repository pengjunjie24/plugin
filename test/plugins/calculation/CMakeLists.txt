#CURRENT_FOLDER 为当前文件夹名称
string( REGEX REPLACE ".*/(.*)" "\\1" CURRENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} )

aux_source_directory(./source srcs)
add_library(${CURRENT_FOLDER} SHARED ${srcs})
#设置输出动态库前后缀名称
set_target_properties(${CURRENT_FOLDER} PROPERTIES PREFIX "" SUFFIX ".pln")


aux_source_directory(./source srcs)
add_library(x3manager SHARED ${srcs})
#设置输出动态库前后缀名称
set_target_properties(x3manager PROPERTIES PREFIX "" SUFFIX ".pln")
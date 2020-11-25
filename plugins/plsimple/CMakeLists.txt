aux_source_directory(./source srcs)
add_library(plsimple SHARED ${srcs})
#设置输出动态库前后缀名称
set_target_properties(plsimple PROPERTIES PREFIX "" SUFFIX ".pln")


#头文件包含路径
include_directories(
    ./include
    )

set(EXECUTABLE_OUTPUT_PATH ${EXECUTABLE_OUTPUT_PATH}/nlohmannjson-3.9.1)
add_subdirectory(test)

#pragma once

#include <string>
#include <stdlib.h>
#include <stdint.h>

#define OUTAPI extern "C" __attribute__((visibility("default")))//此宏修饰符号外部可见
#define LOCALAPI    __attribute__((visibility("hidden")))//用此宏修饰符号仅在内部.so文件可见

typedef void* HMODULE;//动态库句柄
typedef void* PROC;//动态库中函数

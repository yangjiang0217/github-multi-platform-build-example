#pragma once

#include <string>

// 跨平台导出宏：Windows 需要 dllexport/dllimport，Unix 默认可见
// HELLO_STATIC: 静态库场景，禁用导入导出（由 CMake 编译选项定义）
#if defined(_WIN32)
    #if defined(HELLO_STATIC)
        #define HELLO_API
    #elif defined(HELLO_BUILDING_DLL)
        #define HELLO_API __declspec(dllexport)
    #else
        #define HELLO_API __declspec(dllimport)
    #endif
#else
    #define HELLO_API __attribute__((visibility("default")))
#endif

namespace hello {

// 返回 "Hello World!" 字符串
HELLO_API std::string greet();

}  // namespace hello

cmake_minimum_required(VERSION 3.20)
project(hello_world CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC 默认按系统代码页(GBK)读取源文件，UTF-8 无 BOM 的中文注释会导致乱码/编译错误
if(MSVC)
    add_compile_options(/utf-8)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Release CACHE STRING "Build type" FORCE)
endif()

# 可选：同时编译静态库（默认 OFF，动态库 + 可执行程序）
# 用法: cmake -DBUILD_STATIC_LIB=ON
option(BUILD_STATIC_LIB "Also build the static library" OFF)

# ============================================================
# 动态库：libhello（Windows: hello.dll / Linux: libhello.so / macOS: libhello.dylib）
# ============================================================
add_library(hello SHARED src/hello.cpp)
target_include_directories(hello PUBLIC include)
target_compile_definitions(hello PRIVATE HELLO_BUILDING_DLL)

# ============================================================
# 可执行程序：调用动态库接口
# ============================================================
add_executable(hello_app src/main.cpp)
target_link_libraries(hello_app PRIVATE hello)

# ============================================================
# 可选静态库：libhello_static（Windows: hello_static.lib / Unix: libhello_static.a）
# 输出名与动态库区分，避免与动态库的导入库同名冲突
# ============================================================
if(BUILD_STATIC_LIB)
    add_library(hello_static STATIC src/hello.cpp)
    set_target_properties(hello_static PROPERTIES OUTPUT_NAME hello_static)
    target_compile_definitions(hello_static PRIVATE HELLO_STATIC)
    target_include_directories(hello_static PUBLIC include)
endif()

# ============================================================
# 发布规则：CI 用 cmake --install 收集产物打包
# ============================================================
install(TARGETS hello hello_app
        RUNTIME DESTINATION bin   # exe / .dll
        LIBRARY DESTINATION lib   # .so / .dylib
        ARCHIVE DESTINATION lib)  # .a / .lib（导入库）

if(BUILD_STATIC_LIB)
    install(TARGETS hello_static
            RUNTIME DESTINATION bin
            LIBRARY DESTINATION lib
            ARCHIVE DESTINATION lib)
endif()

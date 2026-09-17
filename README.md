# Hello World — 多平台构建（CMake + C++17）

C++17 示例项目：一个动态库（可选静态库）+ 一个调用库接口的可执行程序，
使用 CMake 管理，GitHub Actions 自动并行构建 6 个平台的二进制产物。

## 项目结构

```
hello_world/
├── CMakeLists.txt                    # C++17，动态库 + 可执行程序 + 可选静态库
├── include/
│   └── hello/hello.h                 # 库接口（跨平台导出宏）
├── src/
│   ├── hello.cpp                     # 动态库实现
│   └── main.cpp                      # 可执行程序（调用库接口）
├── cmake/
│   └── linux-arm64-toolchain.cmake   # Linux arm64 交叉编译工具链（C/C++）
├── .github/workflows/build.yml       # 多平台构建流水线
├── .gitignore
└── README.md
```

## 构建目标

| 目标 | 类型 | 产物（Windows / Linux / macOS） |
|---|---|---|
| `hello` | 动态库 | `hello.dll` / `libhello.so` / `libhello.dylib` |
| `hello_static`（可选） | 静态库 | `hello_static.lib` / `libhello_static.a` |
| `hello_app` | 可执行程序 | `hello_app.exe` / `hello_app`（调用动态库接口） |

## 构建产物（Release 附件）

每个平台一个压缩包，内含 `bin/` 与 `lib/` 目录（由 CMake install 规则收集）：

| 目标平台 | Release 附件 |
|---|---|
| Windows x86 | `hello-windows-x86.zip` |
| Windows x64 | `hello-windows-x64.zip` |
| Windows ARM64 | `hello-windows-arm64.zip` |
| Linux x86_64 | `hello-linux-x86_64.tar.gz` |
| Linux arm64 | `hello-linux-arm64.tar.gz` |
| macOS universal | `hello-macos-universal.tar.gz` |

## 本地构建

```bash
# Windows（VS 生成器）
cmake -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release

# Windows 32 位 / ARM64：-A Win32 / -A ARM64

# Linux / macOS
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Linux arm64（交叉编译）
cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/linux-arm64-toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build

# macOS universal
cmake -B build -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -DCMAKE_BUILD_TYPE=Release
cmake --build build

# 可选：同时编译静态库
cmake -B build -DBUILD_STATIC_LIB=ON ...
```

运行（Windows 需 DLL 在 PATH 或同目录）：
```bash
# Windows
.\build\Release\hello_app.exe     # 或 .\build\hello_app.exe
# Linux / macOS
./build/hello_app
```

安装（模拟 CI 打包收集，产出 `dist/hello/bin`、`dist/hello/lib`）：
```bash
cmake --install build --config Release --prefix dist/hello
```

## CI 使用方法

1. 推送到 GitHub。
2. 打 tag 触发自动构建：
   ```bash
   git tag v1.0
   git push --tags
   ```
   或到 Actions 页面手动触发。
3. 构建完成自动创建 Release，6 个平台的压缩包作为附件上传。

## 换项目复用

只改 `.github/workflows/build.yml` 顶部的 `env.PROJECT`；
发布哪些目标在 `CMakeLists.txt` 的 `install(TARGETS ...)` 中声明即可。

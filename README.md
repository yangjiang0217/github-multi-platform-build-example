# Hello World — 多平台构建（CMake）

极简 C HelloWorld，使用 **CMake** 管理构建，GitHub Actions 自动并行构建 6 个平台的二进制产物。

## 构建产物

构建完成后每个平台的二进制会**压缩打包**上传到 Release 附件（Windows 用 zip，Linux/macOS 用 tar.gz）。

| 目标平台 | 架构 | Release 附件 |
|---|---|---|
| Windows 32 位 | x86 | `hello-windows-x86.zip` |
| Windows 64 位 | x64 | `hello-windows-x64.zip` |
| Windows ARM64 | arm64 | `hello-windows-arm64.zip` |
| Linux | x86_64 | `hello-linux-x86_64.tar.gz` |
| Linux | arm64 | `hello-linux-arm64.tar.gz` |
| macOS 通用 | x86_64 + arm64 | `hello-macos-universal.tar.gz` |

## 构建方式说明

| 平台 | 构建方式 |
|---|---|
| Windows x86 / x64 / ARM64 | `windows-latest` 上原生 MSVC 生成器，`cmake -A Win32 / -A x64 / -A ARM64` |
| Linux x86_64 | `ubuntu-latest` 原生 gcc，静态链接 |
| Linux arm64 | `ubuntu-latest` 上用 `aarch64-linux-gnu-gcc` 交叉编译（`cmake/linux-arm64-toolchain.cmake`），静态链接 |
| macOS universal | `macos-latest` 原生 clang，`CMAKE_OSX_ARCHITECTURES="x86_64;arm64"` 生成通用二进制 |

## 本地构建

```bash
# Windows（VS 开发者命令行或任何带 CMake 的环境）
cmake -B build -A x64
cmake --build build --config Release

# Windows 32 位 / ARM64
cmake -B build-x86 -A Win32
cmake --build build-x86 --config Release
cmake -B build-arm64 -A ARM64
cmake --build build-arm64 --config Release

# Linux x86_64
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build

# Linux arm64（需安装交叉编译器: sudo apt install gcc-aarch64-linux-gnu）
cmake -B build -DCMAKE_TOOLCHAIN_FILE=cmake/linux-arm64-toolchain.cmake -DCMAKE_BUILD_TYPE=Release
cmake --build build

# macOS universal
cmake -B build -DCMAKE_OSX_ARCHITECTURES="x86_64;arm64" -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

## CI 使用方法

1. 把本仓库推送到 GitHub。

2. 打 tag 推送，触发自动构建：

   ```bash
   git tag v1.0
   git push --tags
   ```

   也可以在 GitHub 仓库 **Actions → Build MultiPlatform HelloWorld (CMake) → Run workflow**
   手动触发（已启用 `workflow_dispatch`）。

3. 构建完成后自动创建 GitHub Release（tag 触发时），
   6 个平台的二进制全部作为附件上传到 Release 页面。

## 目录结构

```
hello_world/
├── CMakeLists.txt                 # CMake 构建定义
├── hello.c                        # 源码
├── cmake/
│   └── linux-arm64-toolchain.cmake  # Linux arm64 交叉编译工具链
├── .github/
│   └── workflows/
│       └── build.yml              # 多平台构建流水线
└── README.md
```

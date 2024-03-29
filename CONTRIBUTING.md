# Contributing
Thank you for wanting to contribute to XBar.

# Building from source

## Requirements
- Windows 10 64bit
    > you can use 32bit installation but some steps will be different, so... adapt!

- [git](https://git-scm.com)
- [Visual studio build tools 2019](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2019)
- [vcpkg](https://github.com/microsoft/vcpkg)
- [CMake](https:cmake.org) - `3.19.4` Minimum
- [NSIS](https://nsis.sourceforge.io/Download)
- (optional) [LLVM](https://releases.llvm.org/download.html) contains `clang-format` which the `C/C++ vscode plugin` uses to format the code.

> CMake and vcpkg must be included in the [PATH](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/).

**VS Code extensions**
1. C/C++ (Required)
2. CMake (Required)
3. CMake Tools (Required)
5. cmake-format (Optional)
4. EditorConfig for VS Code (Required)
6. Bracket Pair Colorizer 2 (Optional)
> They should be recommended to you after opening the project folder in VS Code.

## Prepare
1. Clone the repo
    ```powershell
    git clone https://github.com/amrbashir/XBar
    cd XBar
    ```
2. Install the packages
    ```powershell
    vcpkg install --triplet x64-windows fmt nlohmann-json
    ```
    and if you want to build/degbu the 32bit installer or the 32bit exe

    ```powershell
    vcpkg install --triplet x86-windows fmt nlohmann-json
    ```
3. Open `.vscode/c_cpp_properties.json` file and change `compilerPath` to match the `cl.exe` compiler path on your system.
    > Should be something like this `C:/Program Files (x86)/Microsoft Visual Studio/2019/BuildTools/VC/Tools/MSVC/14.28.29333/bin/Hostx64/x64/cl.exe`

4. Open `.vscode/settings.json` file and in the `CMAKE_TOOLCHAIN_FILE` field, change `[vcpkg root]` with the path where you installed vcpkg.

## Debug and Build
1. Open command palette (<kbd>ctrl</kbd>+<kbd>shift</kbd>+<kbd>p</kbd> or <kbd>F1</kbd>) and select `CMake: Select a Kit` then select `Visual Studio Build Tools 2019 Release amd64` or if you want to build/debug 32bit exe, select `Visual Studio Build Tools 2019 Release amd64_x86`.
    > Make sure to clean the cache after Selecting a different Kit by opening the command palette and selecting `CMake: Delete Cache and Reconfigure`

2. Open command palette and select `CMake: Select Variant` then select what ever you want.
3. Open command palette and select `CMake: Configure` and wait for it to finish.
4. Open command palette and select `CMake: Set Build Target` then select `XBar`.
5. Open command palette and select `CMake: Debug` or `CMake: Run Without Debugging` or `CMake: Build`.
    > If you selected `CMake: Build`, You will find the exe in either `.\build\Debug` or `.\build\Release` depending on the variant you selected.

## Build the installer
1. Open command palette and select `CMake: Select a Kit` then select `Visual Studio Build Tools 2019 Release amd64` or if you want to build 32bit installer, select `Visual Studio Build Tools 2019 Release amd64_x86`.
    > Make sure to clean the cache after Selecting a different Kit by opening the command palette and selecting `CMake: Delete Cache and Reconfigure`

2. Open command palette and select `CMake: Select Variant` then select `Release`.
3. Open command palette and select `CMake: Configure` and wait for it to finish.
4. Open command palette and select `CMake: Build` and wait for it to finish.
5. Open a terminal in the project root directory.
6. run this command:
    ```powershell
    cmake --build .\build\ --config Release --target PACKAGE
    ```
    > You will find the installer in `.\build` .

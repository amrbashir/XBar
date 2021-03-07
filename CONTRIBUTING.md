# Contributing
Thank you for wanting to contribute to XBar.

# Building from source

## Requirements :
- [git](https://git-scm.com)
- [Visual studio 2019](https://visualstudio.microsoft.com/vs/community/) with the following:
    - Workloads:
        - Desktop development with C++
    - Individual Components:
        - Any of the VC++ 2019 toolsets (latest prefered)
        - Windows 10 SDK (10.0.18362.0) or if you get some errors install Windows 10 SDK (10.0.19041.0)
        - .NET Framework 4.6.2 SDK
        - .NET Framework 4.6.2 targeting pack
- [vcpkg](https://github.com/microsoft/vcpkg)
- [CMake](https:CMcmake.org)
- (optional) [LLVM](https://releases.llvm.org/download.html) contains `clang-format` which the `C/C++ vscode plugin` uses to format the code.

>CMake and vcpkg must be included in the [PATH](https://www.architectryan.com/2018/03/17/add-to-the-path-on-windows-10/).

> I personally like to use VS Code as my IDE but you can still use Visual studio.
>
> If you chose to use VS Code, open the extensions tab and install the recommended extensions which are:
>
> 1. C/C++
> 2. CMake
> 3. CMake Tools
> 4. EditorConfig for VS Code
> 5. cmake-format
> 6. Bracket Pair Colorizer 2 (optional)
## Prepare :
1. Clone the repo
    ```
    git clone https://github.com/amrbashir/XBar
    cd XBar
    ```
2. Install the packages
    ```
    vcpkg install --triplet x64-windows fmt inih
    ```
3. If you plan to use VS Code:
    1. Open `.vscode/c_cpp_properties.json` file and change `compilerPath` to match the `cl.exe` compiler path on your system.

        or open VS Code command pallette (<kbd>ctrl</kbd>+<kbd>shift</kbd>+<kbd>p</kbd> or <kbd>F1</kbd>) and select `C/C++: Edit Configurations (UI)` and select `cl.exe` compiler from the drop down under `Compilr Path` options.
    2. Open `.vscode/settings.json` file and change `[vcpkg root]` with the path where you installed vcpkg.

## Build :
**For VS Code**:

1. Open command palette (<kbd>ctrl</kbd>+<kbd>shift</kbd>+<kbd>p</kbd> or <kbd>F1</kbd>) and select `CMake: Select a Kit` then select `Visual Studio Community 2019 Release - x86_amd64`.
2. Open command palette and select `CMake: Configure` and wait for it to finish.
3. Open command palette and select `CMake: Set Build Target` then select `XBar`.
4. Open command palette and select `CMake: Debug` , `CMake: Run Without Debugging` or `CMake: Build`.

**For Visual Studio**
1. Open a terminal in the project folder and run:
    ```
    cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
    ```
    >**Note**: change [vcpkg root] with the path where you installed vcpkg

2. Open the `XBar.sln` file located inside the `build` folder.
3. After Visual Studio is launched, Righ click XBar from the solution explorer and select `Set as Startup Project` then press `F5`.
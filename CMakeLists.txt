cmake_minimum_required(VERSION 3.0.0)

project(
  XBar
  DESCRIPTION "A tool to customize windows taskbar appearance"
  VERSION 0.1.0)

# copy the XBar.ico to the build directory for release and debug
configure_file(${CMAKE_SOURCE_DIR}/src/assets/XBar.ico
               ${CMAKE_BINARY_DIR}/Release/assets/XBar.ico COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/assets/XBar.ico
               ${CMAKE_BINARY_DIR}/Debug/assets/XBar.ico COPYONLY)

# copy the Defaults.toml file to the build directory for release and debug
configure_file(${CMAKE_SOURCE_DIR}/src/assets/Defaults.toml
               ${CMAKE_BINARY_DIR}/Release/assets/Defaults.toml COPYONLY)
configure_file(${CMAKE_SOURCE_DIR}/src/assets/Defaults.toml
               ${CMAKE_BINARY_DIR}/Debug/assets/Defaults.toml COPYONLY)

file(GLOB_RECURSE sourceFiles "src/*.cpp")
add_executable(XBar WIN32 ${sourceFiles})
target_compile_features(XBar PUBLIC cxx_std_20)

# fmt package
find_package(fmt CONFIG REQUIRED)
target_link_libraries(XBar PRIVATE fmt::fmt)

# toml++ package
find_package(tomlplusplus CONFIG REQUIRED)
target_link_libraries(XBar PRIVATE tomlplusplus::tomlplusplus)

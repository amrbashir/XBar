cmake_minimum_required(VERSION 3.19.4)

project(
  XBar
  DESCRIPTION
    "A tool to change windows 10 taskbar appearance with per-app rules"
  VERSION 0.1.2)

# copy the assets folder to Release/Debug builds folder
file(COPY "${CMAKE_SOURCE_DIR}/src/assets"
     DESTINATION "${CMAKE_BINARY_DIR}/Debug")
file(COPY "${CMAKE_SOURCE_DIR}/src/assets"
     DESTINATION "${CMAKE_BINARY_DIR}/Release")

file(GLOB_RECURSE sourceFiles "src/*.cpp")
file(GLOB_RECURSE resourceFiles "src/*.rc")
add_executable(XBar WIN32 ${sourceFiles} ${resourceFiles})
target_compile_features(XBar PUBLIC cxx_std_20)

# ##############################################################################
# Packages
# ##############################################################################
find_package(fmt CONFIG REQUIRED)
target_link_libraries(XBar PRIVATE fmt::fmt)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(XBar PRIVATE nlohmann_json nlohmann_json::nlohmann_json)
# ##############################################################################
# Installer configuration
# ##############################################################################
install(TARGETS XBar RUNTIME DESTINATION "." CONFIGURATIONS Release)
# copy 3rd party dlls
install(
  FILES "${CMAKE_CURRENT_BINARY_DIR}/Release/fmt.dll"
  CONFIGURATIONS Release
  DESTINATION ".")

# copy assets folder
install(
  DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/Release/assets"
  CONFIGURATIONS Release
  DESTINATION ".")

# set the default install dir to 'XBar' rather than 'XBar <version>'
set(CPACK_PACKAGE_INSTALL_DIRECTORY "XBar")

# prompt user to uninstall before install new version
set(CPACK_NSIS_ENABLE_UNINSTALL_BEFORE_INSTALL ON)

# set the installer icon
set(CPACK_NSIS_MUI_ICON "${CMAKE_CURRENT_SOURCE_DIR}/src/assets/XBar_icon.ico")

# set the app icon in Programs and Features
set(CPACK_NSIS_EXTRA_INSTALL_COMMANDS
    "WriteRegStr HKLM 'Software\\\\Microsoft\\\\Windows\\\\CurrentVersion\\\\Uninstall\\\\XBar' 'DisplayIcon' '$INSTDIR\\\\XBar.exe'"
)

# set installer license
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE.md")

# create a start menu shortcut
set(CPACK_NSIS_CREATE_ICONS_EXTRA
    "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\XBar.lnk' '$INSTDIR\\\\XBar.exe'"
)

# delete the start menu shortcut on uninstall
set(CPACK_NSIS_DELETE_ICONS_EXTRA
    "Delete '$SMPROGRAMS\\\\$START_MENU\\\\XBar.lnk'")

include(CPack)

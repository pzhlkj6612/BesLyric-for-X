cmake_minimum_required(VERSION 3.22.1)  # Ubuntu 22.04

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.cmake")


if (APPLE)
    # > The value of this variable should be set prior to the first project() or enable_language() command invocation because it may influence configuration of the toolchain and flags. It is intended to be set locally by the user creating a build tree. This variable should be set as a CACHE entry (or else CMake may remove it while initializing a cache entry of the same name).
    # CMAKE_OSX_DEPLOYMENT_TARGET — CMake 3.22.6 Documentation
    # https://cmake.org/cmake/help/v3.22/variable/CMAKE_OSX_DEPLOYMENT_TARGET.html
    set(CMAKE_OSX_DEPLOYMENT_TARGET "14.0.0" CACHE STRING "")
endif ()


project(BesLyric-for-X LANGUAGES CXX)


set(APP_VERSION "3.2.0" CACHE STRING "App version number")
set(GIT_COMMIT_SHA1 "0000000000000000000000000000000000000000" CACHE STRING "Git commit SHA-1 hash")
option(DISABLE_QT_DEBUG_OUTPUT "Disable qDebug()" OFF)

add_compile_definitions(
    APP_VERSION=\"${APP_VERSION}\"
    GIT_COMMIT_SHA1=\"${GIT_COMMIT_SHA1}\"
    QT_DEPRECATED_WARNINGS
)

if (DISABLE_QT_DEBUG_OUTPUT)
    add_compile_definitions(QT_NO_DEBUG_OUTPUT)
endif ()


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Getting started with CMake | Build with CMake | Qt 6
#   https://doc.qt.io/qt-6/cmake-get-started.html
find_package(Qt6 6.5.3 REQUIRED COMPONENTS
    Core
    Gui
    Network
    Widgets
)
qt_standard_project_setup()
set(CMAKE_AUTORCC ON)
add_compile_definitions(QT_DISABLE_DEPRECATED_BEFORE=0x060503)

find_package(FFMPEG 4 REQUIRED COMPONENTS
    avcodec
    avformat
    avutil
    swresample
)

find_package(SDL2 REQUIRED)


file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/StackFrame.cpp

    ${PROJECT_SOURCE_DIR}/BesWidgets/*.cpp
    ${PROJECT_SOURCE_DIR}/BottomWidgets/*.cpp
    ${PROJECT_SOURCE_DIR}/Entities/*.cpp
    ${PROJECT_SOURCE_DIR}/MiddleWidgets/*.cpp
    ${PROJECT_SOURCE_DIR}/StackFrame/*.cpp
    ${PROJECT_SOURCE_DIR}/TopWidgets/*.cpp
    ${PROJECT_SOURCE_DIR}/Utility/*.cpp
)

file(GLOB_RECURSE HEADERS
    ${PROJECT_SOURCE_DIR}/global.h
    ${PROJECT_SOURCE_DIR}/MyApplication.h
    ${PROJECT_SOURCE_DIR}/StackFrame.h

    ${PROJECT_SOURCE_DIR}/BesWidgets/*.h
    ${PROJECT_SOURCE_DIR}/BottomWidgets/*.h
    ${PROJECT_SOURCE_DIR}/Define/*.h
    ${PROJECT_SOURCE_DIR}/Entities/*.h
    ${PROJECT_SOURCE_DIR}/MiddleWidgets/*.h
    ${PROJECT_SOURCE_DIR}/StackFrame/*.h
    ${PROJECT_SOURCE_DIR}/TopWidgets/*.h
    ${PROJECT_SOURCE_DIR}/Utility/*.h

    ${PROJECT_SOURCE_DIR}/BesWidgets/table/BesLListViewStyle.hpp
    ${PROJECT_SOURCE_DIR}/Entities/MP3Editor/AlbumImageHelper.hpp
)


# Information for built binary file


if (APPLE)
    # Setting the Application Icon | Qt 5
    # https://doc.qt.io/qt-5/appicon.html

    # NOTE: Don't include the path in MACOSX_BUNDLE_ICON_FILE -- this is
    # the property added to Info.plist
    set(MACOSX_BUNDLE_ICON_FILE BesLyric.icns)

    # And this part tells CMake where to find and install the file itself
    set(APP_ICON_MACOSX ${PROJECT_SOURCE_DIR}/BesLyric.icns)
    set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES MACOSX_PACKAGE_LOCATION "Resources")
endif ()

if (WIN32)
    set(ICON_FILENAME "Beslyric.ico")
    set(VER_FILEVERSION "4,5,6")
    set(VER_PRODUCTVERSION "6,7,8")
    set(VER_COMPANYNAME_STR "BesStudio")
    set(VER_FILEDESCRIPTION_STR "BesLyric-for-X is a lyric (.lrc) maker in the style of NetEase Cloud Music")
    set(VER_FILEVERSION_STR "4.5.6")
    set(VER_LEGALCOPYRIGHT_STR "copyleft (C) GPL")
    set(VER_ORIGINALFILENAME_STR "${PROJECT_NAME}.exe")
    set(VER_PRODUCTNAME_STR "BesLyric")
    set(VER_PRODUCTVERSION_STR "6.7.8")

    set(APP_RC_FILE ${CMAKE_CURRENT_BINARY_DIR}/BesLyric-for-X.rc)
    configure_file(${PROJECT_SOURCE_DIR}/BesLyric-for-X.rc.in ${APP_RC_FILE} @ONLY)
endif ()


# > Indeed, you need to mention all components (headers, source files, resources, forms) that belong to your target in the add_executable call. Otherwise, how should moc, uic, rcc know?
# https://stackoverflow.com/questions/49027620/forced-to-use-qt5-wrap-cpp-macro-instead-of-automoc-property-in-cmake
qt_add_executable(${PROJECT_NAME} MACOSX_BUNDLE
    ${APP_RC_FILE}
    ${APP_ICON_MACOSX}
    ${PROJECT_SOURCE_DIR}/resource.qrc
    ${SOURCES}
    ${HEADERS}
)


target_include_directories(${PROJECT_NAME} PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/BesWidgets
    ${PROJECT_SOURCE_DIR}/BottomWidgets
    ${PROJECT_SOURCE_DIR}/Define
    ${PROJECT_SOURCE_DIR}/Entities
    ${PROJECT_SOURCE_DIR}/Entities/MP3Editor
    ${PROJECT_SOURCE_DIR}/MiddleWidgets
    ${PROJECT_SOURCE_DIR}/StackFrame
    ${PROJECT_SOURCE_DIR}/TopWidgets
    ${PROJECT_SOURCE_DIR}/Utility

    ${SDL2_INCLUDE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::Network

    FFMPEG::avcodec
    FFMPEG::avformat
    FFMPEG::avutil
    FFMPEG::swresample

    ${SDL2_LIBRARY}
)

if (APPLE)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        # MACOSX_BUNDLE_INFO_PLIST — CMake 3.22.6 Documentation
        # https://cmake.org/cmake/help/v3.22/prop_tgt/MACOSX_BUNDLE_INFO_PLIST.html

        # CFBundleName.
        MACOSX_BUNDLE_BUNDLE_NAME "${PROJECT_NAME}"

        # CFBundleVersion.
        MACOSX_BUNDLE_BUNDLE_VERSION "1.2.3"

        # NSHumanReadableCopyright.
        MACOSX_BUNDLE_COPYRIGHT "copyleft (C) GPL"

        # CFBundleGetInfoString
        MACOSX_BUNDLE_INFO_STRING "info string"

        # CFBundleLongVersionString.
        MACOSX_BUNDLE_LONG_VERSION_STRING "4.5.6"

        # CFBundleShortVersionString.
        MACOSX_BUNDLE_SHORT_VERSION_STRING "7.8.9"
    )
endif ()

#--------------------------------

# For create-dmg.
if (APPLE)
    install(FILES
        ${PROJECT_SOURCE_DIR}/BesLyric.icns
        DESTINATION .
    )
endif ()


# For Inno Setup.
if (WIN32)
    install(FILES
        ${PROJECT_SOURCE_DIR}/Beslyric.ico
        ${PROJECT_SOURCE_DIR}/version.txt
        DESTINATION .
    )
endif ()


# For target
if (UNIX AND (NOT APPLE))
    # For AppImage, install files into "AppDir"
    install(TARGETS ${PROJECT_NAME} DESTINATION ./usr/bin)
else ()
    install(TARGETS ${PROJECT_NAME} DESTINATION .)
endif ()


# For PDB
if (MSVC)
    install(FILES $<TARGET_PDB_FILE:${PROJECT_NAME}> DESTINATION .)
endif ()


# For dSYM
# from: Separate debug information (#20256) · Issues · CMake / CMake · GitLab
#       https://gitlab.kitware.com/cmake/cmake/-/issues/20256
if (APPLE)
    find_program(DSYMUTIL_PROGRAM dsymutil)
    if (NOT DSYMUTIL_PROGRAM)
        message(FATAL_ERROR "dsymutil not found")
    endif ()
    foreach (lang C CXX)
        foreach (var LINK_EXECUTABLE CREATE_SHARED_LIBRARY)
            list(APPEND CMAKE_${lang}_${var} "${DSYMUTIL_PROGRAM} '${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}.app/Contents/MacOS/${PROJECT_NAME}'")
        endforeach ()
    endforeach ()
endif ()


# For AppImage, install files into "AppDir"
if (UNIX AND (NOT APPLE))
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/BesLyric-for-X.AppDir/usr/share
        DESTINATION ./usr
    )
endif ()

#--------------------------------

cmake_minimum_required(VERSION 3.10.2)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/.cmake")


project(BesLyric-for-X LANGUAGES CXX)


add_compile_definitions(
        APP_VERSION="9.8.7"
        GIT_COMMIT_SHA1="0000011111222223333344444555550000000000"
)


set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 5.12.4 REQUIRED COMPONENTS
        Core
        Gui
        Network
        Widgets
        )

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

        ${PROJECT_SOURCE_DIR}/BesWidgets/table/BesLListViewStyle.hpp
        ${PROJECT_SOURCE_DIR}/Entities/MP3Editor/AlbumImageHelper.hpp

        ${PROJECT_SOURCE_DIR}/BesWidgets/*.h
        ${PROJECT_SOURCE_DIR}/BottomWidgets/*.h
        ${PROJECT_SOURCE_DIR}/Define/*.h
        ${PROJECT_SOURCE_DIR}/Entities/*.h
        ${PROJECT_SOURCE_DIR}/MiddleWidgets/*.h
        ${PROJECT_SOURCE_DIR}/StackFrame/*.h
        ${PROJECT_SOURCE_DIR}/TopWidgets/*.h
        ${PROJECT_SOURCE_DIR}/Utility/*.h
        )

qt5_wrap_cpp(HEADERS_MOC ${HEADERS})


# Setting the Application Icon | Qt 5
# https://doc.qt.io/qt-5/appicon.html

# NOTE: Don't include the path in MACOSX_BUNDLE_ICON_FILE -- this is
# the property added to Info.plist
set(MACOSX_BUNDLE_ICON_FILE BesLyric.icns)

# And this part tells CMake where to find and install the file itself
set(APP_ICON_MACOSX ${PROJECT_SOURCE_DIR}/BesLyric.icns)
set_source_files_properties(${APP_ICON_MACOSX} PROPERTIES
        MACOSX_PACKAGE_LOCATION "Resources")


add_executable(${PROJECT_NAME} MACOSX_BUNDLE
        ${SOURCES}
        ${HEADERS_MOC}
        ${PROJECT_SOURCE_DIR}/resource.qrc
        ${PROJECT_SOURCE_DIR}/BesLyric.rc
        ${APP_ICON_MACOSX}
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

#--------------------------------

# "Install" required files to specific directory via "make install" after "cmake -DCMAKE_INSTALL_PREFIX"


# For create-dmg.
if (APPLE)
    install(FILES
            ${PROJECT_SOURCE_DIR}/BesLyric.icns
            DESTINATION ./
            )
endif ()


# For Inno Setup.
if (WIN32)
    install(FILES
            ${PROJECT_SOURCE_DIR}/Beslyric.ico
            ${PROJECT_SOURCE_DIR}/version.txt
            DESTINATION ./
            )
endif ()


# For target
if (UNIX AND (NOT APPLE))
    # For AppImage.
    #   INSTALL_ROOT=AppDir
    install(TARGETS ${PROJECT_NAME} DESTINATION ./usr/bin)
else ()
    install(TARGETS ${PROJECT_NAME} DESTINATION ./)
endif ()


# For AppImage.
#   INSTALL_ROOT=AppDir
if (UNIX AND (NOT APPLE))
    install(DIRECTORY ${PROJECT_SOURCE_DIR}/BesLyric-for-X.AppDir/usr/share
            DESTINATION ./usr/
            )
endif ()

#--------------------------------

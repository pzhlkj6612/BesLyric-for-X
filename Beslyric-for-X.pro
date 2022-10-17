#-------------------------------------------------
#
# Project created by QtCreator 2018-07-05T22:26:31
#
#-------------------------------------------------

QT       += core gui network widgets


# To use GCC 11.2.0+ in MinGW, we need Qt 6.2.2+.
#   https://www.qt.io/blog/qt-6.2.2-released

is_qt_version_too_low = false

lessThan(QT_MAJOR_VERSION, 6) {
    is_qt_version_too_low = true
}
else:equals(QT_MAJOR_VERSION, 6) {
    lessThan(QT_MINOR_VERSION, 2) {
        is_qt_version_too_low = true
    }
    else:equals(QT_MINOR_VERSION, 2):lessThan(QT_PATCH_VERSION, 2) {
        is_qt_version_too_low = true
    }
}

equals(is_qt_version_too_low, true) {
    error("Qt version >= 6.2.2 is required, you are using $$[QT_VERSION].")
}

# Don't use any deprecated API.
DEFINES *= QT_DISABLE_DEPRECATED_BEFORE=0x060202


TARGET = BesLyric-for-X
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# To disable qDebug(), pass "-before DEFINES*=RELEASE_VERSION" to qmake
contains(DEFINES, "RELEASE_VERSION") {
    message("DEFINES \"RELEASE_VERSION\" detected.")
    DEFINES *= QT_NO_DEBUG_OUTPUT
}

INCLUDEPATH +=$$PWD BesWidgets
include(BesWidgets/BesWidgets.pri)

INCLUDEPATH +=$$PWD StackFrame
include(StackFrame/StackFrame.pri)

INCLUDEPATH +=$$PWD TopWidgets
include(TopWidgets/TopWidgets.pri)

INCLUDEPATH +=$$PWD MiddleWidgets
include(MiddleWidgets/MiddleWidgets.pri)

INCLUDEPATH +=$$PWD BottomWidgets
include(BottomWidgets/BottomWidgets.pri)

INCLUDEPATH +=$$PWD Entities
include(Entities/Entities.pri)

INCLUDEPATH +=$$PWD Utility
include(Utility/Utility.pri)


SOURCES += main.cpp \
    StackFrame.cpp

HEADERS  += \
    Define/Define.h\
    Define/Enum.h\
    Define/Struct.h\
    Define/Static.h\
    global.h \
    StackFrame.h \
    MyApplication.h

OTHER_FILES += \
    LICENSE \
    README.md \
    version.txt

RESOURCES += \
    resource.qrc


# Git commit SHA1.
isEmpty(GIT_COMMIT_SHA1) {
    warning("No \"GIT_COMMIT_SHA1\" in qmake arguments. Try to find in environment.")
    GIT_COMMIT_SHA1 = $$getenv(GIT_COMMIT_SHA1)
}
isEmpty(GIT_COMMIT_SHA1) {
    warning("No \"GIT_COMMIT_SHA1\" in environment. Try to get it by executing the command 'git -C \"$${_PRO_FILE_PWD_}\" rev-parse HEAD'.")
    GIT_COMMIT_SHA1 = $$system("git -C \"$${_PRO_FILE_PWD_}\" rev-parse HEAD")
}
!contains(GIT_COMMIT_SHA1, "^[0-9a-f]{40}$") {
    warning("\"$${GIT_COMMIT_SHA1}\" is NOT a valid GIT_COMMIT_SHA1. Use the default one.")
    GIT_COMMIT_SHA1 = "0000000000000000000000000000000000000000"
}
message("GIT_COMMIT_SHA1 = $${GIT_COMMIT_SHA1}")
DEFINES *= "GIT_COMMIT_SHA1=\\\"$${GIT_COMMIT_SHA1}\\\""


# Version number. All in one.
isEmpty(APP_VERSION) {
    warning("No \"APP_VERSION\" in qmake arguments. Try to find in environment.")
    APP_VERSION = $$getenv(APP_VERSION)
}
isEmpty(APP_VERSION) {
    warning("No \"APP_VERSION\" in environment. Try to get it by executing the command 'git -C \"$${_PRO_FILE_PWD_}\" describe --tags --abbrev=0'.")
    APP_VERSION_starts_with_v = $$system("git -C \"$${_PRO_FILE_PWD_}\" describe --tags --abbrev=0")
    APP_VERSION = $$replace(APP_VERSION_starts_with_v, "v", "")
    unset(APP_VERSION_starts_with_v)
}
!contains(APP_VERSION, "^\d+\.\d+\.\d+$") {
    warning("\"$${APP_VERSION}\" is NOT a valid APP_VERSION. Use the default one.")
    APP_VERSION = "3.2.0"
}
message("APP_VERSION = $${APP_VERSION}")

DEFINES *= "APP_VERSION=\\\"$${APP_VERSION}\\\""
VERSION = "$${APP_VERSION}"


# windows icon and exe file infomation
#   https://doc.qt.io/qt-5/qmake-platform-notes.html#adding-windows-resource-files
win32{
    QMAKE_TARGET_COMPANY = "BesStudio"
    QMAKE_TARGET_DESCRIPTION = "BesLyric-for-X is a lyric (.lrc) maker in the style of NetEase Cloud Music"
    QMAKE_TARGET_COPYRIGHT = "copyleft (C) GPL"
    QMAKE_TARGET_PRODUCT = "BesLyric"
    RC_ICONS = Beslyric.ico
    RC_LANG = 0x0804 # zh-CN
}

# set icon under Mac Os
macx{
ICON = BesLyric.icns
}

# ubuntu icon recoginition
#  No test on other Linux distros!
# from: https://stackoverflow.com/questions/45329372/ubuntu-recognizes-executable-as-shared-library-and-wont-run-it-by-clicking
!macx:unix{
    QMAKE_LFLAGS *= -no-pie
}

#--------------------------------

#屏蔽 msvc 编译器对 rational.h 的 warning: C4819: 该文件包含不能在当前代码页(936)中表示的字符。请将该文件保存为 Unicode 格式以防止数据丢失
win32-msvc*:QMAKE_CXXFLAGS += /wd"4819"

#--------------------------------

# Third-party libraries

win32 {
    isEmpty(B4X_DEP_PATH) {
        B4X_DEP_PATH = $$getenv(B4X_DEP_PATH)
    }
    isEmpty(B4X_DEP_PATH) {
        error("\"B4X_DEP_PATH\" is NOT set.")
    }
    contains(B4X_DEP_PATH, "^\s*$") {
        error("\"B4X_DEP_PATH\" is empty.")
    }
    message("B4X_DEP_PATH = $${B4X_DEP_PATH}")

    dep_base_path = $$system_path($$absolute_path($$clean_path($${B4X_DEP_PATH})))
    !exists($${dep_base_path}) {
        error("\"$${dep_base_path}\" does NOT exist.")
    }
    message("dep_base_path = $${dep_base_path}")

    dep_include_path = $$system_path($${dep_base_path}/include)
    !exists($${dep_include_path}) {
        error("\"$${dep_include_path}\" does NOT exist.")
    }
    message("dep_include_path = $${dep_include_path}")

    dep_lib_path = $$system_path($${dep_base_path}/lib)
    !exists($${dep_lib_path}) {
        error("\"$${dep_lib_path}\" does NOT exist.")
    }
    message("dep_lib_path = $${dep_lib_path}")

    INCLUDEPATH *= \
        $${dep_include_path} \
        $$system_path($${dep_include_path}/SDL2)
    LIBS *= \
        -L$${dep_lib_path} \
        -lavcodec \
        -lavdevice \
        -lavfilter \
        -lavformat \
        -lavutil \
        -lpostproc \
        -lswresample \
        -lswscale \
        -lSDL2

    unset(dep_lib_path)
    unset(dep_include_path)
    unset(dep_base_path)
    unset(B4X_DEP_PATH)
}

unix {
    CONFIG *= link_pkgconfig
    PKGCONFIG *= \
        libavcodec \
        libavdevice \
        libavfilter \
        libavformat \
        libavutil \
        libpostproc \
        libswresample \
        libswscale \
        sdl2
}

#--------------------------------

# "Install" required files to specific directory via "make install INSTALL_ROOT='somewhere'"


# For create-dmg.
macx {
    icon_icns.files = "$${_PRO_FILE_PWD_}/BesLyric.icns"
    icon_icns.path = "/"

    INSTALLS *= icon_icns
}


# For Inno Setup.
win32 {
    icon_ico.files = "$${_PRO_FILE_PWD_}/Beslyric.ico"
    icon_ico.path = "/"

    INSTALLS *= icon_ico


    version_txt.files = "$${_PRO_FILE_PWD_}/version.txt"
    version_txt.path = "/"

    INSTALLS *= version_txt
}


# For target, both binary and debugging symbol go to target.path .
#   See also QTBUG-81354 for MinGW.
unix:!macx {
    # For AppImage.
    #   INSTALL_ROOT=AppDir

    target.path = "/usr/bin"
} else {
    target.path = "/"
}

INSTALLS *= target


# For AppImage.
#   INSTALL_ROOT=AppDir
unix:!macx {
    appdir_desktop.path = "/usr/share/applications"
    appdir_desktop.files = "$${_PRO_FILE_PWD_}/BesLyric-for-X.AppDir$${appdir_desktop.path}/BesLyric-for-X.desktop"

    INSTALLS *= appdir_desktop


    icon_sizes = \
        "16x16" \
        "24x24" \
        "32x32" \
        "32x32@2" \
        "48x48" \
        "64x64" \
        "64x64@2" \
        "72x72" \
        "96x96" \
        "128x128" \
        "256x256" \
        "256x256@2" \

    for(icon_size, icon_sizes) {
        appdir_icon_$${icon_size}.path = "/usr/share/icons/hicolor/$${icon_size}/apps"
        appdir_icon_$${icon_size}.files = "$${_PRO_FILE_PWD_}/BesLyric-for-X.AppDir$$eval(appdir_icon_$${icon_size}.path)/BesLyric-for-X.png"

        INSTALLS *= appdir_icon_$${icon_size}
    }
}

#--------------------------------

#!/bin/bash


set -exu


# set env:
#   - PACKAGE_APPIMAGE_FILE_PATH


echo "Linux script for $VERSION_NUMBER + $GIT_COMMIT (RELEASE? $RELEASE_BUILD)"


echo "Check $GITHUB_ENV"


repo_dir="$PWD"


sudo add-apt-repository -y ppa:savoury1/ffmpeg4
sudo apt-get update
sudo apt install --no-install-recommends -y \
    libavcodec-dev \
    libavformat-dev \
    libavutil-dev \
    libswresample-dev \
    ffmpeg \
    libsdl2-dev \
    libfuse2  # AppImage: dlopen(): error loading libfuse.so.2

pushd "$(mktemp -d)"

    # GLIBC 2.35 supported: https://github.com/probonopd/linuxdeployqt/commit/5b437841c7cb255b4ead3a91090ca6d7154742a5
    wget 'https://github.com/probonopd/linuxdeployqt/releases/download/continuous/linuxdeployqt-continuous-x86_64.AppImage' \
        -O 'linuxdeployqt'
    chmod u+x 'linuxdeployqt'

    export PATH="$(realpath .):$PATH"

popd


build_dir="$(mktemp -d)"
install_dir="$(mktemp -d)"

cmake \
    -D 'CMAKE_BUILD_TYPE=RelWithDebInfo' \
    -D "CMAKE_INSTALL_PREFIX=$install_dir" \
    -D "B4X_GIT_COMMIT_SHA1=$GIT_COMMIT" \
    -D "B4X_APP_VERSION=$VERSION_NUMBER" \
    -D "B4X_RELEASE_BUILD=$RELEASE_BUILD" \
    -S "$repo_dir" \
    -B "$build_dir"

cmake \
    --build "$build_dir" \
    -j $(nproc) \
    --target install

du -ach "$install_dir"


pushd "$(mktemp -d)"

    DESKTOP_FILE_PATH="$install_dir/usr/share/applications/BesLyric-for-X.desktop" \
    LINUXDEPLOYQT_PATH='linuxdeployqt' \
    QMAKE_PATH="$(which qmake)" \
    VERSION="$VERSION_NUMBER" \
        bash "$repo_dir/.deploy-package/Linux/call_linuxdeployqt.sh"

    appimage_path="BesLyric-for-X-$VERSION_NUMBER-Linux-x86_64.AppImage"
    mv "BesLyric-for-X-$VERSION_NUMBER-x86_64.AppImage" "$appimage_path"
    stat "$appimage_path"
    echo "PACKAGE_APPIMAGE_FILE_PATH=$PWD/$appimage_path" >> "$GITHUB_ENV"

popd

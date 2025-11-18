#!/bin/bash


set -exu


# set env:
#   - PACKAGE_DMG_FILE_PATH


echo "macOS script for $VERSION_NUMBER + $GIT_COMMIT (RELEASE? $RELEASE_BUILD)"


echo "Check $GITHUB_ENV"


repo_dir="$PWD"


brew install sdl2 ffmpeg@4 imagemagick create-dmg

sdl2_path_link="$(brew --prefix --installed sdl2)"
sdl2_path="$(dirname ${sdl2_path_link})/$(readlink ${sdl2_path_link})"

ffmpeg_path_link="$(brew --prefix --installed ffmpeg@4)"
ffmpeg_path="$(dirname ${ffmpeg_path_link})/$(readlink ${ffmpeg_path_link})"


build_dir="$(mktemp -d)"
install_dir="$(mktemp -d)"

cmake \
    -D 'CMAKE_BUILD_TYPE=RelWithDebInfo' \
    -D "CMAKE_PREFIX_PATH=$ffmpeg_path;$sdl2_path" \
    -D "CMAKE_INSTALL_PREFIX=$install_dir" \
    -D "B4X_GIT_COMMIT_SHA1=$GIT_COMMIT" \
    -D "B4X_APP_VERSION=$VERSION_NUMBER" \
    -D "B4X_RELEASE_BUILD=$RELEASE_BUILD" \
    -S "$repo_dir" \
    -B "$build_dir"

# nproc is not a command on Mac · Issue #33 · memkind/memkind
#   https://github.com/memkind/memkind/issues/33#issuecomment-540614162
cmake \
    --build "$build_dir" \
    -j $(sysctl -n hw.logicalcpu) \
    --target install

echo '--------------build dir---------------'

find "$build_dir"

echo '------------install dir-------------'

du -ach "$install_dir"



MACDEPLOYQT_PATH='macdeployqt' \
    APP_FILE_PATH="$install_dir/BesLyric-for-X.app" \
    bash "$repo_dir/.deploy-package/macOS/macdeployqt_enhanced.sh"

pushd "$(mktemp -d)"

    dmg_path="BesLyric-for-X-$VERSION_NUMBER-macOS.dmg"

    IMAGEMAGICK_PATH='magick' \
    CREATE_DMG_PATH='create-dmg' \
    APP_FILE_PATH="$install_dir/BesLyric-for-X.app" \
    OUTPUT_DMG_PATH="./$dmg_path" \
        bash "$repo_dir/.deploy-package/macOS/package_cdmg.sh"

    echo "PACKAGE_DMG_FILE_PATH=$PWD/$dmg_path" >> "$GITHUB_ENV"

popd

#/bin/bash -x

clear && clear

if [ "$1" = "clean" ]; then
  rm -rf build/
  (
    cd src/Engine/Dependencies/LuaJIT
    make clean
  )
fi

build_folder="."
project_name="NumberGame"
top_level_dir=$PWD
asset_command="ln -sf"
asset_directory="Engine"
asset_folders=("Fonts" "Shaders" "Images" "LuaScripts" "Levels")
use_clang_tidy="OFF"
if [[ "$*" == *"clang-tidy"* ]]; then
  use_clang_tidy="ON"
fi

mk_dir() {
  rm -rf $build_folder/$project_name/Assets

  for folder in ${asset_folders[@]}; do
    mkdir -p $build_folder/$project_name/Assets/$folder
  done
}

create_asset_directory() {
  for folder in ${asset_folders[@]}; do
    $asset_command $top_level_dir/src/$asset_directory/Assets/$folder/* $build_folder/$project_name/Assets/$folder 2>/dev/null || :
  done
}

create_asset() {
  mk_dir
  asset_directory="Engine"
  create_asset_directory

  asset_directory=$project_name
  create_asset_directory
}

create_asset_ln() {
  asset_command="ln -sf"
  create_asset
}

copy_asset() {
  asset_command="cp -rf"
  create_asset
}

(
  git submodule update --init --recursive
  (
    cd src/Engine/Dependencies/LuaJIT
    make -j$(($(nproc) + 1))
  )
  mkdir build

  cd build
  unameOut="$(uname -s)"
  case "${unameOut}" in
  Linux*)
    if [ "$1" = "release" ] || [ "$1" = "Release" ] || [ "$2" = "release" ] || [ "$2" = "Release" ]; then
      mkdir RelWithDebInfo
      build_folder="RelWithDebInfo"
      for dir in $top_level_dir/src/*/; do
        project_name=$(basename "$dir")
        if [ "$project_name" != "Engine" ]; then
          copy_asset
        fi
      done
      (
        cd RelWithDebInfo
        cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DUSE_CLANG_TIDY=$use_clang_tidy -G Ninja ../..
        ninja
      )
      project_name="UnitTests"
      copy_asset
    else
      mkdir Debug
      build_folder="Debug"
      for dir in $top_level_dir/src/*/; do
        project_name=$(basename "$dir")
        if [ "$project_name" != "Engine" ]; then
          create_asset_ln
        fi
      done
      (
        cd Debug
        cmake -DCMAKE_BUILD_TYPE=Debug -DUSE_CLANG_TIDY=$use_clang_tidy -G Ninja ../..
        ninja
      )
      project_name="UnitTests"
      create_asset_ln
    fi
    ;;
  Darwin*)
    if [ "$1" = "release" ] || [ "$1" = "Release" ] || [ "$2" = "release" ] || [ "$2" = "Release" ]; then
      mkdir RelWithDebInfo
      build_folder="RelWithDebInfo"
      for dir in $top_level_dir/src/*/; do
        project_name=$(basename "$dir")
        if [ "$project_name" != "Engine" ]; then
          copy_asset
        fi
      done
      project_name="UnitTests"
      copy_asset
      cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -DUSE_CLANG_TIDY=$use_clang_tidy -G Xcode ..
    else
      mkdir Debug
      build_folder="Debug"
      for dir in $top_level_dir/src/*/; do
        project_name=$(basename "$dir")
        if [ "$project_name" != "Engine" ]; then
          create_asset_ln
        fi
      done
      project_name="UnitTests"
      create_asset_ln
      cmake -DCMAKE_BUILD_TYPE=Debug -DUSE_CLANG_TIDY=$use_clang_tidy -G Xcode ..
    fi
    xcodebuild -scheme TempEngine build
    xcodebuild -scheme NumberGame build
    ;;
  CYGWIN*)
    cmake ..
    ;;
  MINGW*)
    cmake ..
    ;;
  *) cmake .. ;;
  esac
)

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

mk_dir() {
  rm -rf $build_folder/Assets/Fonts

  mkdir -p $build_folder/$project_name/Assets/Fonts
  mkdir -p $build_folder/$project_name/Assets/Shaders
  mkdir -p $build_folder/$project_name/Assets/Images
  mkdir -p $build_folder/$project_name/Assets/LuaScripts
  mkdir -p $build_folder/$project_name/Assets/Levels
}

create_asset_ln() {
  mk_dir
  ln -sf $top_level_dir/src/Engine/Assets/Fonts/* $build_folder/$project_name/Assets/Fonts
  ln -sf $top_level_dir/src/Engine/Assets/Shaders/* $build_folder/$project_name/Assets/Shaders
  ln -sf $top_level_dir/src/Engine/Assets/Images/* $build_folder/$project_name/Assets/Images
  ln -sf $top_level_dir/src/Engine/Assets/LuaScripts/* $build_folder/$project_name/Assets/LuaScripts
  ln -sf $top_level_dir/src/Engine/Assets/Levels/* $build_folder/$project_name/Assets/Levels

  ln -sf $top_level_dir/src/$project_name/Assets/Fonts/* $build_folder/$project_name/Assets/Fonts
  ln -sf $top_level_dir/src/$project_name/Assets/Shaders/* $build_folder/$project_name/Assets/Shaders
  ln -sf $top_level_dir/src/$project_name/Assets/Images/* $build_folder/$project_name/Assets/Images
  ln -sf $top_level_dir/src/$project_name/Assets/LuaScripts/* $build_folder/$project_name/Assets/LuaScripts
  ln -sf $top_level_dir/src/$project_name/Assets/Levels/* $build_folder/$project_name/Assets/Levels
}

copy_asset() {
  mk_dir
  cp -rf $top_level_dir/src/Engine/Assets/Fonts/* $build_folder/$project_name/Assets/Fonts
  cp -rf $top_level_dir/src/Engine/Assets/Shaders/* $build_folder/$project_name/Assets/Shaders
  cp -rf $top_level_dir/src/Engine/Assets/Images/* $build_folder/$project_name/Assets/Images
  cp -rf $top_level_dir/src/Engine/Assets/LuaScripts/* $build_folder/$project_name/Assets/LuaScripts
  cp -rf $top_level_dir/src/Engine/Assets/Levels/* $build_folder/$project_name/Assets/Levels

  cp -rf $top_level_dir/src/$project_name/Assets/Fonts/* $build_folder/$project_name/Assets/Fonts
  cp -rf $top_level_dir/src/$project_name/Assets/Render/OpenGL/Shaders/* $build_folder/$project_name/Assets/Shaders
  cp -rf $top_level_dir/src/$project_name/Assets/Images/* $build_folder/$project_name/Assets/Images
  cp -rf $top_level_dir/src/$project_name/Assets/LuaScripts/* $build_folder/$project_name/Assets/LuaScripts
  cp -rf $top_level_dir/src/$project_name/Assets/Levels/* $build_folder/$project_name/Assets/Levels
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
      copy_asset
      (
        cd RelWithDebInfo
        cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -G Ninja ../..
        ninja
      )
      project_name="UnitTests"
      copy_asset
    else
      mkdir Debug
      build_folder="Debug"
      create_asset_ln
      (
        cd Debug
        cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja ../..
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
      copy_asset
      cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -G Xcode ..
    else
      mkdir Debug
      build_folder="Debug"
      create_asset_ln
      cmake -DCMAKE_BUILD_TYPE=Debug -G Xcode ..
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

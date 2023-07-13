#/bin/bash -x

clear && clear

if [[ "$*" == *"clean"* ]]; then
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
asset_folder="."
build_path="$build_folder/$project_name/Assets"
is_copy_asset=true
use_sanitize_thread="OFF"
run_args=""

if [[ "$*" == *"clang-tidy"* ]]; then
  use_clang_tidy="ON"
fi

if [[ "$*" == *"sanitize-thread"* ]]; then
  use_sanitize_thread="ON"
  export TSAN_OPTIONS="suppressions=$PWD/sanitizer-thread-suppressions.supp second_deadlock_stack=1"
elif [[ "$*" == *"mangohud"* ]]; then
  run_args="mangohud --dlsym"
fi

mk_dir() {
  rm -rf $build_path

  for folder in ${asset_folders[@]}; do
    mkdir -p $build_path/$folder
  done
}

create_asset_directory() {
  for folder in ${asset_folders[@]}; do
    $asset_command $top_level_dir/src/$asset_directory/Assets/$folder/* $build_path/$folder 2>/dev/null || :
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

build_project_linux() {
  mkdir $build_folder
  for dir in $top_level_dir/src/*/; do
    project_name=$(basename "$dir")
    build_path="$build_folder/$project_name/Assets"
    if [ "$project_name" != "Engine" ]; then
      if [ "$is_copy_asset" = true ]; then
        copy_asset
      else
        create_asset_ln
      fi
    fi
  done
  (
    cd $build_folder
    cmake -DCMAKE_BUILD_TYPE=$build_folder -DUSE_CLANG_TIDY=$use_clang_tidy -DSANITIZE_THREAD=$use_sanitize_thread -G Ninja ../..
    ninja
  )
}

build_project_mac() {
  for dir in $top_level_dir/src/*/; do
    project_name=$(basename "$dir")
    build_path="$project_name/$build_folder/Assets"
    if [ "$project_name" != "Engine" ]; then
      if [ "$is_copy_asset" = true ]; then
        copy_asset
      else
        create_asset_ln
      fi
    fi
  done
  cmake -DCMAKE_BUILD_TYPE=$build_folder -DUSE_CLANG_TIDY=$use_clang_tidy -DSANITIZE_THREAD=$use_sanitize_thread -G Xcode ..
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
    if [[ "$*" == *"release"* ]] || [[ "$*" == *"Release"* ]]; then
      build_folder="RelWithDebInfo"
      is_copy_asset=true
      build_project_linux
    else
      build_folder="Debug"
      is_copy_asset=false
      build_project_linux
    fi
    ;;
  Darwin*)
    if [[ "$*" == *"release"* ]] || [[ "$*" == *"Release"* ]]; then
      build_folder="RelWithDebInfo"
      is_copy_asset=true
      build_project_mac
    else
      build_folder="Debug"
      is_copy_asset=false
      build_project_mac
    fi
    xcodebuild -scheme NumberGame build -configuration $build_folder
    xcodebuild -scheme UT_Engine build -configuration $build_folder
    ;;
  CYGWIN*)
    cmake ..
    ;;
  MINGW*)
    cmake ..
    ;;
  *) cmake .. ;;
  esac

  if [[ "$*" == *"run"* ]]; then
    unameOut="$(uname -s)"
    case "${unameOut}" in
    Linux*)
      $run_args ./$build_folder/${@: -1}/${@: -1}
      ;;
    Darwin*)
      $run_args ./$build_folder/${@: -1}/${@: -1}
      ;;
    CYGWIN*) ;;
    MINGW*) ;;
    *) ;;
    esac
  fi
)

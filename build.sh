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

create_asset_ln()
{
  rm -rf $build_folder/Fonts
  rm -rf $build_folder/Shaders
  rm -rf $build_folder/Images
  rm -rf $build_folder/LuaScripts
  ln -sf $PWD/../src/Engine/Fonts $build_folder
  ln -sf $PWD/../src/Engine/Render/OpenGL/Shaders $build_folder
  ln -sf $PWD/../src/Engine/Images $build_folder
  ln -sf $PWD/../src/Engine/LuaScripts $build_folder
}

copy_asset()
{
  cp -rf ../src/Engine/Fonts $build_folder
  cp -rf ../src/Engine/Render/OpenGL/Shaders $build_folder
  cp -rf ../src/Engine/Images $build_folder
  cp -rf ../src/Engine/LuaScripts $build_folder
}

(
  git submodule update --init --recursive
  (
    cd src/Engine/Dependencies/LuaJIT
    make -j$((`nproc`+1))
  )
  mkdir build

  cd build
  unameOut="$(uname -s)"
  case "${unameOut}" in
    Linux*)     if [ "$1" = "release" ] || [ "$1" = "Release" ] || [ "$2" = "release" ] || [ "$2" = "Release" ]; then
		  mkdir RelWithDebInfo
                  build_folder="RelWithDebInfo"
		  copy_asset
		  (
		    cd RelWithDebInfo
                    cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -G Ninja ../..
		    ninja
		  )
                else
		  mkdir Debug
                  build_folder="Debug"
                  create_asset_ln
		  (
		    cd Debug
                    cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja ../..
		    ninja
		  )
                fi
		;;
    Darwin*)    if [ "$1" = "release" ] || [ "$1" = "Release" ] || [ "$2" = "release" ] || [ "$2" = "Release" ]; then
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
                # xcodebuild -scheme TestGame build;
                # xcodebuild -scheme TestGame2 build;
                xcodebuild -scheme NumberGame build
		;;
    CYGWIN*)    cmake ..
	        ;;
    MINGW*)     cmake ..
	        ;;
    *)          cmake ..
  esac
)

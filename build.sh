#/bin/bash -x

clear && clear

if [ "$1" = "clean" ]; then
  rm -rf build/
  exit 0
fi

{
  git submodule update --init --recursive
  mkdir build

  cd build
  unameOut="$(uname -s)"
  case "${unameOut}" in
    Linux*)     cp -rf ../src/Engine/Fonts .
                cp -rf ../src/Engine/Render/OpenGL/Shaders .
                cp -rf ../src/Engine/Images .
                if [ "$1" = "release" ] || [ "$1" = "Release" ]; then
                  cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -G Ninja ..;
                else
                  cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja ..;
                fi
                ninja;;
    Darwin*)    if [ "$1" = "release" ] || [ "$1" = "Release" ]; then
                  mkdir Release
                  cp -rf ../src/Engine/Fonts Release
                  cp -rf ../src/Engine/Render/OpenGL/Shaders Release
                  cp -rf ../src/Engine/Images Release
                  cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -G Xcode ..;
                else
                  mkdir Debug
                  cp -rf ../src/Engine/Fonts Debug
                  cp -rf ../src/Engine/Render/OpenGL/Shaders Debug
                  cp -rf ../src/Engine/Images Debug
                  cmake -DCMAKE_BUILD_TYPE=Debug -G Xcode ..;
                fi
                xcodebuild -scheme TempEngine build;
                # xcodebuild -scheme TestGame build;
                # xcodebuild -scheme TestGame2 build;
                xcodebuild -scheme NumberGame build;;
    CYGWIN*)    cmake ..;;
    MINGW*)     cmake ..;;
    *)          cmake ..
  esac
}

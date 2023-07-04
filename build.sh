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
		  mkdir RelWithDebInfo
                  cp -rf ../src/Engine/Fonts RelWithDebInfo
                  cp -rf ../src/Engine/Render/OpenGL/Shaders RelWithDebInfo
                  cp -rf ../src/Engine/Images RelWithDebInfo
		  cp -rf ../src/Engine/LuaScripts RelWithDebInfo
                  cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo -G Xcode ..;
                else
                  mkdir Debug
                  #cp -rf ../src/Engine/Fonts Debug
                  #cp -rf ../src/Engine/Render/OpenGL/Shaders Debug
                  #cp -rf ../src/Engine/Images Debug
		  rm -rf Debug/Fonts
		  rm -rf Debug/Shaders
		  rm -rf Debug/Images
                  ln -sf $PWD/../src/Engine/Fonts Debug
                  ln -sf $PWD/../src/Engine/Render/OpenGL/Shaders Debug
                  ln -sf $PWD/../src/Engine/Images Debug
		  ln -sf $PWD/../src/Engine/LuaScripts Debug
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

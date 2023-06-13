#/bin/bash -x

if [ "$1" = "clean" ]; then
  rm -rf build/
  exit 0
fi

{
  git submodule update --init --recursive
  mkdir build
  cp -rf src/Engine/Fonts build
  cp -rf src/Engine/Render/OpenGL/Shaders build
  cp -rf src/Engine/Images build
  cd build
  unameOut="$(uname -s)"
  case "${unameOut}" in
    Linux*)     if [ "$1" = "release" ] || [ "$1" = "Release" ]; then
	    	  cmake -DCMAKE_BUILD_TYPE=Release -G Ninja ..;
		else
		  cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja ..;
		fi
                ninja;;
    Darwin*)    cmake -G Xcode ..; # Mac
                xcodebuild -scheme TempEngine build;
                xcodebuild -scheme TestGame build;
                xcodebuild -scheme TestGame2 build;
                xcodebuild -scheme NumberGame build;;
    CYGWIN*)    cmake ..;;
    MINGW*)     cmake ..;;
    *)          cmake ..
  esac
}

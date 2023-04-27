#/bin/bash -x

if [ "$1" = "clean" ]; then
  rm -rf build/
  exit 0
fi

{
  mkdir build
  cd build
  unameOut="$(uname -s)"
  case "${unameOut}" in
    Linux*)     cmake ..;
                make;;
    Darwin*)    cmake -G Xcode ..; # Mac
                xcodebuild -scheme tempEngine build;;
    CYGWIN*)    cmake ..;;
    MINGW*)     cmake ..;;
    *)          cmake ..
  esac
}

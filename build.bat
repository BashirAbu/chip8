@echo off

mkdir build 

pushd build

cmake -G "Ninja" .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=1 -DQt_DIR=D:/Qt/6.8.1/msvc2022_64/
ninja -j0

popd build

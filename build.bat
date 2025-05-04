@echo off

mkdir build 

pushd build

cmake -G "Ninja" .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=clang -DCMAKE_CXX_COMPILER=clang++ -DCMAKE_EXPORT_COMPILE_COMMANDS=1
ninja -j0

popd build

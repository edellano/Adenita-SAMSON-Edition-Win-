# Requirements:

- Qt 5.8
- CMake
- Visual Studio 2015
- Google Test

if multiple Qt versions are installed then the best solution is to set the environment setting in the visual studio  PATH=PathToQt5.8_bin;%PATH%

# CMAKE Commands

cmake -G"Visual Studio 14 Win64" -DSAMSON_SDK_PATH="C:\Users\DeLlanoE\NANO-D\SDK\0.7.0" -DQT5_CMAKE_INCLUDE_DIR="C:\Qt\5.8\msvc2015_64\lib" -DBOOST_PATH="C:/Users/DeLlanoE/Documents/code/boost_1_64_0" -DRAPIDJSON_PATH="C:\Users\DeLlanoE\Documents\code\Adenita_thirdparty\rapidjson" -DPYBIND11_PATH="C:\Users\DeLlanoE\Documents\code\Adenita_thirdparty\pybind11\include" ..

cmake -G"Visual Studio 14 Win64" -DSAMSON_SDK_PATH="C:\Users\Haichao\NANO-D\SDK\0.7.0" -DQT5_CMAKE_INCLUDE_DIR="C:\Qt\Qt5.8.0\5.8\msvc2015_64\lib" -DBOOST_PATH="C:/Development/boost_1_63_0" -DRAPIDJSON_PATH="C:\Development\DNA_Nanomodeling\Adenita\Adenita\thirdparty" -DPYBIND11_PATH="C:\Development\pybind11\include" ..

To compile python bindings it is necessary to link to the pybind11 library (https://github.com/pybind/pybind11)

-DPYBIND11_PATH="/path/to/pybind11/include"


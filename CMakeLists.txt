cmake_minimum_required(VERSION 3.10)
project(FallChallenge203)

set(CMAKE_CXX_STANDARD 14)

# ソースファイルの追加
add_executable(FallChallenge203 src/main.cpp src/GameEnv.cpp)

# インクルードディレクトリの追加
target_include_directories(FallChallenge203 PRIVATE include)
cmake_minimum_required(VERSION 3.20)
project(DiscordBot)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(dotenv-cpp)

# 必要なパッケージを探す（vcpkgでインストール済み前提）
find_package(dpp CONFIG REQUIRED)
find_package(google_cloud_cpp_compute CONFIG REQUIRED)

add_executable(discordbot
main.cpp
gce.cpp
)

target_link_libraries(discordbot PRIVATE
google-cloud-cpp::compute
dpp::dpp
dotenv)


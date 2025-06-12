cmake_minimum_required(VERSION 4.0)
project(DiscordBot)

# C++17 以降を使用
set(CMAKE_CXX_STANDARD 20)

# 必要なパッケージを探す（vcpkgでインストール済み前提）
find_package(dpp CONFIG REQUIRED)
find_package(google_cloud_cpp_compute CONFIG REQUIRED)
find_package(laserpants_dotenv)

add_executable(discordbot
main.cpp
gce.cpp
)

target_link_libraries(discordbot PRIVATE
google-cloud-cpp::compute
dpp::dpp
laserpants::dotenv)


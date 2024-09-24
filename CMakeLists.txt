cmake_minimum_required(VERSION 3.16 FATAL_ERROR)
project(ryml-test LANGUAGES CXX)

include(FetchContent)
FetchContent_Declare(ryml
    GIT_REPOSITORY https://github.com/biojppm/rapidyaml.git
    GIT_TAG v0.7.2
    GIT_SHALLOW FALSE  # ensure submodules are checked out
)
FetchContent_MakeAvailable(ryml)

add_executable(ryml-test main.cpp)
target_link_libraries(ryml-test ryml::ryml)
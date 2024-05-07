cmake_minimum_required(VERSION 3.5.0)
project(Project4Take2 VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(SFML_DIR "C:/Libraries/SFML-2.6.1/lib/cmake/SFML")
find_package(SFML 2.6 COMPONENTS graphics audio REQUIRED)

# add_library(ToolBox Toolbox.h)
# target_link_libraries(ToolBox sfml-graphics sfml-audio)

add_executable(Project4Take2 main.cpp)
target_link_libraries(Project4Take2 sfml-graphics sfml-audio)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

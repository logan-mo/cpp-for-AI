cmake_minimum_required(VERSION 3.15)
project(EigenPractice)

set(CMAKE_CXX_STANDARD 17)

# Find Eigen
find_package(Eigen3 REQUIRED)

# Auto-detect every .cpp file and make it its own executable
file(GLOB SOURCES "*.cpp")

foreach(SOURCE ${SOURCES})
    # Turn filename like "01_setting_up_eigen.cpp" into target name "01_setting_up_eigen"
    get_filename_component(TARGET_NAME ${SOURCE} NAME_WE)
    add_executable(${TARGET_NAME} ${SOURCE})
    target_link_libraries(${TARGET_NAME} Eigen3::Eigen)
endforeach()
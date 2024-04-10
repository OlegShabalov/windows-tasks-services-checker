cmake_minimum_required(VERSION 3.5)

project(TasksServicesChecker LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static")

set(PROJECT_SOURCES
        main.cpp
        CheckServices.hpp CheckServices.cpp
        CheckTasks.hpp CheckTasks.cpp
        ConfigReader.hpp ConfigReader.cpp
        Conclusion.hpp Conclusion.cpp
)

add_executable(TasksServicesChecker ${PROJECT_SOURCES})

target_link_libraries(TasksServicesChecker PRIVATE -ltaskschd -lole32 -loleaut32)

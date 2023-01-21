cmake_minimum_required(VERSION 3.15)

#set vcpkg env variables! You don't need that if you have set those environment variables
#set(VCPKG_TARGET_TRIPLET x64-windows)
#set(VCPKG_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/vcpkg)

# Configure vcpkg
include(cmake/app_utils.cmake)
setup_vcpkg_before_project()

#setup the program language: c++ and some othe parameters
project(CryptoLocker LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# configure qt
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

#configuration for Boost library
#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
#set(Boost_INCLUDE_DIR "./vcpkg_installed/x64-windows/include")
#message(STATUS "Boost_INCLUDE_DIR = ${Boost_INCLUDE_DIR}")
find_package(Boost REQUIRED COMPONENTS filesystem system)

add_executable(CryptoLocker
  main.cpp
  view/screenblocker.h
  view/screenblocker.cpp
  service/folderservice.cpp
  service/folderservice.h
)

target_link_libraries(CryptoLocker
    PRIVATE
	Qt${QT_VERSION_MAJOR}::Widgets 
        ${Boost_FILESYSTEM_LIBRARY}
        Boost::system
        Boost::filesystem
)

install(TARGETS CryptoLocker
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

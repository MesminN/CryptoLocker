cmake_minimum_required(VERSION 3.14)

project(CryptoLocker LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(VCPKG_ROOT_DIR ${CMAKE_CURRENT_LIST_DIR}/vcpkg)
#set(VCPKG_TARGET_TRIPLET x64-windows)
#set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT_DIR}/scripts/buildsystems/vcpkg.cmake)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)
#find_package(Boost 1.81.0 REQUIRED COMPONENTS filesystem system)

add_executable(CryptoLocker
  main.cpp
  view/screenblocker.h
  view/screenblocker.cpp
  #service/folderservice.cpp
  #service/folderservice.h
)
target_link_libraries(CryptoLocker Qt${QT_VERSION_MAJOR}::Widgets) #PRIVATE Boost::filesystem Boost::system

install(TARGETS CryptoLocker
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

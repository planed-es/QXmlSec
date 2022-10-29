cmake_minimum_required(VERSION 3.14)

project(xmlsec-qt LANGUAGES CXX)

option(BUILD_TESTS "builds tests" ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Xml)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Xml)
find_package(PkgConfig)
pkg_check_modules(XMLSEC REQUIRED xmlsec1-openssl>=1.0.0)

include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${XMLSEC_INCLUDE_DIRS} /usr/include/xmlsec1)

file(GLOB headers xmlsec-qt/*.h)
file(GLOB sources xmlsec-qt/*.cpp)

add_library(xmlsec-qt SHARED
  ${headers}
  ${sources}
)

target_compile_definitions(xmlsec-qt PRIVATE XMLSECQT_LIBRARY)

target_link_libraries(xmlsec-qt PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Xml
  ${XMLSEC_LIBRARIES})

if (BUILD_TESTS)
#  add_subdirectory(tests)
endif()

install(FILES ${headers} DESTINATION include/xmlsec-qt)
install(TARGETS xmlsec-qt LIBRARY DESTINATION lib)

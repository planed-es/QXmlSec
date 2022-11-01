cmake_minimum_required(VERSION 3.14)

project(xmlsec-qt
  LANGUAGES CXX
  VERSION 1.0.0
  DESCRIPTION "XML digital signatures for Qt")

option(BUILD_TESTS "builds tests" ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 REQUIRED COMPONENTS Core Xml Network)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Xml Network)
find_package(PkgConfig)
pkg_check_modules(XMLSEC REQUIRED xmlsec1-openssl>=1.0.0)

include_directories(${ZLIB_INCLUDE_DIRS})
include_directories(${XMLSEC_INCLUDE_DIRS} /usr/include/xmlsec1)

file(GLOB headers xmlsec-qt/*.h)
file(GLOB sources xmlsec-qt/*.cpp)
file(GLOB xades_headers xmlsec-qt/xades/*.h)
file(GLOB xades_sources xmlsec-qt/xades/*.cpp)

add_library(xmlsec-qt SHARED
  ${headers} ${xades_headers}
  ${sources} ${xades_sources}
)

target_compile_definitions(xmlsec-qt PRIVATE XMLSECQT_LIBRARY)

target_link_libraries(xmlsec-qt PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Xml
  ${XMLSEC_LIBRARIES})

if (BUILD_TESTS)
#  add_subdirectory(tests)
endif()

configure_file(xmlsec-qt.pc.in xmlsec-qt.pc @ONLY)

install(FILES ${headers} DESTINATION include/xmlsec-qt)
install(FILES ${xades_headers} DESTINATION include/xmlsec-qt/xades)
install(TARGETS xmlsec-qt LIBRARY DESTINATION lib)
install(FILES ${CMAKE_BINARY_DIR}/xmlsec-qt.pc DESTINATION lib/pkgconfig)

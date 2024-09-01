find_package(QT NAMES Qt6 Qt5 COMPONENTS Test Core REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Test Core REQUIRED)

enable_testing(true)
include_directories(..)

add_executable(pkcs12xmlsign tst_pkcs12xmlsign.cpp tst_pkcs12xmlsign_canSignDocument.cpp)
add_test(NAME pkcs12xmlsign COMMAND pkcs12xmlsign)
target_link_libraries(pkcs12xmlsign PRIVATE
  Qt${QT_VERSION_MAJOR}::Core
  Qt${QT_VERSION_MAJOR}::Xml
  Qt${QT_VERSION_MAJOR}::Network
  Qt${QT_VERSION_MAJOR}::Test
  ${XMLSEC_LIBRARIES}
  ${TARGET_NAME})

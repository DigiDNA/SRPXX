cmake_minimum_required(VERSION 3.16)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

project(SRPXX
  VERSION 0.1.0
  LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

file(GLOB_RECURSE SRPXX_SOURCES
  CONFIGURE_DEPENDS
  "${CMAKE_CURRENT_SOURCE_DIR}/SRPXX/source/*.cpp"
)

add_library(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${SRPXX_SOURCES})

target_include_directories(${PROJECT_NAME}
  PUBLIC
    "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/SRPXX/include>"
    "$<INSTALL_INTERFACE:include>"
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

# SRPXX uses OpenSSL for hashing / bignum. On Apple / Windows builds this is
# the vendored BoringSSL submodule; on Linux the system OpenSSL headers are
# used instead (BoringSSL-only APIs are shimmed in BigNum.cpp).
find_package(OpenSSL REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE OpenSSL::Crypto)

# SHA*_Init / SHA*_Update / SHA*_Final are deprecated in OpenSSL 3 but still
# provided. Silence the deprecation warnings.
if(NOT OPENSSL_VERSION VERSION_LESS 3.0)
  target_compile_options(${PROJECT_NAME} PRIVATE -Wno-deprecated-declarations)
endif()

option(SRPXX_BUILD_TESTS "Build SRPXX unit tests" ON)

if(SRPXX_BUILD_TESTS)
  enable_testing()

  set(XSTEST_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Submodules/XSTest/XSTest/include")
  if(NOT EXISTS "${XSTEST_INCLUDE_DIR}/XSTest/XSTest.hpp")
    message(FATAL_ERROR "XSTest submodule not initialized — run `git submodule update --init Submodules/XSTest`")
  endif()

  file(GLOB SRPXX-Tests_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_CURRENT_SOURCE_DIR}/SRPXX-Tests/*.cpp"
  )

  add_executable(SRPXX-Tests ${SRPXX-Tests_SOURCES})
  target_include_directories(SRPXX-Tests PRIVATE
    "${XSTEST_INCLUDE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/SRPXX-Tests"
  )
  target_link_libraries(SRPXX-Tests PRIVATE ${PROJECT_NAME} OpenSSL::Crypto)
  # -fkeep-inline-functions: XSTest defines XS::Test::GetAllInfos() as an
  # inline function only in the TU that defines XSTEST_MAIN_RUN (main.cpp).
  # At -O2 gcc inlines every call there and never emits the out-of-line
  # comdat copy, leaving the other TUs' references undefined at link time.
  target_compile_options(SRPXX-Tests PRIVATE -fpermissive -fkeep-inline-functions)

  add_test(NAME SRPXX-Tests COMMAND SRPXX-Tests)
endif()

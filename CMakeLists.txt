cmake_minimum_required(VERSION 3.10)

project(seafile VERSION 13.0.20 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

option(ENABLE_LDAP "Enable LDAP" OFF)
option(ENABLE_HTTPSERVER "Enable httpserver" ON)
option(WITH_POSTGRESQL "Enable postgresql support" OFF)
option(WITH_MYSQL "Enable mysql support" ON)

option(LINT "clang-tidy" OFF)

if(LINT)
    set(CMAKE_C_CLANG_TIDY clang-tidy
            --checks=-clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling,-clang-analyzer-deadcode.DeadStores)
endif()

if(APPLE)
    set(MACOS 1)
    add_definitions(-D_DARWIN_C_SOURCE)
elseif(WIN32)
else()
    set(LINUX 1)
endif()

add_definitions(-D_FILE_OFFSET_BITS=64)

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(SQLite3 REQUIRED)
find_package(Python3 REQUIRED COMPONENTS Interpreter)
find_library(UUID_LIBRARY NAMES uuid REQUIRED)

pkg_check_modules(SSL REQUIRED openssl)
pkg_check_modules(GLIB2 REQUIRED glib-2.0)
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)
pkg_check_modules(SEARPC REQUIRED libsearpc>=1.0)
pkg_check_modules(JANSSON REQUIRED jansson>=2.2.1)
pkg_check_modules(LIBEVENT REQUIRED libevent>=2.0)
pkg_check_modules(ZLIB REQUIRED zlib>=1.2.0)
pkg_check_modules(LIBHIREDIS REQUIRED hiredis>=0.15.0)
pkg_check_modules(CURL REQUIRED libcurl>=7.17)
pkg_check_modules(JWT REQUIRED libjwt)
pkg_check_modules(ARGON2 REQUIRED libargon2)
pkg_check_modules(LIBARCHIVE REQUIRED libarchive>=2.8.5)

if(WITH_MYSQL)
    pkg_check_modules(MYSQL mysqlclient REQUIRED)
    add_definitions(-DHAVE_MYSQL=1)
endif()

if(WITH_POSTGRESQL)
    pkg_check_modules(PGSQL libpq REQUIRED)
    add_definitions(-DHAVE_POSTGRESQL=1)
endif()

if(ENABLE_LDAP)
    find_library(LDAP_LIBRARY NAMES ldap REQUIRED)
    find_library(LBER_LIBRARY NAMES lber REQUIRED)
    set(LDAP_LIBS ${LDAP_LIBRARY} ${LBER_LIBRARY})
    add_definitions(-DHAVE_LDAP=1)
endif()

if(ENABLE_HTTPSERVER)
    pkg_check_modules(EVHTP evhtp)
    if(NOT EVHTP_FOUND)
        find_library(EVHTP_LIBRARIES NAMES evhtp)
    endif()

    if(EVHTP_LIBRARIES)
        add_definitions(-DHAVE_EVHTP=1)
        set(EVHTP_LIBS ${EVHTP_LIBRARIES})
    else()
        message(WARNING "evhtp not found via pkg-config or find_library. Disabling httpserver.")
        set(ENABLE_HTTPSERVER OFF)
    endif()
endif()

configure_file(config.cmake.h config.h)

include_directories(
    ${SSL_INCLUDE_DIRS}
    ${GLIB2_INCLUDE_DIRS}
    ${GOBJECT_INCLUDE_DIRS}
    ${SEARPC_INCLUDE_DIRS}
    ${JANSSON_INCLUDE_DIRS}
    ${LIBEVENT_INCLUDE_DIRS}
    ${ZLIB_INCLUDE_DIRS}
    ${LIBHIREDIS_INCLUDE_DIRS}
    ${CURL_INCLUDE_DIRS}
    ${JWT_INCLUDE_DIRS}
    ${ARGON2_INCLUDE_DIRS}
    ${LIBARCHIVE_INCLUDE_DIRS}
    ${SQLite3_INCLUDE_DIRS}
    ${MYSQL_INCLUDE_DIRS}
    ${PGSQL_INCLUDE_DIRS}
)

add_subdirectory(lib)
add_subdirectory(common/cdc)
add_subdirectory(server)
add_subdirectory(scripts)

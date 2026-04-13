set(VALA_SOURCES
    branch.vala
    ccnetobj.vala
    commit.vala
    copy-task.vala
    crypt.vala
    dir.vala
    dirent.vala
    repo.vala
    seahub.vala
    search-result.vala
    task.vala
    webaccess.vala
)

set(VALA_C_OUTPUTS)
foreach(vala_file ${VALA_SOURCES})
    string(REPLACE ".vala" ".c" c_file ${vala_file})
    list(APPEND VALA_C_OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/generated/${c_file})
endforeach()

file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated)

set(VALA_SOURCES_FULL)
foreach(vala_file ${VALA_SOURCES})
    list(APPEND VALA_SOURCES_FULL ${CMAKE_CURRENT_SOURCE_DIR}/${vala_file})
endforeach()

add_custom_command(
    OUTPUT ${VALA_C_OUTPUTS} ${CMAKE_CURRENT_BINARY_DIR}/generated/seafile-object.h
    COMMAND valac -C --pkg posix ${VALA_SOURCES_FULL} --directory=${CMAKE_CURRENT_BINARY_DIR}/generated -H ${CMAKE_CURRENT_BINARY_DIR}/generated/seafile-object.h
    DEPENDS ${VALA_SOURCES_FULL}
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
)

set_source_files_properties(
        ${VALA_C_OUTPUTS}
        PROPERTIES
        SKIP_LINTING ON
        COMPILE_FLAGS "-w"
)

find_program(SEARPC_CODEGEN NAMES searpc-codegen.py REQUIRED)
set(SEARPC_OUTPUTS ${CMAKE_CURRENT_BINARY_DIR}/generated/searpc-signature.h ${CMAKE_CURRENT_BINARY_DIR}/generated/searpc-marshal.h)
add_custom_command(
    OUTPUT ${SEARPC_OUTPUTS}
    COMMAND ${Python3_EXECUTABLE} ${SEARPC_CODEGEN} ${CMAKE_CURRENT_SOURCE_DIR}/rpc_table.py
    DEPENDS rpc_table.py
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/generated
)

set(UTILS_SOURCES
    bloom-filter.c
    db.c
    job-mgr.c
    net.c
    timer.c
    utils.c
)

add_library(seafile_common STATIC
    ${VALA_C_OUTPUTS}
    ${UTILS_SOURCES}
    ${SEARPC_OUTPUTS}
)

target_include_directories(seafile_common PUBLIC
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/common
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_BINARY_DIR}
    ${CMAKE_CURRENT_BINARY_DIR}/generated
)

target_link_libraries(seafile_common
    ${GLIB2_LIBRARIES}
    ${GOBJECT_LIBRARIES}
    ${SSL_LIBRARIES}
    crypto
    ${UUID_LIBRARY}
    ${SQLite3_LIBRARIES}
    ${LIBEVENT_LIBRARIES}
    ${SEARPC_LIBRARIES}
    ${ZLIB_LIBRARIES}
)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/generated/seafile-object.h DESTINATION include/seafile)

set(SEARPC_LIBS "-L${SEARPC_LIBRARY_DIRS} -lsearpc")
set(SEARPC_CFLAGS "-I${SEARPC_INCLUDE_DIRS}")

configure_file(libseafile.cmake.pc libseafile.pc @ONLY)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/libseafile.pc DESTINATION lib/pkgconfig)

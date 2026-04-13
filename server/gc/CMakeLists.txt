set(common_sources
    repo-mgr.c
    seafile-session.c
    ../../common/block-backend-fs.c
    ../../common/block-backend.c
    ../../common/block-mgr.c
    ../../common/branch-mgr.c
    ../../common/commit-mgr.c
    ../../common/config-mgr.c
    ../../common/fs-mgr.c
    ../../common/log.c
    ../../common/obj-backend-fs.c
    ../../common/obj-store.c
    ../../common/password-hash.c
    ../../common/seaf-db.c
    ../../common/seaf-queries-mysql.c
    ../../common/seaf-queries-pgsql.c
    ../../common/seaf-queries-sqlite.c
    ../../common/seaf-utils.c
    ../../common/seafile-crypt.c
)

add_executable(seafserv-gc
    seafserv-gc.c
    verify.c
    gc-core.c
    ${common_sources}
)

target_include_directories(seafserv-gc PRIVATE .)

target_compile_definitions(seafserv-gc PRIVATE SEAFILE_SERVER)
target_link_libraries(seafserv-gc
    cdc
    seafile_common
    ${GLIB2_LIBRARIES}
    ${GOBJECT_LIBRARIES}
    ${SSL_LIBRARIES}
    ${UUID_LIBRARY}
    ${SQLite3_LIBRARIES}
    ${LIBEVENT_LIBRARIES}
    ${SEARPC_LIBRARIES}
    ${JANSSON_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${MYSQL_LIBRARIES}
    ${PGSQL_LIBRARIES}
    ${ARGON2_LIBRARIES}
    Threads::Threads
)

add_executable(seaf-fsck
    seaf-fsck.c
    fsck.c
    ${common_sources}
)

target_include_directories(seaf-fsck PRIVATE .)

target_compile_definitions(seaf-fsck PRIVATE SEAFILE_SERVER)
target_link_libraries(seaf-fsck
    cdc
    seafile_common
    ${GLIB2_LIBRARIES}
    ${GOBJECT_LIBRARIES}
    ${SSL_LIBRARIES}
    ${UUID_LIBRARY}
    ${SQLite3_LIBRARIES}
    ${LIBEVENT_LIBRARIES}
    ${SEARPC_LIBRARIES}
    ${JANSSON_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${MYSQL_LIBRARIES}
    ${PGSQL_LIBRARIES}
    ${ARGON2_LIBRARIES}
    Threads::Threads
)

install(TARGETS seafserv-gc seaf-fsck DESTINATION bin)

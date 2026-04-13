set(seaf_fuse_SOURCES
    file.c
    getattr.c
    readdir.c
    repo-mgr.c
    seaf-fuse.c
    seafile-session.c
    ../common/block-mgr.c
    ../common/user-mgr.c
    ../common/group-mgr.c
    ../common/org-mgr.c
    ../common/block-backend.c
    ../common/block-backend-fs.c
    ../common/branch-mgr.c
    ../common/commit-mgr.c
    ../common/fs-mgr.c
    ../common/log.c
    ../common/seaf-db.c
    ../common/seaf-queries-pgsql.c
    ../common/seaf-queries-mysql.c
    ../common/seaf-queries-sqlite.c
    ../common/seaf-utils.c
    ../common/obj-store.c
    ../common/obj-backend-fs.c
    ../common/obj-backend-riak.c
    ../common/seafile-crypt.c
    ../common/password-hash.c
)

add_executable(seaf-fuse ${seaf_fuse_SOURCES})

target_include_directories(seaf-fuse PRIVATE .)

target_compile_definitions(seaf-fuse PRIVATE SEAFILE_SERVER)

target_link_libraries(seaf-fuse
    cdc
    ${GLIB2_LIBRARIES}
    ${GOBJECT_LIBRARIES}
    ${SSL_LIBRARIES}
    ${UUID_LIBRARY}
    ${SQLite3_LIBRARIES}
    ${LIBEVENT_LIBRARIES}
    ${SEARPC_LIBRARIES}
    ${JANSSON_LIBRARIES}
    ${FUSE_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${LDAP_LIBS}
    ${MYSQL_LIBRARIES}
    ${ARGON2_LIBRARIES}
    Threads::Threads
)

install(TARGETS seaf-fuse DESTINATION bin)

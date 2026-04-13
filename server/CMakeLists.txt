add_subdirectory(gc)

set(seaf_server_SOURCES
    access-file.c
    change-set.c
    copy-mgr.c
    fileserver-config.c
    http-server.c
    http-tx-mgr.c
    index-blocks-mgr.c
    metric-mgr.c
    notif-mgr.c
    pack-dir.c
    passwd-mgr.c
    quota-mgr.c
    repo-mgr.c
    repo-op.c
    repo-perm.c
    seaf-server.c
    seafile-session.c
    share-mgr.c
    size-sched.c
    upload-file.c
    virtual-repo.c
    web-accesstoken-mgr.c
    zip-download-mgr.c
    ../common/block-backend-fs.c
    ../common/block-backend.c
    ../common/block-mgr.c
    ../common/block-tx-utils.c
    ../common/branch-mgr.c
    ../common/commit-mgr.c
    ../common/config-mgr.c
    ../common/diff-simple.c
    ../common/fs-mgr.c
    ../common/group-mgr.c
    ../common/log.c
    ../common/merge-new.c
    ../common/mq-mgr.c
    ../common/obj-backend-fs.c
    ../common/obj-cache.c
    ../common/obj-store.c
    ../common/object-list.c
    ../common/org-mgr.c
    ../common/password-hash.c
    ../common/redis-cache.c
    ../common/rpc-service.c
    ../common/seaf-db.c
    ../common/seaf-queries-mysql.c
    ../common/seaf-queries-pgsql.c
    ../common/seaf-queries-sqlite.c
    ../common/seaf-utils.c
    ../common/seafile-crypt.c
    ../common/user-mgr.c
    ../common/vc-common.c
)

add_executable(seaf-server ${seaf_server_SOURCES})

target_include_directories(seaf-server PRIVATE .)

target_compile_definitions(seaf-server PRIVATE
    SEAFILE_SERVER
    FULL_FEATURE
)

target_link_libraries(seaf-server
    seafile_common
    cdc
    ${GLIB2_LIBRARIES}
    ${GOBJECT_LIBRARIES}
    ${SSL_LIBRARIES}
    ${UUID_LIBRARY}
    ${SQLite3_LIBRARIES}
    ${LIBEVENT_LIBRARIES}
    ${EVHTP_LIBS}
    ${SEARPC_LIBRARIES}
    ${JANSSON_LIBRARIES}
    ${ZLIB_LIBRARIES}
    ${LIBARCHIVE_LIBRARIES}
    ${LDAP_LIBS}
    ${MYSQL_LIBRARIES}
    ${PGSQL_LIBRARIES}
    ${CURL_LIBRARIES}
    ${JWT_LIBRARIES}
    ${LIBHIREDIS_LIBRARIES}
    ${ARGON2_LIBRARIES}
    Threads::Threads
)

install(TARGETS seaf-server DESTINATION bin)

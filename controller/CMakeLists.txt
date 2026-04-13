add_executable(seafile-controller
    seafile-controller.c
    ../common/log.c
)

target_compile_definitions(seafile-controller PRIVATE SEAFILE_SERVER)

target_link_libraries(seafile-controller
    seafile_common
    ${GLIB2_LIBRARIES}
    ${GOBJECT_LIBRARIES}
    ${SSL_LIBRARIES}
    ${UUID_LIBRARY}
    ${LIBEVENT_LIBRARIES}
    ${SEARPC_LIBRARIES}
    ${JANSSON_LIBRARIES}
    ${ZLIB_LIBRARIES}
    Threads::Threads
)

install(TARGETS seafile-controller DESTINATION bin)

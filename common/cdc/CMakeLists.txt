add_library(cdc STATIC
    cdc.c
    rabin-checksum.c
)

target_link_libraries(cdc
    ${SSL_LIBRARIES}
    ${GLIB2_LIBRARIES}
    seafile_common
)

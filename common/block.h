#pragma once

typedef struct _BMetadata BlockMetadata;
typedef struct _BMetadata BMetadata;

struct _BMetadata {
    char        id[41];
    uint32_t    size;
};

/* Opaque block handle.
 */
typedef struct _BHandle BlockHandle;
typedef struct _BHandle BHandle;

enum {
    BLOCK_READ,
    BLOCK_WRITE,
};

typedef gboolean (*SeafBlockFunc) (const char *store_id,
                                   int version,
                                   const char *block_id,
                                   void *user_data);

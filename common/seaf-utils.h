#pragma once

#include <searpc-client.h>

struct _SeafileSession;

int
load_database_config (struct _SeafileSession *session);

int
load_ccnet_database_config (struct _SeafileSession *session);

#ifdef FULL_FEATURE
#endif

char *
seaf_gen_notif_server_jwt (const char *repo_id, const char *username);

char *
seaf_parse_auth_token (const char *auth_token);

void
split_filename (const char *filename, char **name, char **ext);

int
seaf_delete_repo_tokens (SeafRepo *repo);

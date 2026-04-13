#pragma once

struct _SeafileSession;

struct WebATPriv;

struct _SeafWebAccessTokenManager {
    struct _SeafileSession	*seaf;
    struct WebATPriv *priv;
};
typedef struct _SeafWebAccessTokenManager SeafWebAccessTokenManager;

SeafWebAccessTokenManager* seaf_web_at_manager_new (struct _SeafileSession *seaf);

int
seaf_web_at_manager_start (SeafWebAccessTokenManager *mgr);

/*
 * Returns an access token for the given access info.
 * If a token doesn't exist or has expired, generate and return a new one.
 */
char *
seaf_web_at_manager_get_access_token (SeafWebAccessTokenManager *mgr,
                                      const char *repo_id,
                                      const char *obj_id,
                                      const char *op,
                                      const char *username,
                                      int use_onetime,
                                      GError **error);

/*
 * Returns access info for the given token.
 */
SeafileWebAccess *
seaf_web_at_manager_query_access_token (SeafWebAccessTokenManager *mgr,
                                        const char *token);

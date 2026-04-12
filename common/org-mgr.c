/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */

#include "common.h"

#include "seafile-session.h"
#include "seaf-db.h"
#include "org-mgr.h"
#include "seaf-utils.h"

#include "utils.h"
#include "log.h"

#define DEFAULT_MAX_CONNECTIONS 100

struct _CcnetOrgManagerPriv
{
    CcnetDB	*db;
};

static int open_db (CcnetOrgManager *manager);
static int check_db_table (CcnetDB *db);

CcnetOrgManager* ccnet_org_manager_new (SeafileSession *session)
{
    CcnetOrgManager *manager = g_new0 (CcnetOrgManager, 1);

    manager->session = session;
    manager->priv = g_new0 (CcnetOrgManagerPriv, 1);

    return manager;
}

int
ccnet_org_manager_init (CcnetOrgManager *manager)
{
    return 0;
}

int
ccnet_org_manager_prepare (CcnetOrgManager *manager)
{
    return open_db (manager);
}

static CcnetDB *
open_sqlite_db (CcnetOrgManager *manager)
{
    CcnetDB *db = NULL;
    char *db_dir;
    char *db_path;

    db_dir = g_build_filename (manager->session->ccnet_dir, "OrgMgr", NULL);
    if (checkdir_with_mkdir(db_dir) < 0) {
        ccnet_error ("Cannot open db dir %s: %s\n", db_dir,
                     strerror(errno));
        g_free (db_dir);
        return NULL;
    }
    g_free (db_dir);

    db_path = g_build_filename (manager->session->ccnet_dir, "OrgMgr",
                                "orgmgr.db", NULL);
    db = seaf_db_new_sqlite (db_path, DEFAULT_MAX_CONNECTIONS);

    g_free (db_path);

    return db;
}

static int
open_db (CcnetOrgManager *manager)
{
    CcnetDB *db = NULL;

    switch (seaf_db_type(manager->session->ccnet_db)) {
    case SEAF_DB_TYPE_SQLITE:
        db = open_sqlite_db (manager);
        break;
    case SEAF_DB_TYPE_PGSQL:
        db = manager->session->ccnet_db;
        break;
    }

    if (!db)
        return -1;
    
    manager->priv->db = db;
    if ((manager->session->create_tables || seaf_db_type(db) == SEAF_DB_TYPE_PGSQL)
         && check_db_table (db) < 0) {
        ccnet_warning ("Failed to create org db tables.\n");
        return -1;
    }

    return 0;
}

void ccnet_org_manager_start (CcnetOrgManager *manager)
{
}

/* -------- Group Database Management ---------------- */

static int check_db_table (CcnetDB *db)
{
    SeafDBQueries *queries = seaf_db_get_queries(db);

    if (seaf_db_query (db, queries->create_table_organization) < 0) return -1;
    if (seaf_db_query (db, queries->create_table_org_user) < 0) return -1;
    if (seaf_db_query (db, queries->create_table_org_group) < 0) return -1;

    return 0;
}

int ccnet_org_manager_create_org (CcnetOrgManager *mgr,
                                  const char *org_name,
                                  const char *url_prefix,
                                  const char *creator,
                                  GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    gint64 now = get_current_time();
    int rc;

    rc = seaf_db_statement_query (db, queries->insert_organization,
        4, "string", org_name, "string", url_prefix, "string", creator, "int64", now);
    
    if (rc < 0) {
        g_set_error (error, CCNET_DOMAIN, 0, "Failed to create organization");
        return -1;
    }

    int org_id = seaf_db_statement_get_int (db, queries->get_organization_org_id,
        1, "string", url_prefix);
    if (org_id < 0) {
        g_set_error (error, CCNET_DOMAIN, 0, "Failed to create organization");
        return -1;
    }

    rc = seaf_db_statement_query (db, queries->insert_org_user,
        3, "int", org_id, "string", creator, "int", 1);
    if (rc < 0) {
        seaf_db_statement_query (db, queries->delete_organization,
            1, "int", org_id);
        g_set_error (error, CCNET_DOMAIN, 0, "Failed to create organization");
        return -1;
    }
    
    return org_id;
}

int
ccnet_org_manager_remove_org (CcnetOrgManager *mgr,
                              int org_id,
                              GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);


    seaf_db_statement_query (db, queries->delete_organization,
        1, "int", org_id);

    seaf_db_statement_query (db, queries->delete_org_user_by_org,
        1, "int", org_id);

    seaf_db_statement_query (db, queries->delete_org_group_by_org,
        1, "int", org_id);

    return 0;
}


static gboolean
get_all_orgs_cb (CcnetDBRow *row, void *data)
{
    GList **p_list = data;
    CcnetOrganization *org = NULL;
    int org_id;
    const char *org_name;
    const char *url_prefix;
    const char *creator;
    gint64 ctime;

    org_id = seaf_db_row_get_column_int (row, 0);
    org_name = seaf_db_row_get_column_text (row, 1);
    url_prefix = seaf_db_row_get_column_text (row, 2);
    creator = seaf_db_row_get_column_text (row, 3);
    ctime = seaf_db_row_get_column_int64 (row, 4);

    org = g_object_new (CCNET_TYPE_ORGANIZATION,
                        "org_id", org_id,
                        "org_name", org_name,
                        "url_prefix", url_prefix,
                        "creator", creator,
                        "ctime", ctime,
                        NULL);

    *p_list = g_list_prepend (*p_list, org);

    return TRUE;
}

GList *
ccnet_org_manager_get_all_orgs (CcnetOrgManager *mgr,
                                int start,
                                int limit)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    GList *ret = NULL;
    int rc;

    if (start == -1 && limit == -1) {
        rc = seaf_db_statement_foreach_row (db, queries->list_organization,
            get_all_orgs_cb, &ret, 0);
    } else {
        rc = seaf_db_statement_foreach_row (db, queries->list_organization_paginated,
            get_all_orgs_cb, &ret,
            2, "int", limit, "int", start);
    }

    if (rc < 0)
        return NULL;

    return g_list_reverse (ret);
}

int
ccnet_org_manager_count_orgs (CcnetOrgManager *mgr)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    gint64 ret;

    ret = seaf_db_get_int64 (db, queries->count_organization);
    if (ret < 0)
        return -1;
    return ret;
}

static gboolean
get_org_cb (CcnetDBRow *row, void *data)
{
    CcnetOrganization **p_org = data;
    int org_id;
    const char *org_name;
    const char *url_prefix;
    const char *creator;
    gint64 ctime;

    org_id = seaf_db_row_get_column_int (row, 0);    
    org_name = seaf_db_row_get_column_text (row, 1);
    url_prefix = seaf_db_row_get_column_text (row, 2);
    creator = seaf_db_row_get_column_text (row, 3);
    ctime = seaf_db_row_get_column_int64 (row, 4);

    *p_org = g_object_new (CCNET_TYPE_ORGANIZATION,
                           "org_id", org_id,
                           "org_name", org_name,
                           "url_prefix", url_prefix,
                           "creator", creator,
                           "ctime", ctime,
                           NULL);
    return FALSE;
}

CcnetOrganization *
ccnet_org_manager_get_org_by_url_prefix (CcnetOrgManager *mgr,
                                         const char *url_prefix,
                                         GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    CcnetOrganization *org = NULL;

    if (seaf_db_statement_foreach_row (db, queries->get_organization_by_url_prefix,
            get_org_cb, &org,
            1, "string", url_prefix) < 0) {
        return NULL;
    }

    return org;
}

CcnetOrganization *
ccnet_org_manager_get_org_by_id (CcnetOrgManager *mgr,
                                 int org_id,
                                 GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    CcnetOrganization *org = NULL;

    if (seaf_db_statement_foreach_row (db, queries->get_organization,
            get_org_cb, &org,
            1, "int", org_id) < 0) {
        return NULL;
    }

    return org;
}

int
ccnet_org_manager_add_org_user (CcnetOrgManager *mgr,
                                int org_id,
                                const char *email,
                                int is_staff,
                                GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    return seaf_db_statement_query (db, queries->insert_org_user,
        3, "int", org_id, "string", email, "int", is_staff);
}

int
ccnet_org_manager_remove_org_user (CcnetOrgManager *mgr,
                                   int org_id,
                                   const char *email,
                                   GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    return seaf_db_statement_query (db, queries->delete_org_user,
        2, "int", org_id, "string", email);
}

static gboolean
get_orgs_by_user_cb (CcnetDBRow *row, void *data)
{
    GList **p_list = (GList **)data;
    CcnetOrganization *org = NULL;
    int org_id;
    const char *email;
    int is_staff;
    const char *org_name;
    const char *url_prefix;
    const char *creator;
    gint64 ctime;

    org_id = seaf_db_row_get_column_int (row, 0);
    email = (char *) seaf_db_row_get_column_text (row, 1);
    is_staff = seaf_db_row_get_column_int (row, 2);
    org_name = (char *) seaf_db_row_get_column_text (row, 3);
    url_prefix = (char *) seaf_db_row_get_column_text (row, 4);
    creator = (char *) seaf_db_row_get_column_text (row, 5);
    ctime = seaf_db_row_get_column_int64 (row, 6);
    
    org = g_object_new (CCNET_TYPE_ORGANIZATION,
                        "org_id", org_id,
                        "email", email,
                        "is_staff", is_staff,
                        "org_name", org_name,
                        "url_prefix", url_prefix,
                        "creator", creator,
                        "ctime", ctime,
                        NULL);
    *p_list = g_list_prepend (*p_list, org);
        
    return TRUE;
}

GList *
ccnet_org_manager_get_orgs_by_user (CcnetOrgManager *mgr,
                                   const char *email,
                                   GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    GList *ret = NULL;

    if (seaf_db_statement_foreach_row (db, queries->get_org_user_by_email,
            get_orgs_by_user_cb, &ret,
            1, "string", email) < 0) {
        g_list_free (ret);
        return NULL;
    }

    return g_list_reverse (ret);
}

static gboolean
get_org_emailusers (CcnetDBRow *row, void *data)
{
    GList **list = (GList **)data;
    const char *email = (char *) seaf_db_row_get_column_text (row, 0);

    *list = g_list_prepend (*list, g_strdup (email));
    return TRUE;
}

GList *
ccnet_org_manager_get_org_emailusers (CcnetOrgManager *mgr,
                                      const char *url_prefix,
                                      int start, int limit)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    GList *ret = NULL;
    int rc;

    if (start == -1 && limit == -1) {
        rc = seaf_db_statement_foreach_row (db, queries->list_org_user,
            get_org_emailusers, &ret,
            1, "string", url_prefix);
    } else {
        rc = seaf_db_statement_foreach_row (db, queries->list_org_user_paginated,
            get_org_emailusers, &ret,
            3, "string", url_prefix, "int", limit, "int", start);
    }

    if (rc < 0)
        return NULL;

    return g_list_reverse (ret);
}

int
ccnet_org_manager_add_org_group (CcnetOrgManager *mgr,
                                 int org_id,
                                 int group_id,
                                 GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);


    return seaf_db_statement_query (db, queries->insert_org_group,
        2, "int", org_id, "int", group_id);
}

int
ccnet_org_manager_remove_org_group (CcnetOrgManager *mgr,
                                    int org_id,
                                    int group_id,
                                    GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    return seaf_db_statement_query (db, queries->delete_org_group,
        2, "int", org_id, "string", group_id);
}

int
ccnet_org_manager_is_org_group (CcnetOrgManager *mgr,
                                int group_id,
                                GError **error)
{
    gboolean exists, err;

    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    exists = seaf_db_statement_exists (db, queries->get_org_group_exists, &err,
        1, "int", group_id);
    if (err) {
        ccnet_warning ("DB error when check group exist in OrgGroup.\n");
        return 0;
    }
    return exists;
}

int
ccnet_org_manager_get_org_id_by_group (CcnetOrgManager *mgr,
                                       int group_id,
                                       GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    return seaf_db_statement_get_int (db, queries->get_org_group_org_id,
        1, "int", group_id);
}

static gboolean
get_org_group_ids (CcnetDBRow *row, void *data)
{
    GList **plist = data;

    int group_id = seaf_db_row_get_column_int (row, 0);

    *plist = g_list_prepend (*plist, (gpointer)(long)group_id);

    return TRUE;
}

GList *
ccnet_org_manager_get_org_group_ids (CcnetOrgManager *mgr,
                                     int org_id,
                                     int start,
                                     int limit)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    GList *ret = NULL;
    int rc;

    if (limit == -1) {
        rc = seaf_db_statement_foreach_row (db, queries->list_org_group_id,
            get_org_group_ids, &ret,
            1, "int", org_id);
    } else {
        rc = seaf_db_statement_foreach_row (db, queries->list_org_group_id_paginated,
            get_org_group_ids, &ret,
            3, "int", org_id, "int", limit, "int", start);
    }
    
    if (rc < 0) {
        g_list_free (ret);
        return NULL;
    }

    return g_list_reverse (ret);
}

static gboolean
get_org_groups (CcnetDBRow *row, void *data)
{
    GList **plist = data;
    CcnetGroup *group;

    int group_id = seaf_db_row_get_column_int (row, 0);
    const char *group_name = seaf_db_row_get_column_text (row, 1);
    const char *creator_name = seaf_db_row_get_column_text (row, 2);
    gint64 ts = seaf_db_row_get_column_int64 (row, 3);
    int parent_group_id = seaf_db_row_get_column_int (row, 4);

    group = g_object_new (CCNET_TYPE_GROUP,
                          "id", group_id,
                          "group_name", group_name,
                          "creator_name", creator_name,
                          "timestamp", ts,
                          "source", "DB",
                          "parent_group_id", parent_group_id,
                          NULL);

    *plist = g_list_prepend (*plist, group);

    return TRUE;
}

GList *
ccnet_org_manager_get_org_top_groups (CcnetOrgManager *mgr, int org_id, GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    GList *ret = NULL;
    int rc;

    rc = seaf_db_statement_foreach_row (db, queries->list_org_group_parent,
        get_org_groups, &ret,
        1, "int", org_id);
    if (rc < 0)
        return NULL;

    return g_list_reverse (ret);
}

GList *
ccnet_org_manager_get_org_groups (CcnetOrgManager *mgr,
                                  int org_id,
                                  int start,
                                  int limit)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    GList *ret = NULL;
    int rc;

    if (limit == -1) {
        rc = seaf_db_statement_foreach_row (db, queries->list_org_group,
            get_org_groups, &ret,
            1, "int", org_id);
    } else {
        rc = seaf_db_statement_foreach_row (db, queries->list_org_group_paginated,
            get_org_groups, &ret,
            3, "int", org_id, "int", limit, "int", start);
    }
    
    if (rc < 0) {
        return NULL;
    }

    return g_list_reverse (ret);
}

GList *
ccnet_org_manager_get_org_groups_by_user (CcnetOrgManager *mgr,
                                          const char *user,
                                          int org_id)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    GList *ret = NULL;
    int rc;

    rc = seaf_db_statement_foreach_row (db, queries->list_org_group_membership,
        get_org_groups, &ret,
        2, "int", org_id, "string", user);
    if (rc < 0)
        return NULL;

    return g_list_reverse (ret);
}

int
ccnet_org_manager_org_user_exists (CcnetOrgManager *mgr,
                                   int org_id,
                                   const char *email,
                                   GError **error)
{
    gboolean exists, err;

    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    exists = seaf_db_statement_exists (db, queries->get_org_user_exists, &err,
        2, "int", org_id, "string", email);
    if (err) {
        ccnet_warning ("DB error when check user exist in OrgUser.\n");
        return 0;
    }
    return exists;
}

char *
ccnet_org_manager_get_url_prefix_by_org_id (CcnetOrgManager *mgr,
                                            int org_id,
                                            GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    return seaf_db_statement_get_string (db, queries->get_organization_url_prefix,
        1, "int", org_id);
}

int
ccnet_org_manager_is_org_staff (CcnetOrgManager *mgr,
                                int org_id,
                                const char *email,
                                GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    return seaf_db_statement_get_int (db, queries->get_org_user_is_staff,
        2, "int", org_id, "string", email);
}

int
ccnet_org_manager_set_org_staff (CcnetOrgManager *mgr,
                                 int org_id,
                                 const char *email,
                                 GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    return seaf_db_statement_query (db, queries->update_org_user_is_staff_true,
        2, "int", org_id, "string", email);
}

int
ccnet_org_manager_unset_org_staff (CcnetOrgManager *mgr,
                                   int org_id,
                                   const char *email,
                                   GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    return seaf_db_statement_query (db, queries->update_org_user_is_staff_false,
        2, "int", org_id, "string", email);
}

int
ccnet_org_manager_set_org_name(CcnetOrgManager *mgr,
                               int org_id,
                               const char *org_name,
                               GError **error)
{
    CcnetDB *db = mgr->priv->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    return seaf_db_statement_query (db, queries->update_organization_org_name,
        2, "string", org_name, "int", org_id);
}


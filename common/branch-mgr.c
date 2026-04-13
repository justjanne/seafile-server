#include "common.h"

#include "log.h"

#include "seaf-db.h"

#include "seafile-session.h"

#ifdef FULL_FEATURE
#include "notif-mgr.h"
#endif

#include "branch-mgr.h"

#define BRANCH_DB "branch.db"

SeafBranch *
seaf_branch_new (const char *name, const char *repo_id, const char *commit_id)
{
    SeafBranch *branch;

    branch = g_new0 (SeafBranch, 1);

    branch->name = g_strdup (name);
    memcpy (branch->repo_id, repo_id, 36);
    branch->repo_id[36] = '\0';
    memcpy (branch->commit_id, commit_id, 40);
    branch->commit_id[40] = '\0';

    branch->ref = 1;

    return branch;
}

void
seaf_branch_free (SeafBranch *branch)
{
    if (branch == NULL) return;
    g_free (branch->name);
    g_free (branch);
}

void
seaf_branch_list_free (GList *blist)
{
    GList *ptr;

    for (ptr = blist; ptr; ptr = ptr->next) {
        seaf_branch_unref (ptr->data);
    }
    g_list_free (blist);
}


void
seaf_branch_set_commit (SeafBranch *branch, const char *commit_id)
{
    memcpy (branch->commit_id, commit_id, 40);
    branch->commit_id[40] = '\0';
}

void
seaf_branch_ref (SeafBranch *branch)
{
    branch->ref++;
}

void
seaf_branch_unref (SeafBranch *branch)
{
    if (!branch)
        return;

    if (--branch->ref <= 0)
        seaf_branch_free (branch);
}

struct _SeafBranchManagerPriv {
    sqlite3 *db;
};

static int open_db (SeafBranchManager *mgr);

SeafBranchManager *
seaf_branch_manager_new (struct _SeafileSession *seaf)
{
    SeafBranchManager *mgr;

    mgr = g_new0 (SeafBranchManager, 1);
    mgr->priv = g_new0 (SeafBranchManagerPriv, 1);
    mgr->seaf = seaf;


    return mgr;
}

int
seaf_branch_manager_init (SeafBranchManager *mgr)
{
    return open_db (mgr);
}

static int
open_db (SeafBranchManager *mgr)
{
    if (!mgr->seaf->create_tables && seaf_db_type (mgr->seaf->db) != SEAF_DB_TYPE_PGSQL)
        return 0;

#ifdef FULL_FEATURE
    SeafDB *db = mgr->seaf->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    if (seaf_db_query (db, queries->create_table_branch) < 0) return -1;
#endif

    return 0;
}

int
seaf_branch_manager_add_branch (SeafBranchManager *mgr, SeafBranch *branch)
{
    SeafDB *db = mgr->seaf->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    int rc = seaf_db_statement_query (db, queries->upsert_branch,
        3, "string", branch->name, "string", branch->repo_id, "string", branch->commit_id);
    if (rc < 0)
        return -1;

    return 0;
}

int
seaf_branch_manager_del_branch (SeafBranchManager *mgr,
                                const char *repo_id,
                                const char *name)
{

    SeafDB *db = mgr->seaf->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);
    int rc = seaf_db_statement_query (db, queries->delete_branch,
        2, "string", name, "string", repo_id);
    if (rc < 0)
        return -1;
    return 0;
}

int
seaf_branch_manager_update_branch (SeafBranchManager *mgr, SeafBranch *branch)
{

    SeafDB *db = mgr->seaf->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);
    int rc = seaf_db_statement_query (db, queries->update_branch,
        3, "string", branch->commit_id, "string", branch->name, "string", branch->repo_id);
    if (rc < 0)
        return -1;
    return 0;
}

#ifdef FULL_FEATURE

#include "mq-mgr.h"

static gboolean
get_commit_id (SeafDBRow *row, void *data)
{
    char *out_commit_id = data;
    const char *commit_id;

    commit_id = seaf_db_row_get_column_text (row, 0);
    memcpy (out_commit_id, commit_id, 41);
    out_commit_id[40] = '\0';

    return FALSE;
}

static void
publish_repo_update_event (const char *repo_id, const char *commit_id)
{
    json_t *msg = json_object ();
    char *msg_str = NULL;

    json_object_set_new (msg, "msg_type", json_string("repo-update"));
    json_object_set_new (msg, "repo_id", json_string(repo_id));
    json_object_set_new (msg, "commit_id", json_string(commit_id));

    msg_str = json_dumps (msg, JSON_PRESERVE_ORDER);

    seaf_mq_manager_publish_event (seaf->mq_mgr, SEAFILE_SERVER_CHANNEL_EVENT, msg_str);
    g_free (msg_str);
    json_decref (msg);
}

static void
notify_repo_update (const char *repo_id, const char *commit_id)
{
    json_t *event = NULL;
    json_t *content = NULL;
    char *msg = NULL;

    event = json_object ();
    content = json_object ();

    json_object_set_new (event, "type", json_string("repo-update"));

    json_object_set_new (content, "repo_id", json_string(repo_id));
    json_object_set_new (content, "commit_id", json_string(commit_id));

    json_object_set_new (event, "content", content);

    msg = json_dumps (event, JSON_COMPACT);

    if (seaf->notif_mgr)
        seaf_notif_manager_send_event (seaf->notif_mgr, msg);

    json_decref (event);
    g_free (msg);
}

static void
on_branch_updated (SeafBranchManager *mgr, SeafBranch *branch)
{
    if (seaf->is_repair)
        return;
    seaf_repo_manager_update_repo_info (seaf->repo_mgr, branch->repo_id, branch->commit_id);

    notify_repo_update(branch->repo_id, branch->commit_id);

    if (seaf_repo_manager_is_virtual_repo (seaf->repo_mgr, branch->repo_id))
        return;

    publish_repo_update_event (branch->repo_id, branch->commit_id);
}

static gboolean
get_gc_id (SeafDBRow *row, void *data)
{
    char **out_gc_id = data;

    *out_gc_id = g_strdup(seaf_db_row_get_column_text (row, 0));

    return FALSE;
}

int
seaf_branch_manager_test_and_update_branch (SeafBranchManager *mgr,
                                            SeafBranch *branch,
                                            const char *old_commit_id,
                                            gboolean check_gc,
                                            const char *last_gc_id,
                                            const char *origin_repo_id,
                                            gboolean *gc_conflict)
{
    SeafDBTrans *trans;
    char *sql;
    char commit_id[41] = { 0 };
    char *gc_id = NULL;

    if (check_gc)
        *gc_conflict = FALSE;

    trans = seaf_db_begin_transaction (mgr->seaf->db);
    if (!trans)
        return -1;

    if (check_gc) {
        sql = "SELECT gc_id FROM GCID WHERE repo_id = ? FOR UPDATE";
        if (!origin_repo_id) {
            if (seaf_db_trans_foreach_selected_row (trans, sql,
                                                    get_gc_id, &gc_id,
                                                    1, "string", branch->repo_id) < 0) {
                seaf_db_rollback (trans);
                seaf_db_trans_close (trans);
                return -1;
            }
        }
        else {
            if (seaf_db_trans_foreach_selected_row (trans, sql,
                                                    get_gc_id, &gc_id,
                                                    1, "string", origin_repo_id) < 0) {
                seaf_db_rollback (trans);
                seaf_db_trans_close (trans);
                return -1;
            }
        }

        if (g_strcmp0 (last_gc_id, gc_id) != 0) {
            seaf_warning ("Head branch update for repo %s conflicts with GC.\n",
                          branch->repo_id);
            seaf_db_rollback (trans);
            seaf_db_trans_close (trans);
            *gc_conflict = TRUE;
            g_free (gc_id);
            return -1;
        }
        g_free (gc_id);
    }

    SeafDB *db = mgr->seaf->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);
    if (seaf_db_trans_foreach_selected_row (trans, queries->get_branch_commit_id_for_update,
            get_commit_id, commit_id,
            2, "string", branch->name, "string", branch->repo_id) < 0) {
        seaf_db_rollback (trans);
        seaf_db_trans_close (trans);
        return -1;
    }
    if (strcmp (old_commit_id, commit_id) != 0) {
        seaf_db_rollback (trans);
        seaf_db_trans_close (trans);
        return -1;
    }

    if (seaf_db_trans_query (trans, queries->update_branch,
        3, "string", branch->commit_id, "string", branch->name, "string", branch->repo_id) < 0) {
        seaf_db_rollback (trans);
        seaf_db_trans_close (trans);
        return -1;
    }

    if (seaf_db_commit (trans) < 0) {
        seaf_db_rollback (trans);
        seaf_db_trans_close (trans);
        return -1;
    }

    seaf_db_trans_close (trans);

    on_branch_updated (mgr, branch);

    return 0;
}

#endif

static gboolean
get_branch (SeafDBRow *row, void *vid)
{
    char *ret = vid;
    const char *commit_id;

    commit_id = seaf_db_row_get_column_text (row, 0);
    memcpy (ret, commit_id, 41);

    return FALSE;
}

static SeafBranch *
real_get_branch (SeafBranchManager *mgr,
                 const char *repo_id,
                 const char *name)
{
    SeafDB *db = mgr->seaf->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    char commit_id[41];
    commit_id[0] = 0;

    if (seaf_db_statement_foreach_row (db, queries->get_branch_commit_id,
            get_branch, commit_id,
            2, "string", name, "string", repo_id) < 0) {
        seaf_warning ("[branch mgr] DB error when get branch %s.\n", name);
        return NULL;
    }

    if (commit_id[0] == 0)
        return NULL;

    return seaf_branch_new (name, repo_id, commit_id);
}

SeafBranch *
seaf_branch_manager_get_branch (SeafBranchManager *mgr,
                                const char *repo_id,
                                const char *name)
{
    SeafBranch *branch;

    /* "fetch_head" maps to "master" on server. */
    if (strcmp (name, "fetch_head") == 0) {
        branch = real_get_branch (mgr, repo_id, "master");
        return branch;
    } else {
        return real_get_branch (mgr, repo_id, name);
    }
}

gboolean
seaf_branch_manager_branch_exists (SeafBranchManager *mgr,
                                   const char *repo_id,
                                   const char *name)
{
    gboolean db_err = FALSE;

    SeafDB *db = mgr->seaf->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    return seaf_db_statement_exists (db, queries->get_branch_name,  &db_err,
        2, "string", name, "string", repo_id);
}

static gboolean
get_branches (SeafDBRow *row, void *vplist)
{
    GList **plist = vplist;
    const char *commit_id;
    const char *name;
    const char *repo_id;
    SeafBranch *branch;

    name = seaf_db_row_get_column_text (row, 0);
    repo_id = seaf_db_row_get_column_text (row, 1);
    commit_id = seaf_db_row_get_column_text (row, 2);

    branch = seaf_branch_new (name, repo_id, commit_id);
    *plist = g_list_prepend (*plist, branch);

    return TRUE;
}

GList *
seaf_branch_manager_get_branch_list (SeafBranchManager *mgr,
                                     const char *repo_id)
{
    SeafDB *db = mgr->seaf->db;
    SeafDBQueries *queries = seaf_db_get_queries(db);

    GList *ret = NULL;

    if (seaf_db_statement_foreach_row (db, queries->get_branch,
            get_branches, &ret,
            1, "string", repo_id) < 0) {
        seaf_warning ("[branch mgr] DB error when get branch list.\n");
        return NULL;
    }

    return ret;
}

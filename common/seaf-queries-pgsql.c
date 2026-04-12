#include "common.h"
#include "seaf-queries.h"


SeafDBQueries queries_pgsql = {
    .user_count =
        "SELECT COUNT(id)"
        " FROM EmailUser"
        " WHERE is_active = true;",
    .inactive_user_count =
        "SELECT COUNT(id)"
        " FROM EmailUser"
        " WHERE is_active = false;",
    .get_superusers =
        "SELECT t1.id, t1.email, t1.is_staff, t1.is_active, t1.ctime, t2.role, t1.passwd"
        " FROM EmailUser t1 LEFT JOIN UserRole t2 ON t1.email = t2.email"
        " WHERE is_staff = true AND t1.email NOT LIKE '%%@seafile_group';",

    .ldap_user_count =
        "SELECT COUNT(id)"
        " FROM LDAPUsers"
        " WHERE is_active = true;",
    .ldap_inactive_user_count =
        "SELECT COUNT(id)"
        " FROM LDAPUsers"
        " WHERE is_active = false;",
    .get_ldap_superusers =
        "SELECT t1.id, t1.email, t1.is_staff, t1.is_active, t2.role"
        " FROM LDAPUsers t1 LEFT JOIN \"userrole\" t2 ON t1.email = t2.email"
        " WHERE is_staff = true;",

    .create_table_repo =
        "CREATE TABLE IF NOT EXISTS Repo ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(37) UNIQUE"
        ");",
    .create_table_repo_owner =
        "CREATE TABLE IF NOT EXISTS RepoOwner ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(37) UNIQUE,"
            "owner_id VARCHAR(255)"
        ");"
        "CREATE INDEX IF NOT EXISTS RepoOwner_owner_id_idx"
            " ON RepoOwner(owner_id);",
    .create_table_repo_group =
        "CREATE TABLE IF NOT EXISTS RepoGroup ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(37),"
            "group_id INTEGER,"
            "user_name VARCHAR(255),"
            "permission CHAR(15),"
            "UNIQUE (group_id, repo_id)"
        ");"
        "CREATE INDEX IF NOT EXISTS RepoGroup_repo_id_idx"
            " ON RepoGroup(repo_id);"
        "CREATE INDEX IF NOT EXISTS RepoGroup_user_name_idx"
            " ON RepoGroup(user_name);",
    .create_table_inner_pub_repo =
        "CREATE TABLE IF NOT EXISTS InnerPubRepo ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(37) UNIQUE,"
            "permission CHAR(15)"
        ");",
    .create_table_repo_user_token =
        "CREATE TABLE IF NOT EXISTS RepoUserToken ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(37),"
            "email VARCHAR(255),"
            "token CHAR(41),"
            "UNIQUE (repo_id, token)"
        ");"
        "CREATE INDEX IF NOT EXISTS RepoUserToken_token_idx"
            " ON RepoUserToken(token);"
        "CREATE INDEX IF NOT EXISTS RepoUserToken_email_idx"
            " ON RepoUserToken(email);",
    .create_table_repo_token_peer_info =
        "CREATE TABLE IF NOT EXISTS RepoTokenPeerInfo ("
            "id BIGSERIAL PRIMARY KEY,"
            "token CHAR(41) UNIQUE,"
            "peer_id CHAR(41),"
            "peer_ip VARCHAR(50),"
            "peer_name VARCHAR(255),"
            "sync_time BIGINT,"
            "client_ver VARCHAR(20)"
        ");"
        "CREATE INDEX IF NOT EXISTS RepoTokenPeerInfo_peer_id_idx"
            " ON RepoTokenPeerInfo(peer_id);",
    .create_table_repo_head =
        "CREATE TABLE IF NOT EXISTS RepoHead ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(37) UNIQUE,"
            "branch_name VARCHAR(10)"
        ");",
    .create_table_repo_size =
        "CREATE TABLE IF NOT EXISTS RepoSize ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(37) UNIQUE,"
            "\"size\" BIGINT,"
            "head_id CHAR(41)"
        ");",
    .create_table_repo_history_limit =
        "CREATE TABLE IF NOT EXISTS RepoHistoryLimit ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(37) UNIQUE,"
            "days INTEGER"
        ");",
    .create_table_repo_valid_since =
        "CREATE TABLE IF NOT EXISTS RepoValidSince ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(37) UNIQUE,"
            "\"timestamp\" BIGINT"
        ");",
    .create_table_web_ap =
        "CREATE TABLE IF NOT EXISTS WebAP ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(37) UNIQUE,"
            "access_property CHAR(10)"
        ");",
    .create_table_virtual_repo =
        "CREATE TABLE IF NOT EXISTS VirtualRepo ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(36) UNIQUE,"
            "origin_repo CHAR(36),"
            "path TEXT,"
            "base_commit CHAR(40)"
        ");"
        "CREATE INDEX IF NOT EXISTS VirtualRepo_origin_repo_idx"
            " ON VirtualRepo(origin_repo);",
    .create_table_garbage_repos =
        "CREATE TABLE IF NOT EXISTS GarbageRepos ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(36) UNIQUE"
        ");",
    .create_table_gcid =
        "CREATE TABLE IF NOT EXISTS GCID ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(36) UNIQUE,"
            "gc_id CHAR(36)"
        ");",
    .create_table_last_gcid =
        "CREATE TABLE IF NOT EXISTS LastGCID ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(36),"
            "client_id VARCHAR(128),"
            "gc_id CHAR(36),"
            "UNIQUE (repo_id, client_id)"
        ");",
    .create_table_repo_trash =
        "CREATE TABLE IF NOT EXISTS RepoTrash ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(36) UNIQUE,"
            "repo_name VARCHAR(255),"
            "head_id CHAR(40),"
            "owner_id VARCHAR(255),"
            "\"size\" BIGINT,"
            "org_id INTEGER,"
            "del_time BIGINT"
        ");"
        "CREATE INDEX IF NOT EXISTS RepoTrash_owner_id_idx"
            " ON RepoTrash(owner_id);"
        "CREATE INDEX IF NOT EXISTS RepoTrash_org_id_idx"
            " ON RepoTrash(org_id);",
    .create_table_repo_file_count =
        "CREATE TABLE IF NOT EXISTS RepoFileCount ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(36) UNIQUE,"
            "file_count BIGINT"
        ");",
    .create_table_repo_info =
        "CREATE TABLE IF NOT EXISTS RepoInfo ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(36) UNIQUE,"
            "name VARCHAR(255) NOT NULL,"
            "update_time BIGINT,"
            "version INTEGER,"
            "is_encrypted INTEGER,"
            "last_modifier VARCHAR(255),"
            "status INTEGER DEFAULT 0,"
            "type VARCHAR(10)"
        ");"
        "CREATE INDEX IF NOT EXISTS RepoInfo_type_idx"
            " ON RepoInfo(type);",
    .create_table_webupload_temp_files =
        "CREATE TABLE IF NOT EXISTS WebUploadTempFiles ("
            "id BIGSERIAL PRIMARY KEY,"
            "repo_id CHAR(40) NOT NULL,"
            "file_path TEXT NOT NULL,"
            "tmp_file_path TEXT NOT NULL"
        ");"
        "CREATE INDEX IF NOT EXISTS WebUploadTempFiles_repo_id_idx"
            " ON WebUploadTempFiles(repo_id);",

    .create_table_user_quota =
        "CREATE TABLE IF NOT EXISTS UserQuota ("
              "id BIGSERIAL PRIMARY KEY,"
              "\"user\" VARCHAR(255),"
              "quota BIGINT,"
              "UNIQUE (\"user\")"
        ");",
    .create_table_user_share_quota =
        "CREATE TABLE IF NOT EXISTS UserShareQuota ("
              "id BIGSERIAL PRIMARY KEY,"
              "\"user\" VARCHAR(255),"
              "quota BIGINT,"
              "UNIQUE (\"user\")"
        ");",
    .create_table_org_quota =
        "CREATE TABLE IF NOT EXISTS OrgQuota ("
              "id BIGSERIAL PRIMARY KEY,"
              "org_id INTEGER,"
              "quota BIGINT,"
              "UNIQUE (org_id)"
        ");",
    .create_table_org_user_quota =
        "CREATE TABLE IF NOT EXISTS OrgUserQuota ("
              "id BIGSERIAL PRIMARY KEY,"
              "org_id INTEGER,"
              "\"user\" VARCHAR(255),"
              "quota BIGINT,"
              "UNIQUE (org_id, \"user\")"
        ");",

    .create_table_system_info =
        "CREATE TABLE IF NOT EXISTS SystemInfo ("
            "id BIGSERIAL PRIMARY KEY, "
            "info_key VARCHAR(256), "
            "info_value VARCHAR(1024)"
        ");",
    .create_table_shared_repo =
        "CREATE TABLE IF NOT EXISTS SharedRepo ("
              "id BIGSERIAL PRIMARY KEY,"
              "repo_id CHAR(37),"
              "from_email VARCHAR(255),"
              "to_email VARCHAR(255),"
              "permission CHAR(15)"
        ");"
        "CREATE INDEX IF NOT EXISTS SharedRepo_repo_id_idx"
            " ON SharedRepo(repo_id);"
        "CREATE INDEX IF NOT EXISTS SharedRepo_from_email_idx"
            " ON SharedRepo(from_email);"
        "CREATE INDEX IF NOT EXISTS SharedRepo_to_email_idx"
            " ON SharedRepo(to_email);",

    .create_table_seafile_conf =
        "CREATE TABLE IF NOT EXISTS SeafileConf ("
            "id BIGSERIAL PRIMARY KEY,"
            "cfg_group VARCHAR(255) NOT NULL,"
            "cfg_key VARCHAR(255) NOT NULL,"
            "value VARCHAR(255),"
            "property INTEGER"
        ");",
    .get_seafile_conf_exists =
        "SELECT 1 FROM SeafileConf"
        " WHERE cfg_group=? AND cfg_key=?;",
    .get_seafile_conf_value =
        "SELECT value FROM SeafileConf"
        " WHERE cfg_group=? AND cfg_key=?;",
    .insert_seafile_conf =
        "INSERT INTO SeafileConf (value, cfg_group, cfg_key, property)"
        " VALUES (?,?,?,0);",
    .update_seafile_conf =
        "UPDATE SeafileConf SET value=?"
        " WHERE cfg_group=? AND cfg_key=?;",

    .create_table_group =
        "CREATE TABLE IF NOT EXISTS \"group\" ("
            "\"group_id\" BIGSERIAL PRIMARY KEY,"
            "\"group_name\" VARCHAR(255),"
            "\"creator_name\" VARCHAR(255),"
            "\"timestamp\" BIGINT,"
            "\"type\" VARCHAR(32),"
            "\"parent_group_id\" INTEGER"
        ");",
    .insert_group =
        "INSERT INTO \"group\" (group_name, creator_name, timestamp, parent_group_id)"
        " VALUES(?, ?, ?, ?);",
    .get_group =
        "SELECT group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM \"group\""
        " WHERE group_id = ?;",
    .get_group_group_id =
        "SELECT group_id"
        " FROM \"group\""
        " WHERE group_name = ? AND creator_name = ? AND timestamp = ?;",
    .get_group_exists =
        "SELECT group_id"
        " FROM \"group\""
        " WHERE group_id=?;",
    .get_group_has_children =
        "SELECT 1"
        " FROM \"group\""
        " WHERE parent_group_id=?;",
    .list_group_by_path =
        "SELECT g.group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM \"group\" g"
        " WHERE g.group_id IN(%s)"
        " ORDER BY g.group_id;",
    .list_group_by_user =
        "SELECT g.group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM \"group\" g, \"groupuser\" u"
        " WHERE g.group_id = u.group_id AND user_name=?"
        " ORDER BY g.group_id DESC;",
    .list_group_by_parent =
        "SELECT group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM \"group\""
        " WHERE parent_group_id=?;",
    .list_group_by_ancestor =
        "SELECT g.group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM \"group\" g, GroupStructure s"
        " WHERE g.group_id=s.group_id AND (s.path LIKE ? OR s.path LIKE ? OR g.group_id=?);",
    .update_group =
        "UPDATE \"group\" SET group_name = ?"
        " WHERE group_id = ?;",
    .delete_group =
        "DELETE FROM \"group\""
        " WHERE group_id=?;",

    .create_table_group_user =
        "CREATE TABLE IF NOT EXISTS \"groupuser\" ("
            "\"id\" BIGSERIAL PRIMARY KEY,"
            "\"group_id\" BIGINT,"
            "\"user_name\" VARCHAR(255),"
            "\"is_staff\" BOOLEAN,"
            "UNIQUE (\"group_id\", \"user_name\")"
        ");"
        "CREATE INDEX IF NOT EXISTS groupuser_username_idx"
            " ON \"groupuser\" (\"user_name\");",
    .insert_group_user =
        "INSERT INTO \"groupuser\" (group_id, user_name, is_staff)"
        " VALUES (?, ?, ?);",
    .get_group_user_is_staff =
        "SELECT group_id"
        " FROM \"groupuser\""
        " WHERE group_id = ? AND user_name = ? AND is_staff = true;",
    .get_group_user_group_id =
        "SELECT group_id"
        " FROM \"groupuser\""
        " WHERE group_id IN (%s) AND user_name = ? AND is_staff = true;",
    .list_group_user =
        "SELECT group_id, user_name, is_staff"
        " FROM \"groupuser\""
        " WHERE group_id = ?;",
    .list_group_user_paginated =
        "SELECT group_id, user_name, is_staff"
        " FROM \"groupuser\""
        " WHERE group_id = ?"
        " LIMIT ? OFFSET ?;",
    .update_group_user_is_staff_true =
        "UPDATE \"groupuser\" SET is_staff = true"
        " WHERE group_id = ? and user_name = ?;",
    .update_group_user_is_staff_false =
        "UPDATE \"groupuser\" SET is_staff = false"
        " WHERE group_id = ? and user_name = ?;",
    .delete_group_user =
        "DELETE FROM \"groupuser\""
        " WHERE group_id=? AND user_name=?;",
    .delete_group_user_by_group =
        "DELETE FROM \"groupuser\""
        " WHERE group_id=?;",

    .create_table_group_dn_pair =
        "CREATE TABLE IF NOT EXISTS GroupDNPair ("
            "id BIGSERIAL PRIMARY KEY,"
            "group_id INTEGER,"
            "dn VARCHAR(255)"
        ");",
    .create_table_group_structure =
        "CREATE TABLE IF NOT EXISTS GroupStructure ("
            "id BIGSERIAL PRIMARY KEY,"
            "group_id INTEGER,"
            "path VARCHAR(1024),"
            "UNIQUE (group_id)"
        ");",
    .insert_group_structure =
        "INSERT INTO GroupStructure (group_id, path)"
        " VALUES (?, ?);",
    .get_group_structure_path =
        "SELECT path"
        " FROM GroupStructure"
        " WHERE group_id=?;",
    .delete_group_structure_by_group =
        "DELETE FROM GroupStructure"
        " WHERE group_id=?;",

    .create_table_organization =
        "CREATE TABLE IF NOT EXISTS Organization ("
            "org_id BIGSERIAL PRIMARY KEY,"
            "org_name VARCHAR(255),"
            "url_prefix VARCHAR(255),"
            "creator VARCHAR(255),"
            "ctime BIGINT,"
            "UNIQUE (url_prefix)"
        ");",
    .insert_organization =
        "INSERT INTO Organization (org_name, url_prefix, creator, ctime)"
        " VALUES (?, ?, ?, ?);",
    .list_organization =
        "SELECT *"
        " FROM Organization"
        " ORDER BY org_id;",
    .list_organization_paginated =
        "SELECT *"
        " FROM Organization"
        " ORDER BY org_id"
        " LIMIT ? OFFSET ?;",
    .count_organization =
        "SELECT count(*)"
        " FROM Organization;",
    .get_organization =
        "SELECT org_id, org_name, url_prefix, creator, ctime"
        " FROM Organization"
        " WHERE org_id = ?;",
    .get_organization_by_url_prefix =
        "SELECT org_id, org_name, url_prefix, creator, ctime"
        " FROM Organization"
        " WHERE url_prefix = ?;",
    .get_organization_org_id =
        "SELECT org_id"
        " FROM Organization"
        " WHERE url_prefix = ?;",
    .get_organization_url_prefix =
        "SELECT url_prefix"
        " FROM Organization"
        " WHERE org_id = ?;",
    .update_organization_org_name =
        "UPDATE Organization set org_name = ?"
        " WHERE org_id = ?;",
    .delete_organization =
        "DELETE FROM Organization"
        " WHERE org_id = ?;",

    .create_table_org_user =
        "CREATE TABLE IF NOT EXISTS OrgUser ("
            "id BIGSERIAL PRIMARY KEY,"
            "org_id INTEGER,"
            "email VARCHAR(255),"
            "is_staff BOOL NOT NULL,"
            "UNIQUE (org_id, email)"
        ");"
        "CREATE INDEX IF NOT EXISTS orguser_email_idx"
            " ON OrgUser (email);",
    .insert_org_user =
        "INSERT INTO OrgUser (org_id, email, is_staff)"
        " VALUES (?, ?, ?);",
    .get_org_user_by_email =
        "SELECT t1.org_id, email, is_staff, org_name, url_prefix, creator, ctime"
        " FROM OrgUser t1, Organization t2"
        " WHERE t1.org_id = t2.org_id AND email = ?;",
    .list_org_user =
        "SELECT u.email"
        " FROM OrgUser u, Organization o"
        " WHERE u.org_id = o.org_id AND o.url_prefix = ?"
        " ORDER BY email;",
    .list_org_user_paginated =
        "SELECT u.email"
        " FROM OrgUser u, Organization o"
        " WHERE u.org_id = o.org_id AND o.url_prefix = ?"
        " ORDER BY email"
        " LIMIT ? OFFSET ?;",
    .get_org_user_exists =
        "SELECT org_id"
        " FROM OrgUser"
        " WHERE org_id = ? AND email = ?;",
    .get_org_user_is_staff =
        "SELECT is_staff"
        " FROM OrgUser"
        " WHERE org_id=? AND email=?;",
    .update_org_user_is_staff_true =
        "UPDATE OrgUser SET is_staff = true"
        " WHERE org_id=? AND email=?;",
    .update_org_user_is_staff_false =
        "UPDATE OrgUser SET is_staff = false"
        " WHERE org_id=? AND email=?;",
    .delete_org_user =
        "DELETE FROM OrgUser"
        " WHERE org_id=? AND email=?;",
    .delete_org_user_by_org =
        "DELETE FROM OrgUser"
        " WHERE org_id = ?;",

    .create_table_org_group =
        "CREATE TABLE IF NOT EXISTS OrgGroup ("
            "id BIGSERIAL PRIMARY KEY,"
            "org_id INTEGER,"
            "group_id INTEGER,"
            "UNIQUE (org_id, group_id)"
        ");"
        "CREATE INDEX IF NOT EXISTS orggroup_group_id_idx"
            " ON OrgGroup (group_id);",
    .insert_org_group =
        "INSERT INTO OrgGroup (org_id, group_id)"
        " VALUES (?, ?);",
    .get_org_group_exists =
        "SELECT group_id"
        " FROM OrgGroup"
        " WHERE group_id = ?;",
    .get_org_group_org_id =
        "SELECT org_id"
        " FROM OrgGroup"
        " WHERE group_id = ?;",
    .list_org_group_id =
        "SELECT group_id"
        " FROM OrgGroup"
        " WHERE org_id = ?;",
    .list_org_group_id_paginated =
        "SELECT group_id"
        " FROM OrgGroup"
        " WHERE org_id = ?"
        " LIMIT ? OFFSET ?;",
    .list_org_group_parent =
        "SELECT g.group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM `OrgGroup` o, `Group` g"
        " WHERE o.group_id = g.group_id AND org_id=? AND parent_group_id=-1"
        " ORDER BY timestamp DESC;",
    .list_org_group =
        "SELECT g.group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM OrgGroup o, `Group` g"
        " WHERE o.group_id = g.group_id AND org_id = ?;",
    .list_org_group_paginated =
        "SELECT g.group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM OrgGroup o, `Group` g"
        " WHERE o.group_id = g.group_id AND org_id = ?"
        " LIMIT ? OFFSET ?;",
    .list_org_group_membership =
        "SELECT g.group_id, group_name, creator_name, timestamp"
        " FROM OrgGroup o, `Group` g, GroupUser u"
        " WHERE o.group_id = g.group_id AND org_id = ? AND g.group_id = u.group_id AND user_name = ?;",
    .delete_org_group =
        "DELETE FROM OrgGroup"
        " WHERE org_id=? AND group_id=?;",
    .delete_org_group_by_org =
        "DELETE FROM OrgGroup"
        " WHERE org_id = ?;",

    .create_table_email_user =
        "CREATE TABLE IF NOT EXISTS EmailUser ("
            "id BIGSERIAL PRIMARY KEY,"
            "email VARCHAR(255) UNIQUE,"
            "passwd VARCHAR(256),"
            "is_staff BOOLEAN NOT NULL,"
            "is_active BOOLEAN NOT NULL,"
            "ctime BIGINT,"
            "reference_id VARCHAR(255) UNIQUE"
        ");",
    .create_table_binding =
        "CREATE TABLE IF NOT EXISTS Binding ("
            "id BIGSERIAL PRIMARY KEY,"
            "email VARCHAR(255),"
            "peer_id CHAR(41) UNIQUE"
        ");"
        "CREATE INDEX IF NOT EXISTS Binding_email_idx"
            " ON Binding(email);",
    .create_table_user_role =
        "CREATE TABLE IF NOT EXISTS UserRole ("
            "id BIGSERIAL PRIMARY KEY,"
            "email VARCHAR(255) UNIQUE,"
            "role VARCHAR(255)"
        ");",
    .create_table_ldap_users =
        "CREATE TABLE IF NOT EXISTS LDAPUsers ("
            "id BIGSERIAL PRIMARY KEY,"
            "email VARCHAR(255) NOT NULL UNIQUE,"
            "password VARCHAR(255) NOT NULL,"
            "is_staff BOOLEAN NOT NULL,"
            "is_active BOOLEAN NOT NULL,"
            "extra_attrs TEXT,"
            "reference_id VARCHAR(255) UNIQUE"
        ");",
    .create_table_ldap_config =
        "CREATE TABLE IF NOT EXISTS LDAPConfig ("
            "id BIGSERIAL PRIMARY KEY,"
            "cfg_group VARCHAR(255) NOT NULL,"
            "cfg_key VARCHAR(255) NOT NULL,"
            "value VARCHAR(255),"
            "property INTEGER"
        ");",

    .create_table_branch =
        "CREATE TABLE IF NOT EXISTS Branch ("
            "id BIGSERIAL PRIMARY KEY,"
            "name VARCHAR(10),"
            "repo_id CHAR(41),"
            "commit_id CHAR(41),"
            "UNIQUE (repo_id, name)"
        ");",
    .upsert_branch =
        "INSERT INTO Branch (name, repo_id, commit_id)"
        " VALUES (?, ?, ?)"
        " ON CONFLICT (repo_id, name)"
        " DO UPDATE SET commit_id = EXCLUDED.commit_id;",
    .delete_branch =
        "DELETE FROM Branch"
        " WHERE name=? AND repo_id=?;",
    .update_branch =
        "UPDATE Branch SET commit_id = ?"
        " WHERE name = ? AND repo_id = ?;",
    .get_branch_commit_id_for_update =
        "SELECT commit_id FROM Branch"
        " WHERE name=? AND repo_id=?"
        " FOR UPDATE;",
    .get_branch_commit_id =
        "SELECT commit_id FROM Branch"
        " WHERE name=? AND repo_id=?;",
    .get_branch_name =
        "SELECT name FROM Branch"
        " WHERE name=? AND repo_id=?;",
    .get_branch =
        "SELECT name, repo_id, commit_id FROM Branch"
        " WHERE repo_id=?;",
};

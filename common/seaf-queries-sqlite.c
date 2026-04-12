#include "common.h"
#include "seaf-queries.h"


SeafDBQueries queries_sqlite = {
    .user_count = "SELECT COUNT(id) FROM EmailUser WHERE is_active = 1",
    .inactive_user_count = "SELECT COUNT(id) FROM EmailUser WHERE is_active = 0",
    .ldap_user_count = "SELECT COUNT(id) FROM LDAPUsers WHERE is_active = 1",
    .ldap_inactive_user_count = "SELECT COUNT(id) FROM LDAPUsers WHERE is_active = 0",
    .get_superusers = "SELECT t1.id, t1.email, "
        "t1.is_staff, t1.is_active, t1.ctime, "
        "t2.role, t1.passwd FROM EmailUser t1 "
        "LEFT JOIN UserRole t2 "
        "ON t1.email = t2.email "
        "WHERE is_staff = 1 AND t1.email NOT LIKE '%%@seafile_group'",
    .get_ldap_superusers = "SELECT t1.id, t1.email, "
        "t1.is_staff, t1.is_active, "
        "t2.role FROM LDAPUsers t1 "
        "LEFT JOIN UserRole t2 "
        "ON t1.email = t2.email "
        "WHERE is_staff = 1",

    .add_group_member = "INSERT INTO GroupUser (group_id, user_name, is_staff) VALUES (?, ?, ?)",
    .remove_group_member = "DELETE FROM GroupUser WHERE group_id=? AND user_name=?",
    .set_group_admin = "UPDATE GroupUser SET is_staff = 1 WHERE group_id = ? and user_name = ?",
    .unset_group_admin = "UPDATE GroupUser SET is_staff = 0 WHERE group_id = ? and user_name = ?",

    .create_table_repo =
        "CREATE TABLE IF NOT EXISTS Repo ("
            "repo_id CHAR(37) PRIMARY KEY"
        ");",
    .create_table_repo_owner =
        "CREATE TABLE IF NOT EXISTS RepoOwner ("
            "repo_id CHAR(37) PRIMARY KEY,"
            "owner_id TEXT"
        ");"
        "CREATE INDEX IF NOT EXISTS OwnerIndex"
            " ON RepoOwner (owner_id)",
    .create_table_repo_group =
        "CREATE TABLE IF NOT EXISTS RepoGroup ("
            "repo_id CHAR(37),"
            "group_id INTEGER,"
            "user_name TEXT,"
            "permission CHAR(15)"
        ");"
        "CREATE UNIQUE INDEX IF NOT EXISTS groupid_repoid_indx"
            " ON RepoGroup (group_id, repo_id);"
        "CREATE INDEX IF NOT EXISTS repogroup_repoid_index"
            " ON RepoGroup (repo_id);"
        "CREATE INDEX IF NOT EXISTS repogroup_username_indx"
            " ON RepoGroup (user_name);",
    .create_table_inner_pub_repo =
        "CREATE TABLE IF NOT EXISTS InnerPubRepo ("
            "repo_id CHAR(37) PRIMARY KEY,"
            "permission CHAR(15)"
        ");",
    .create_table_repo_user_token =
        "CREATE TABLE IF NOT EXISTS RepoUserToken ("
            "repo_id CHAR(37),"
            "email VARCHAR(255),"
            "token CHAR(41)"
        ");"
        "CREATE UNIQUE INDEX IF NOT EXISTS repo_token_indx"
            " ON RepoUserToken (repo_id, token);"
        "CREATE INDEX IF NOT EXISTS repo_token_email_indx"
            " ON RepoUserToken (email);",
    .create_table_repo_token_peer_info =
        "CREATE TABLE IF NOT EXISTS RepoTokenPeerInfo ("
            "token CHAR(41) PRIMARY KEY,"
            "peer_id CHAR(41),"
            "peer_ip VARCHAR(50),"
            "peer_name VARCHAR(255),"
            "sync_time BIGINT,"
            "client_ver VARCHAR(20)"
        ");",
    .create_table_repo_head =
        "CREATE TABLE IF NOT EXISTS RepoHead ("
            "repo_id CHAR(37) PRIMARY KEY,"
            "branch_name VARCHAR(10)"
        ");",
    .create_table_repo_size =
        "CREATE TABLE IF NOT EXISTS RepoSize ("
            "repo_id CHAR(37) PRIMARY KEY,"
            "size BIGINT UNSIGNED,"
            "head_id CHAR(41)"
        ");",
    .create_table_repo_history_limit =
        "CREATE TABLE IF NOT EXISTS RepoHistoryLimit ("
            "repo_id CHAR(37) PRIMARY KEY,"
            "days INTEGER"
        ");",
    .create_table_repo_valid_since =
        "CREATE TABLE IF NOT EXISTS RepoValidSince ("
            "repo_id CHAR(37) PRIMARY KEY,"
            "timestamp BIGINT"
        ");",
    .create_table_web_ap =
        "CREATE TABLE IF NOT EXISTS WebAP ("
            "repo_id CHAR(37) PRIMARY KEY,"
           "access_property CHAR(10)"
       ");",
    .create_table_virtual_repo =
        "CREATE TABLE IF NOT EXISTS VirtualRepo ("
            "repo_id CHAR(36) PRIMARY KEY,"
            "origin_repo CHAR(36),"
            "path TEXT,"
            "base_commit CHAR(40)"
        ");"
        "CREATE INDEX IF NOT EXISTS virtualrepo_origin_repo_idx"
            " ON VirtualRepo (origin_repo);",
    .create_table_garbage_repos =
        "CREATE TABLE IF NOT EXISTS GarbageRepos ("
            "repo_id CHAR(36) PRIMARY KEY"
        ");",
    .create_table_gcid = NULL,
    .create_table_last_gcid = NULL,
    .create_table_repo_trash =
        "CREATE TABLE IF NOT EXISTS RepoTrash ("
            "repo_id CHAR(36) PRIMARY KEY,"
            "repo_name VARCHAR(255),"
            "head_id CHAR(40),"
            "owner_id VARCHAR(255),"
            "size BIGINT UNSIGNED,"
            "org_id INTEGER,"
            "del_time BIGINT"
        ");"
        "CREATE INDEX IF NOT EXISTS repotrash_owner_id_idx"
            " ON RepoTrash(owner_id);"
        "CREATE INDEX IF NOT EXISTS repotrash_org_id_idx"
            " ON RepoTrash(org_id);",
    .create_table_repo_file_count =
        "CREATE TABLE IF NOT EXISTS RepoFileCount ("
            "repo_id CHAR(36) PRIMARY KEY,"
            "file_count BIGINT UNSIGNED"
        ");",
    .create_table_repo_info =
        "CREATE TABLE IF NOT EXISTS RepoInfo ("
            "repo_id CHAR(36) PRIMARY KEY,"
            "name VARCHAR(255) NOT NULL,"
            "update_time INTEGER,"
            "version INTEGER,"
            "is_encrypted INTEGER,"
            "last_modifier VARCHAR(255),"
            "status INTEGER DEFAULT 0"
        ");",
    .create_table_webupload_temp_files =
        "CREATE TABLE IF NOT EXISTS WebUploadTempFiles ("
            "repo_id CHAR(40) NOT NULL,"
            "file_path TEXT NOT NULL,"
            "tmp_file_path TEXT NOT NULL"
        ");"
        "CREATE INDEX IF NOT EXISTS webuploadtempfiles_repo_id_idx"
            " ON WebUploadTempFiles(repo_id);",

    .create_table_user_quota =
        "CREATE TABLE IF NOT EXISTS UserQuota ("
            "user VARCHAR(255) PRIMARY KEY,"
            "quota BIGINT"
        ");",
    .create_table_user_share_quota =
        "CREATE TABLE IF NOT EXISTS UserShareQuota ("
            "user VARCHAR(255) PRIMARY KEY,"
            "quota BIGINT"
        ");",
    .create_table_org_quota =
        "CREATE TABLE IF NOT EXISTS OrgQuota ("
            "org_id INTEGER PRIMARY KEY,"
            "quota BIGINT"
        ");",
    .create_table_org_user_quota =
        "CREATE TABLE IF NOT EXISTS OrgUserQuota ("
            "org_id INTEGER,"
            "user VARCHAR(255),"
            "quota BIGINT,"
            "PRIMARY KEY (org_id, user)"
        ");",

    .create_table_system_info =
        "CREATE TABLE IF NOT EXISTS SystemInfo("
            "info_key VARCHAR(256),"
            "info_value VARCHAR(1024)"
        ");",
    .create_table_shared_repo =
        "CREATE TABLE IF NOT EXISTS SharedRepo ("
            "repo_id CHAR(37),"
            "from_email VARCHAR(255),"
            "to_email VARCHAR(255),"
            "permission CHAR(15)"
        ");"
        "CREATE INDEX IF NOT EXISTS RepoIdIndex"
            " ON SharedRepo (repo_id);"
        "CREATE INDEX IF NOT EXISTS FromEmailIndex"
            " ON SharedRepo (from_email);"
        "CREATE INDEX IF NOT EXISTS ToEmailIndex"
            " ON SharedRepo (to_email);",

    .create_table_seafile_conf =
        "CREATE TABLE IF NOT EXISTS SeafileConf ("
            "cfg_group VARCHAR(255) NOT NULL,"
            "cfg_key VARCHAR(255) NOT NULL,"
            "value VARCHAR(255),"
            "property INTEGER"
        ");",
    .get_seafile_conf_exists =
        "SELECT 1 FROM SeafileConf"
        " WHERE cfg_group=? AND cfg_key=?",
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
        "CREATE TABLE IF NOT EXISTS `Group` ("
            "`group_id` INTEGER PRIMARY KEY AUTOINCREMENT,"
            "`group_name` VARCHAR(255),"
            "`creator_name` VARCHAR(255),"
            "`timestamp` BIGINT,"
            "`type` VARCHAR(32),"
            "`parent_group_id` INTEGER"
        ");",
    .create_table_group_user =
        "CREATE TABLE IF NOT EXISTS `GroupUser` ("
            "`group_id` INTEGER,"
            "`user_name` VARCHAR(255),"
            "`is_staff` tinyint"
        ");"
        "CREATE UNIQUE INDEX IF NOT EXISTS groupid_username_indx"
            " ON `GroupUser` (`group_id`, `user_name`)"
        "CREATE INDEX IF NOT EXISTS username_indx"
            " ON `GroupUser` (`user_name`)",
    .create_table_group_dn_pair =
        "CREATE TABLE IF NOT EXISTS GroupDNPair ("
            "group_id INTEGER,"
            "dn VARCHAR(255)"
        ");",
    .create_table_group_structure =
        "CREATE TABLE IF NOT EXISTS GroupStructure ("
            "group_id INTEGER PRIMARY KEY,"
            "path VARCHAR(1024)"
        ");"
        "CREATE INDEX IF NOT EXISTS path_indx"
            " ON `GroupStructure` (`path`);",

    .create_table_organization =
        "CREATE TABLE IF NOT EXISTS Organization ("
            "org_id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "org_name VARCHAR(255),"
            "url_prefix VARCHAR(255), "
            "creator VARCHAR(255),"
            "ctime BIGINT"
        ");"
        "CREATE UNIQUE INDEX IF NOT EXISTS url_prefix_indx"
            " ON Organization (url_prefix);",
    .create_table_org_user =
        "CREATE TABLE IF NOT EXISTS OrgUser ("
            "org_id INTEGER,"
            "email TEXT,"
            "is_staff bool NOT NULL"
        ");"
        "CREATE INDEX IF NOT EXISTS email_indx"
            " ON OrgUser (email);"
        "CREATE UNIQUE INDEX IF NOT EXISTS orgid_email_indx"
            " ON OrgUser (org_id, email);",
    .create_table_org_group =
        "CREATE TABLE IF NOT EXISTS OrgGroup ("
            "org_id INTEGER,"
            "group_id INTEGER"
        ");"
        "CREATE INDEX IF NOT EXISTS groupid_indx"
            " ON OrgGroup (group_id);"
        "CREATE UNIQUE INDEX IF NOT EXISTS org_group_indx"
            " ON OrgGroup (org_id, group_id);",

    .create_table_email_user =
        "CREATE TABLE IF NOT EXISTS EmailUser ("
            "id INTEGER NOT NULL PRIMARY KEY AUTOINCREMENT,"
            "email TEXT,"
            "passwd TEXT,"
            "is_staff bool NOT NULL,"
            "is_active bool NOT NULL,"
            "ctime INTEGER,"
            "reference_id TEXT"
        ");"
        "CREATE UNIQUE INDEX IF NOT EXISTS email_index"
            " ON EmailUser (email);"
        "CREATE UNIQUE INDEX IF NOT EXISTS reference_id_index"
            " ON EmailUser (reference_id);",
    .create_table_binding =
        "CREATE TABLE IF NOT EXISTS Binding ("
            "email TEXT,"
            "peer_id TEXT"
        ");"
        "CREATE INDEX IF NOT EXISTS email_index"
            " ON Binding (email);"
        "CREATE UNIQUE INDEX IF NOT EXISTS peer_index"
            " ON Binding (peer_id);",
    .create_table_user_role =
        "CREATE TABLE IF NOT EXISTS UserRole ("
            "email TEXT,"
            "role TEXT"
        ");"
        "CREATE INDEX IF NOT EXISTS userrole_email_index"
            " ON UserRole (email);"
        "CREATE UNIQUE INDEX IF NOT EXISTS userrole_userrole_index"
            " ON UserRole (email, role);",
    .create_table_ldap_users =
        "CREATE TABLE IF NOT EXISTS LDAPUsers ("
            "id INTEGER PRIMARY KEY AUTOINCREMENT,"
            "email TEXT NOT NULL,"
            "password TEXT NOT NULL,"
            "is_staff BOOL NOT NULL,"
            "is_active BOOL NOT NULL,"
            "extra_attrs TEXT,"
            "reference_id TEXT"
        ");"
        "CREATE UNIQUE INDEX IF NOT EXISTS ldapusers_email_index"
            " ON LDAPUsers(email);"
        "CREATE UNIQUE INDEX IF NOT EXISTS ldapusers_reference_id_index"
            " ON LDAPUsers(reference_id);",
    .create_table_ldap_config =
        "CREATE TABLE IF NOT EXISTS LDAPConfig ("
            "cfg_group VARCHAR(255) NOT NULL,"
            "cfg_key VARCHAR(255) NOT NULL,"
            "value VARCHAR(255),"
            "property INTEGER"
        ");",

    .create_table_branch =
        "CREATE TABLE IF NOT EXISTS Branch ("
            "name VARCHAR(10),"
            "repo_id CHAR(41),"
            "commit_id CHAR(41),"
            "PRIMARY KEY (repo_id, name)"
        ");",
    .upsert_branch =
        "REPLACE INTO Branch (name, repo_id, commit_id)"
        " VALUES (?, ?, ?)",
    .delete_branch =
        "DELETE FROM Branch"
        " WHERE name=? AND repo_id=?",
    .update_branch =
        "UPDATE Branch SET commit_id = ?"
        " WHERE name = ? AND repo_id = ?",
    .get_branch_commit_id_for_update =
        "SELECT commit_id FROM Branch"
        " WHERE name=? AND repo_id=?",
    .get_branch_commit_id =
        "SELECT commit_id FROM Branch"
        " WHERE name=? AND repo_id=?",
    .get_branch_name =
        "SELECT name FROM Branch"
        " WHERE name=? AND repo_id=?",
    .get_branch =
        "SELECT name, repo_id, commit_id FROM Branch"
        " WHERE repo_id=?",
};

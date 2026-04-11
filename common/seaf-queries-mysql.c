#include "common.h"
#include "seaf-queries.h"


SeafDBQueries queries_mysql = {
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

    .get_branch_commit_id_for_update = "SELECT commit_id FROM Branch WHERE name=? "
        "AND repo_id=? FOR UPDATE",
    .replace_branch = "REPLACE INTO Branch (name, repo_id, commit_id) VALUES (?, ?, ?)",

    .add_group_member = "INSERT INTO GroupUser (group_id, user_name, is_staff) VALUES (?, ?, ?)",
    .remove_group_member = "DELETE FROM GroupUser WHERE group_id=? AND user_name=?",
    .set_group_admin = "UPDATE GroupUser SET is_staff = 1 WHERE group_id = ? and user_name = ?",
    .unset_group_admin = "UPDATE GroupUser SET is_staff = 0 WHERE group_id = ? and user_name = ?",

    .create_table_repo =
        "CREATE TABLE IF NOT EXISTS Repo ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "UNIQUE INDEX (repo_id)"
        ") ENGINE=INNODB;",
    .create_table_repo_owner =
        "CREATE TABLE IF NOT EXISTS RepoOwner ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "owner_id VARCHAR(255),"
            "UNIQUE INDEX (repo_id),"
            "INDEX (owner_id)"
        ") ENGINE=INNODB",
    .create_table_repo_group =
        "CREATE TABLE IF NOT EXISTS RepoGroup ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "group_id INTEGER,"
            "user_name VARCHAR(255),"
            "permission CHAR(15),"
            "UNIQUE INDEX (group_id, repo_id),"
            "INDEX (repo_id),"
            "INDEX (user_name)"
        ") ENGINE=INNODB",
    .create_table_inner_pub_repo =
        "CREATE TABLE IF NOT EXISTS InnerPubRepo ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "permission CHAR(15),"
            "UNIQUE INDEX (repo_id)"
        ") ENGINE=INNODB",
    .create_table_repo_user_token =
        "CREATE TABLE IF NOT EXISTS RepoUserToken ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "email VARCHAR(255),"
            "token CHAR(41),"
            "UNIQUE INDEX(repo_id, token),"
            "INDEX(token),"
            "INDEX (email)"
        ") ENGINE=INNODB",
    .create_table_repo_token_peer_info =
        "CREATE TABLE IF NOT EXISTS RepoTokenPeerInfo ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "token CHAR(41),"
            "peer_id CHAR(41),"
            "peer_ip VARCHAR(50),"
            "peer_name VARCHAR(255),"
            "sync_time BIGINT,"
            "client_ver VARCHAR(20),"
            "UNIQUE INDEX(token),"
            "INDEX(peer_id)"
        ") ENGINE=INNODB",
    .create_table_repo_head =
        "CREATE TABLE IF NOT EXISTS RepoHead ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "branch_name VARCHAR(10),"
            "UNIQUE INDEX(repo_id)"
        ") ENGINE=INNODB",
    .create_table_repo_size =
        "CREATE TABLE IF NOT EXISTS RepoSize ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "size BIGINT UNSIGNED,"
            "head_id CHAR(41),"
            "UNIQUE INDEX (repo_id)"
        ") ENGINE=INNODB",
    .create_table_repo_history_limit =
        "CREATE TABLE IF NOT EXISTS RepoHistoryLimit ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "days INTEGER,"
            "UNIQUE INDEX(repo_id)"
        ") ENGINE=INNODB",
    .create_table_repo_valid_since =
        "CREATE TABLE IF NOT EXISTS RepoValidSince ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "timestamp BIGINT,"
            "UNIQUE INDEX(repo_id)"
        ") ENGINE=INNODB",
    .create_table_web_ap =
        "CREATE TABLE IF NOT EXISTS WebAP ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "access_property CHAR(10),"
            "UNIQUE INDEX(repo_id)"
        ") ENGINE=INNODB",
    .create_table_virtual_repo =
        "CREATE TABLE IF NOT EXISTS VirtualRepo ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(36),"
            "origin_repo CHAR(36),"
            "path TEXT,"
            "base_commit CHAR(40),"
            "UNIQUE INDEX(repo_id),"
            "INDEX(origin_repo)"
        ") ENGINE=INNODB",
    .create_table_garbage_repos =
        "CREATE TABLE IF NOT EXISTS GarbageRepos ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(36),"
            "UNIQUE INDEX(repo_id)"
        ")",
    .create_table_gcid =
        "CREATE TABLE IF NOT EXISTS GCID ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(36),"
            "gc_id CHAR(36"
            "UNIQUE INDEX(repo_id)"
        ") ENGINE=INNODB",
    .create_table_last_gcid =
        "CREATE TABLE IF NOT EXISTS LastGCID ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(36),"
            "client_id VARCHAR(128),"
            "gc_id CHAR(36),"
            "UNIQUE INDEX(repo_id, client_id)"
        ") ENGINE=INNODB",
    .create_table_repo_trash =
        "CREATE TABLE IF NOT EXISTS RepoTrash ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(36),"
            "repo_name VARCHAR(255),"
            "head_id CHAR(40),"
            "owner_id VARCHAR(255),"
            "size BIGINT(20),"
            "org_id INTEGER,"
            "del_time BIGINT,"
            "UNIQUE INDEX(repo_id),"
            "INDEX(owner_id),"
            "INDEX(org_id)"
        ") ENGINE=INNODB",
    .create_table_repo_file_count =
        "CREATE TABLE IF NOT EXISTS RepoFileCount ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(36),"
            "file_count BIGINT UNSIGNED,"
            "UNIQUE INDEX(repo_id)"
        ") ENGINE=INNODB",
    .create_table_repo_info =
        "CREATE TABLE IF NOT EXISTS RepoInfo ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(36),"
            "name VARCHAR(255) NOT NULL,"
            "update_time BIGINT,"
            "version INTEGER,"
            "is_encrypted INTEGER,"
            "last_modifier VARCHAR(255),"
            "status INTEGER DEFAULT 0,"
            "type VARCHAR(10),"
            "UNIQUE INDEX(repo_id),"
            "INDEX(type)"
        ") ENGINE=INNODB",
    .create_table_webupload_temp_files =
        "CREATE TABLE IF NOT EXISTS WebUploadTempFiles ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(40) NOT NULL,"
            "file_path TEXT NOT NULL,"
            "tmp_file_path TEXT NOT NULL,"
            "INDEX(repo_id)"
        ") ENGINE=INNODB",
};

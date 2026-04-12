#include "common.h"
#include "seaf-queries.h"


SeafDBQueries queries_pgsql = {
    .user_count = "SELECT COUNT(id) FROM EmailUser WHERE is_active = true",
    .inactive_user_count = "SELECT COUNT(id) FROM EmailUser WHERE is_active = false",
    .ldap_user_count = "SELECT COUNT(id) FROM LDAPUsers WHERE is_active = true",
    .ldap_inactive_user_count = "SELECT COUNT(id) FROM LDAPUsers WHERE is_active = false",
    .get_superusers = "SELECT t1.id, t1.email, "
        "t1.is_staff, t1.is_active, t1.ctime, "
        "t2.role, t1.passwd FROM EmailUser t1 "
        "LEFT JOIN UserRole t2 "
        "ON t1.email = t2.email "
        "WHERE is_staff = true AND t1.email NOT LIKE '%%@seafile_group'",
    .get_ldap_superusers = "SELECT t1.id, t1.email, "
        "t1.is_staff, t1.is_active, "
        "t2.role FROM LDAPUsers t1 "
        "LEFT JOIN \"userrole\" t2 "
        "ON t1.email = t2.email "
        "WHERE is_staff = true",

    .get_branch_commit_id_for_update = "SELECT commit_id FROM Branch WHERE name=? "
        "AND repo_id=? FOR UPDATE",
    .replace_branch = "INSERT INTO Branch (name, repo_id, commit_id) VALUES (?, ?, ?) "
        "ON CONFLICT (repo_id, name) DO UPDATE SET commit_id = EXCLUDED.commit_id",

    .add_group_member = "INSERT INTO \"groupuser\" (group_id, user_name, is_staff) VALUES (?, ?, ?)",
    .remove_group_member = "DELETE FROM \"groupuser\" WHERE group_id=? AND user_name=?",
    .set_group_admin = "UPDATE \"groupuser\" SET is_staff = 1 WHERE group_id = ? and user_name = ?",
    .unset_group_admin = "UPDATE \"groupuser\" SET is_staff = 0 WHERE group_id = ? and user_name = ?",

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

    .create_table_group =
        "CREATE TABLE IF NOT EXISTS \"group\" ("
            "\"group_id\" BIGSERIAL PRIMARY KEY,"
            "\"group_name\" VARCHAR(255),"
            "\"creator_name\" VARCHAR(255),"
            "\"timestamp\" BIGINT,"
            "\"type\" VARCHAR(32),"
            "\"parent_group_id\" INTEGER"
        ");",
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

    .create_table_organization =
        "CREATE TABLE IF NOT EXISTS Organization ("
            "org_id BIGSERIAL PRIMARY KEY,"
            "org_name VARCHAR(255),"
            "url_prefix VARCHAR(255),"
            "creator VARCHAR(255),"
            "ctime BIGINT,"
            "UNIQUE (url_prefix)"
        ");",
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
    .create_table_org_group =
        "CREATE TABLE IF NOT EXISTS OrgGroup ("
            "id BIGSERIAL PRIMARY KEY,"
            "org_id INTEGER,"
            "group_id INTEGER,"
            "UNIQUE (org_id, group_id)"
        ");"
        "CREATE INDEX IF NOT EXISTS orggroup_group_id_idx"
            " ON OrgGroup (group_id);",

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
};

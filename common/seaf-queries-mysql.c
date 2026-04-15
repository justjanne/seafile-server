#include "common.h"
#include "seaf-queries.h"


SeafDBQueries queries_mysql = {
    .user_count =
        "SELECT COUNT(id)"
        " FROM EmailUser"
        " WHERE is_active = 1;",
    .inactive_user_count =
        "SELECT COUNT(id)"
        " FROM EmailUser"
        " WHERE is_active = 0;",
    .get_superusers =
        "SELECT t1.id, t1.email, t1.is_staff, t1.is_active, t1.ctime, t2.role, t1.passwd"
        " FROM EmailUser t1 LEFT JOIN UserRole t2 ON t1.email = t2.email"
        " WHERE is_staff = 1 AND t1.email NOT LIKE '%%@seafile_group';",

    .ldap_user_count =
        "SELECT COUNT(id)"
        " FROM LDAPUsers"
        " WHERE is_active = 1;",
    .ldap_inactive_user_count =
        "SELECT COUNT(id)"
        " FROM LDAPUsers"
        " WHERE is_active = 0;",
    .get_ldap_superusers =
        "SELECT t1.id, t1.email, t1.is_staff, t1.is_active, t2.role"
        " FROM LDAPUsers t1 LEFT JOIN UserRole t2 ON t1.email = t2.email"
        " WHERE is_staff = 1;",

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
        ") ENGINE=INNODB;",
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
        ") ENGINE=INNODB;",
    .create_table_inner_pub_repo =
        "CREATE TABLE IF NOT EXISTS InnerPubRepo ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "permission CHAR(15),"
            "UNIQUE INDEX (repo_id)"
        ") ENGINE=INNODB;",
    .create_table_shared_repo =
        "CREATE TABLE IF NOT EXISTS `SharedRepo` ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "from_email VARCHAR(255),"
            "to_email VARCHAR(255),"
            "permission CHAR(15),"
            "INDEX (repo_id),"
            "INDEX(from_email),"
            "INDEX(to_email)"
        ") ENGINE=INNODB;",

    .create_table_org_repo =
        "CREATE TABLE IF NOT EXISTS OrgRepo ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "org_id INTEGER,"
            "repo_id CHAR(37),"
            "user VARCHAR(255),"
            "UNIQUE INDEX(org_id, repo_id),"
            "UNIQUE INDEX (repo_id),"
            "INDEX (org_id, user),"
            "INDEX(user)"
        ") ENGINE=INNODB;",
    .create_table_org_repo_group =
        "CREATE TABLE IF NOT EXISTS OrgGroupRepo ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "org_id INTEGER,"
            "repo_id CHAR(37),"
            "group_id INTEGER,"
            "owner VARCHAR(255),"
            "permission CHAR(15),"
            "UNIQUE INDEX(org_id, group_id, repo_id),"
            "INDEX (repo_id), INDEX (owner)"
        ") ENGINE=INNODB;",
    .create_table_org_inner_pub_repo =
        "CREATE TABLE IF NOT EXISTS OrgInnerPubRepo ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "org_id INTEGER,"
            "repo_id CHAR(37),"
            "UNIQUE INDEX(org_id, repo_id),"
            "permission CHAR(15)"
        ") ENGINE=INNODB;",
    .create_table_org_shared_repo =
        "CREATE TABLE IF NOT EXISTS OrgSharedRepo ("
            "id INTEGER NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "org_id INT,"
            "repo_id CHAR(37) ,"
            "from_email VARCHAR(255),"
            "to_email VARCHAR(255),"
            "permission CHAR(15),"
            "INDEX(repo_id),"
            "INDEX (org_id, repo_id),"
            "INDEX(from_email), INDEX(to_email)"
        ") ENGINE=INNODB;",

    .create_table_repo_user_token =
        "CREATE TABLE IF NOT EXISTS RepoUserToken ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "email VARCHAR(255),"
            "token CHAR(41),"
            "UNIQUE INDEX(repo_id, token),"
            "INDEX(token),"
            "INDEX (email)"
        ") ENGINE=INNODB;",
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
        ") ENGINE=INNODB;",
    .create_table_repo_head =
        "CREATE TABLE IF NOT EXISTS RepoHead ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "branch_name VARCHAR(10),"
            "UNIQUE INDEX(repo_id)"
        ") ENGINE=INNODB;",
    .create_table_repo_size =
        "CREATE TABLE IF NOT EXISTS RepoSize ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "size BIGINT UNSIGNED,"
            "head_id CHAR(41),"
            "UNIQUE INDEX (repo_id)"
        ") ENGINE=INNODB;",
    .create_table_repo_history_limit =
        "CREATE TABLE IF NOT EXISTS RepoHistoryLimit ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "days INTEGER,"
            "UNIQUE INDEX(repo_id)"
        ") ENGINE=INNODB;",
    .create_table_repo_valid_since =
        "CREATE TABLE IF NOT EXISTS RepoValidSince ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "timestamp BIGINT,"
            "UNIQUE INDEX(repo_id)"
        ") ENGINE=INNODB;",
    .create_table_web_ap =
        "CREATE TABLE IF NOT EXISTS WebAP ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(37),"
            "access_property CHAR(10),"
            "UNIQUE INDEX(repo_id)"
        ") ENGINE=INNODB;",
    .create_table_virtual_repo =
        "CREATE TABLE IF NOT EXISTS VirtualRepo ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(36),"
            "origin_repo CHAR(36),"
            "path TEXT,"
            "base_commit CHAR(40),"
            "UNIQUE INDEX(repo_id),"
            "INDEX(origin_repo)"
        ") ENGINE=INNODB;",
    .create_table_garbage_repos =
        "CREATE TABLE IF NOT EXISTS GarbageRepos ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(36),"
            "UNIQUE INDEX(repo_id)"
        ");",
    .create_table_gcid =
        "CREATE TABLE IF NOT EXISTS GCID ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(36),"
            "gc_id CHAR(36"
            "UNIQUE INDEX(repo_id)"
        ") ENGINE=INNODB;",
    .create_table_last_gcid =
        "CREATE TABLE IF NOT EXISTS LastGCID ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(36),"
            "client_id VARCHAR(128),"
            "gc_id CHAR(36),"
            "UNIQUE INDEX(repo_id, client_id)"
        ") ENGINE=INNODB;",
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
        ") ENGINE=INNODB;",
    .create_table_repo_file_count =
        "CREATE TABLE IF NOT EXISTS RepoFileCount ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(36),"
            "file_count BIGINT UNSIGNED,"
            "UNIQUE INDEX(repo_id)"
        ") ENGINE=INNODB;",
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
        ") ENGINE=INNODB;",
    .create_table_webupload_temp_files =
        "CREATE TABLE IF NOT EXISTS WebUploadTempFiles ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(40) NOT NULL,"
            "file_path TEXT NOT NULL,"
            "tmp_file_path TEXT NOT NULL,"
            "INDEX(repo_id)"
        ") ENGINE=INNODB;",

    .create_table_user_quota =
        "CREATE TABLE IF NOT EXISTS `UserQuota` ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "user VARCHAR(255),"
            "quota BIGINT,"
            "UNIQUE INDEX(user)"
        ") ENGINE=INNODB;",
    .create_table_user_share_quota =
        "CREATE TABLE IF NOT EXISTS `UserShareQuota` ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "user VARCHAR(255),"
            "quota BIGINT,"
            "UNIQUE INDEX(user)"
        ") ENGINE=INNODB;",
    .create_table_org_quota =
        "CREATE TABLE IF NOT EXISTS `OrgQuota` ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "org_id INTEGER,"
            "quota BIGINT,"
            "UNIQUE INDEX(org_id)"
        ") ENGINE=INNODB;",
    .create_table_org_user_quota =
        "CREATE TABLE IF NOT EXISTS `OrgUserQuota` ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "org_id INTEGER,"
            "user VARCHAR(255),"
            "quota BIGINT,"
            "UNIQUE INDEX(org_id, user)"
        ") ENGINE=INNODB;",

    .create_table_system_info =
        "CREATE TABLE IF NOT EXISTS `SystemInfo` ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "info_key VARCHAR(256),"
            "info_value VARCHAR(1024)"
        ");",

    .create_table_seafile_conf =
        "CREATE TABLE IF NOT EXISTS `SeafileConf` ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "cfg_group VARCHAR(255) NOT NULL,"
            "cfg_key VARCHAR(255) NOT NULL,"
            "value VARCHAR(255),"
            "property INTEGER"
        ") ENGINE=INNODB;",
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
        "CREATE TABLE IF NOT EXISTS `Group` ("
            "`group_id` BIGINT PRIMARY KEY AUTO_INCREMENT,"
            "`group_name` VARCHAR(255),"
            "`creator_name` VARCHAR(255),"
            "`timestamp` BIGINT,"
            "`type` VARCHAR(32),"
            "`parent_group_id` INTEGER"
        ") ENGINE=INNODB;",
    .insert_group =
        "INSERT INTO `Group` (group_name, creator_name, timestamp, parent_group_id)"
        " VALUES(?, ?, ?, ?);",
    .get_group =
        "SELECT group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM `Group`"
        " WHERE group_id = ?;",
    .get_group_group_id =
        "SELECT group_id"
        " FROM `Group`"
        " WHERE group_name = ? AND creator_name = ? AND timestamp = ?;",
    .get_group_exists =
        "SELECT group_id"
        " FROM `Group`"
        " WHERE group_id=?;",
    .get_group_has_children =
        "SELECT 1"
        " FROM `Group`"
        " WHERE parent_group_id=?;",
    .list_group =
        "SELECT `group_id`, `group_name`, `creator_name`, `timestamp`, `parent_group_id`"
        " FROM `Group`"
        " ORDER BY timestamp DESC;",
    .list_group_paginated =
        "SELECT `group_id`, `group_name`, `creator_name`, `timestamp`, `parent_group_id`"
        " FROM `Group`"
        " ORDER BY timestamp DESC"
        " LIMIT ? OFFSET ?;",
    .list_group_by_path =
        "SELECT g.group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM `Group` g"
        " WHERE g.group_id IN(%s)"
        " ORDER BY g.group_id;",
    .list_group_by_user =
        "SELECT g.group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM `Group` g, GroupUser u"
        " WHERE g.group_id = u.group_id AND user_name=?"
        " ORDER BY g.group_id DESC;",
    .list_group_by_parent =
        "SELECT group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM `Group`"
        " WHERE parent_group_id=?;",
    .list_group_by_ancestor =
        "SELECT g.group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM `Group` g, GroupStructure s"
        " WHERE g.group_id=s.group_id AND (s.path LIKE ? OR s.path LIKE ? OR g.group_id=?);",
    .list_group_root =
        "SELECT group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM `Group`"
        " WHERE parent_group_id=-1"
        " ORDER BY timestamp DESC;",
    .list_group_root_without_org =
        "SELECT g.group_id, g.group_name, g.creator_name, g.timestamp, g.parent_group_id"
        " FROM `Group` g LEFT JOIN OrgGroup o ON g.group_id = o.group_id"
        " WHERE g.parent_group_id=-1 AND o.group_id is NULL"
        " ORDER BY timestamp DESC;",
    .list_group_departments =
        "SELECT `group_id`, `group_name`, `creator_name`, `timestamp`, `type`, `parent_group_id`"
        " FROM `Group`"
        " WHERE parent_group_id = -1 OR parent_group_id > 0"
        " ORDER BY group_id;",
    .list_group_name_like =
        "SELECT group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM `Group`"
        " WHERE group_name LIKE ?;",
    .list_group_name_like_paginated =
        "SELECT group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM `Group`"
        " WHERE group_name LIKE ?"
        " LIMIT ? OFFSET ?;",
    .list_group_by_id =
        "SELECT g.group_id, group_name, creator_name, timestamp, parent_group_id"
        " FROM `Group` g"
        " WHERE g.group_id IN (%s)"
        " ORDER BY g.group_id DESC;",
    .update_group_name =
        "UPDATE `Group` SET group_name = ?"
        " WHERE group_id = ?;",
    .update_group_creator =
        "UPDATE `Group` SET creator_name = ?"
        " WHERE group_id = ?;",
    .delete_group =
        "DELETE FROM `Group`"
        " WHERE group_id=?;",

    .create_table_group_user =
        "CREATE TABLE IF NOT EXISTS `GroupUser` ("
            "`id` BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "`group_id` BIGINT,"
            "`user_name` VARCHAR(255),"
            "`is_staff` tinyint,"
            "UNIQUE INDEX (`group_id`, `user_name`),"
            "INDEX (`user_name`)"
        ") ENGINE=INNODB;",
    .insert_group_user =
        "INSERT INTO GroupUser (group_id, user_name, is_staff)"
        " VALUES (?, ?, ?);",
    .get_group_user_is_staff =
        "SELECT group_id FROM GroupUser"
        " WHERE group_id = ? AND user_name = ? AND is_staff = 1;",
    .get_group_user_group_id =
        "SELECT group_id FROM GroupUser"
        " WHERE group_id IN (%s) AND user_name = ? AND is_staff = 1;",
    .list_group_user =
        "SELECT group_id, user_name, is_staff"
        " FROM GroupUser"
        " WHERE group_id = ?;",
    .list_group_user_paginated =
        "SELECT group_id, user_name, is_staff"
        " FROM GroupUser"
        " WHERE group_id = ?"
        " LIMIT ? OFFSET ?;",
    .update_group_user_is_staff_true =
        "UPDATE GroupUser SET is_staff = 1"
        " WHERE group_id = ? and user_name = ?;",
    .update_group_user_is_staff_false =
        "UPDATE GroupUser SET is_staff = 0"
        " WHERE group_id = ? and user_name = ?;",
    .delete_group_user =
        "DELETE FROM GroupUser"
        " WHERE group_id=? AND user_name=?;",
    .delete_group_user_by_group =
        "DELETE FROM GroupUser"
        " WHERE group_id=?;",

    .create_table_group_dn_pair =
        "CREATE TABLE IF NOT EXISTS GroupDNPair ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "group_id INTEGER,"
            "dn VARCHAR(255)"
        ") ENGINE=INNODB;",
    .create_table_group_structure =
        "CREATE TABLE IF NOT EXISTS GroupStructure ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "group_id INTEGER,"
            "path VARCHAR(1024),"
            "UNIQUE INDEX(group_id)"
        ") ENGINE=INNODB;",
    .insert_group_structure =
        "INSERT INTO GroupStructure (group_id, path)"
        " VALUES (?, ?);",
    .get_group_structure_path =
        "SELECT path FROM GroupStructure"
        " WHERE group_id=?;",
    .list_group_structure_path =
        "SELECT path"
        " FROM GroupStructure"
        " WHERE group_id IN (%s);",
    .delete_group_structure_by_group =
        "DELETE FROM GroupStructure"
        " WHERE group_id=?;",

    .create_table_organization =
        "CREATE TABLE IF NOT EXISTS Organization ("
            "org_id BIGINT PRIMARY KEY AUTO_INCREMENT,"
            "org_name VARCHAR(255),"
            "url_prefix VARCHAR(255),"
            "creator VARCHAR(255),"
            "ctime BIGINT,"
            "UNIQUE INDEX (url_prefix)"
        ") ENGINE=INNODB;",
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
        "UPDATE `Organization` set org_name = ?"
        " WHERE org_id = ?;",
    .delete_organization =
        "DELETE FROM Organization"
        " WHERE org_id = ?;",

    .create_table_org_user =
        "CREATE TABLE IF NOT EXISTS OrgUser ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "org_id INTEGER,"
            "email VARCHAR(255),"
            "is_staff BOOL NOT NULL,"
            "INDEX (email),"
            "UNIQUE INDEX(org_id, email)"
        ") ENGINE=INNODB;",
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
        "UPDATE OrgUser SET is_staff = 1"
        " WHERE org_id=? AND email=?;",
    .update_org_user_is_staff_false =
        "UPDATE OrgUser SET is_staff = 0"
        " WHERE org_id=? AND email=?;",
    .delete_org_user =
        "DELETE FROM OrgUser"
        " WHERE org_id=? AND email=?;",
    .delete_org_user_by_org =
        "DELETE FROM OrgUser"
        " WHERE org_id = ?;",

    .create_table_org_group =
        "CREATE TABLE IF NOT EXISTS OrgGroup ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "org_id INTEGER,"
            "group_id INTEGER,"
            "INDEX (group_id),"
            "UNIQUE INDEX(org_id, group_id)"
        ") ENGINE=INNODB;",
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
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "email VARCHAR(255),"
            "passwd VARCHAR(256),"
            "is_staff BOOL NOT NULL,"
            "is_active BOOL NOT NULL,"
            "ctime BIGINT,"
            "reference_id VARCHAR(255),"
            "UNIQUE INDEX (email),"
            "UNIQUE INDEX (reference_id)"
        ") ENGINE=INNODB;",
    .create_table_binding =
        "CREATE TABLE IF NOT EXISTS Binding ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "email VARCHAR(255),"
            "peer_id CHAR(41),"
            "UNIQUE INDEX (peer_id),"
            "INDEX (email(20))"
        ") ENGINE=INNODB;",
    .create_table_user_role =
        "CREATE TABLE IF NOT EXISTS UserRole ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "email VARCHAR(255),"
            "role VARCHAR(255),"
            "UNIQUE INDEX (email)"
        ") ENGINE=INNODB;",
    .create_table_folder_user_perm =
        "CREATE TABLE IF NOT EXISTS FolderUserPerm ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "repo_id CHAR(36) NOT NULL,"
            "path TEXT NOT NULL,"
            "permission CHAR(15),"
            "user VARCHAR(255) NOT NULL,"
            "INDEX(repo_id)"
        ") ENGINE=INNODB;",
    .create_table_ldap_users =
        "CREATE TABLE IF NOT EXISTS LDAPUsers ("
            "id BIGINT PRIMARY KEY AUTO_INCREMENT,"
            "email VARCHAR(255) NOT NULL,"
            "password varchar(255) NOT NULL,"
            "is_staff BOOL NOT NULL,"
            "is_active BOOL NOT NULL,"
            "extra_attrs TEXT,"
            "reference_id VARCHAR(255),"
            "UNIQUE INDEX(email),"
            "UNIQUE INDEX(reference_id)"
        ") ENGINE=INNODB;",
    .create_table_ldap_config =
        "CREATE TABLE IF NOT EXISTS LDAPConfig ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "cfg_group VARCHAR(255) NOT NULL,"
            "cfg_key VARCHAR(255) NOT NULL,"
            "value VARCHAR(255),"
            "property INTEGER"
        ") ENGINE=INNODB;",

    .create_table_branch =
        "CREATE TABLE IF NOT EXISTS Branch ("
            "id BIGINT NOT NULL PRIMARY KEY AUTO_INCREMENT,"
            "name VARCHAR(10),"
            "repo_id CHAR(41),"
            "commit_id CHAR(41),"
            "UNIQUE INDEX(repo_id, name)"
        ") ENGINE=INNODB;",
    .upsert_branch =
        "REPLACE INTO Branch (name, repo_id, commit_id)"
        " VALUES (?, ?, ?);",
    .delete_branch =
        "DELETE FROM Branch"
        " WHERE name=? AND repo_id=?;",
    .update_branch =
        "UPDATE Branch SET commit_id = ?"
        " WHERE name = ? AND repo_id = ?;",
    .get_branch_commit_id_for_update =
        "SELECT commit_id"
        " FROM Branch"
        " WHERE name=? AND repo_id=?"
        " FOR UPDATE;",
    .get_branch_commit_id =
        "SELECT commit_id"
        " FROM Branch"
        " WHERE name=? AND repo_id=?;",
    .get_branch_name =
        "SELECT name"
        " FROM Branch"
        " WHERE name=? AND repo_id=?;",
    .get_branch =
        "SELECT name, repo_id, commit_id"
        " FROM Branch"
        " WHERE repo_id=?;",

    .user_manager_update_id_repo_owner_id =
        "UPDATE RepoOwner SET owner_id=? WHERE owner_id=?",
    .user_manager_update_id_shared_repo_from =
        "UPDATE SharedRepo SET from_email=? WHERE from_email=?",
    .user_manager_update_id_shared_repo_to =
        "UPDATE SharedRepo SET to_email=? WHERE to_email=?",
    .user_manager_update_id_repo_user_token_email =
        "UPDATE RepoUserToken SET email=? WHERE email=?",
    .user_manager_update_id_folder_user_perm_user  =
        "UPDATE FolderUserPerm SET user=? WHERE user=?",
    .user_manager_update_id_email_user_email =
        "UPDATE EmailUser SET email=? WHERE email=?",
    .user_manager_update_id_user_quota_user =
        "UPDATE UserQuota SET user=? WHERE user=?",
};

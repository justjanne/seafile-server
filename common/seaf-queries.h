#ifndef SEAF_QUERIES_H
#define SEAF_QUERIES_H

struct SeafDBQueries {
    const char *user_count;
    const char *inactive_user_count;
    const char *ldap_user_count;
    const char *ldap_inactive_user_count;
    const char *get_superusers;
    const char *get_ldap_superusers;

    const char *add_group_member;
    const char *remove_group_member;
    const char *set_group_admin;
    const char *unset_group_admin;

    const char *create_table_repo;
    const char *create_table_repo_owner;
    const char *create_table_repo_group;
    const char *create_table_inner_pub_repo;
    const char *create_table_repo_user_token;
    const char *create_table_repo_token_peer_info;
    const char *create_table_repo_head;
    const char *create_table_repo_size;
    const char *create_table_repo_history_limit;
    const char *create_table_repo_valid_since;
    const char *create_table_web_ap;
    const char *create_table_virtual_repo;
    const char *create_table_garbage_repos;
    const char *create_table_gcid;
    const char *create_table_last_gcid;
    const char *create_table_repo_trash;
    const char *create_table_repo_file_count;
    const char *create_table_repo_info;
    const char *create_table_webupload_temp_files;

    const char *create_table_user_quota;
    const char *create_table_user_share_quota;
    const char *create_table_org_quota;
    const char *create_table_org_user_quota;

    const char *create_table_system_info;
    const char *create_table_shared_repo;
    const char *create_table_seafile_conf;

    const char *create_table_group;
    const char *create_table_group_user;
    const char *create_table_group_dn_pair;
    const char *create_table_group_structure;

    const char *create_table_organization;
    const char *create_table_org_user;
    const char *create_table_org_group;

    const char *create_table_email_user;
    const char *create_table_binding;
    const char *create_table_user_role;
    const char *create_table_ldap_users;
    const char *create_table_ldap_config;

    const char *create_table_branch;
    const char *upsert_branch;
    const char *delete_branch;
    const char *update_branch;
    const char *get_branch_commit_id_for_update;
    const char *get_branch_commit_id;
    const char *get_branch_name;
    const char *get_branch;
};
typedef const struct SeafDBQueries SeafDBQueries;

#endif
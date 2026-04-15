#pragma once

struct SeafDBQueries {
    const char *user_count;
    const char *inactive_user_count;
    const char *get_superusers;

    const char *ldap_user_count;
    const char *ldap_inactive_user_count;
    const char *get_ldap_superusers;

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
    const char *get_seafile_conf_exists;
    const char *get_seafile_conf_value;
    const char *insert_seafile_conf;
    const char *update_seafile_conf;

    const char *create_table_group;
    const char *insert_group;
    const char *get_group;
    const char *get_group_group_id;
    const char *get_group_exists;
    const char *get_group_has_children;
    const char *list_group;
    const char *list_group_paginated;
    const char *list_group_by_path;
    const char *list_group_by_user;
    const char *list_group_by_parent;
    const char *list_group_by_ancestor;
    const char *list_group_root;
    const char *list_group_root_without_org;
    const char *list_group_departments;
    const char *list_group_name_like;
    const char *list_group_name_like_paginated;
    const char *list_group_by_id;
    const char *update_group_name;
    const char *update_group_creator;
    const char *delete_group;

    const char *create_table_group_user;
    const char *insert_group_user;
    const char *get_group_user_is_staff;
    const char *get_group_user_group_id;
    const char *list_group_user;
    const char *list_group_user_paginated;
    const char *update_group_user_is_staff_true;
    const char *update_group_user_is_staff_false;
    const char *delete_group_user;
    const char *delete_group_user_by_group;

    const char *create_table_group_dn_pair;
    const char *create_table_group_structure;
    const char *insert_group_structure;
    const char *get_group_structure_path;
    const char *list_group_structure_path;
    const char *delete_group_structure_by_group;

    const char *create_table_organization;
    const char *insert_organization;
    const char *list_organization;
    const char *list_organization_paginated;
    const char *count_organization;
    const char *get_organization;
    const char *get_organization_by_url_prefix;
    const char *get_organization_org_id;
    const char *get_organization_url_prefix;
    const char *update_organization_org_name;
    const char *delete_organization;

    const char *create_table_org_user;
    const char *insert_org_user;
    const char *get_org_user_by_email;
    const char *list_org_user;
    const char *list_org_user_paginated;
    const char *get_org_user_exists;
    const char *get_org_user_is_staff;
    const char *update_org_user_is_staff_true;
    const char *update_org_user_is_staff_false;
    const char *delete_org_user;
    const char *delete_org_user_by_org;

    const char *create_table_org_group;
    const char *insert_org_group;
    const char *get_org_group_exists;
    const char *get_org_group_org_id;
    const char *list_org_group_id;
    const char *list_org_group_id_paginated;
    const char *list_org_group_parent;
    const char *list_org_group;
    const char *list_org_group_paginated;
    const char *list_org_group_membership;
    const char *delete_org_group;
    const char *delete_org_group_by_org;

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

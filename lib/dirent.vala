namespace Seafile {
    public class Dirent : Object {
        public string obj_id { set; get; }

        public string obj_name { set; get; }

        public int mode { set; get; }

        public int version { set; get; }
        public int64 mtime { set; get; }
        public int64 size { set; get; }
        public string modifier { set; get;}

        public string permission { set; get; }

        public bool is_locked { set; get; }
        public string lock_owner { set; get; }
        public int64 lock_time { set; get; }

        public bool is_shared { set; get; }
    }

    public class FileLastModifiedInfo : Object {
        public string file_name { set; get; }
        public int64 last_modified { set; get; }
    }
}

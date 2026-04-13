#pragma once

#ifdef HAVE_EVHTP
int
upload_file_init (evhtp_t *evhtp, const char *http_temp_dir);
#endif

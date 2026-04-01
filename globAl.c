#include "globAl.h"

#ifdef UNICODE
  #define __CHAR WCHAR
#else
  #define __CHAR char
#endif

#define COMBINE_DWORDS(high, low) ( ( (unsigned long long)(high) << 32 ) | (unsigned long long)(low) )

int glob_foreach(__CHAR* location, GlobCallback callback, void* extra) {
  WIN32_FIND_DATA data;
  HANDLE hfind = FindFirstFile(location, &data);

  if (hfind == INVALID_HANDLE_VALUE) {
    int err = GetLastError();
    if (err == ERROR_FILE_NOT_FOUND)
      return 0;
    return err;
  }

  do {
    GlobMetadata metadata = {
      .attributes = data.dwFileAttributes,
      .creation_time = COMBINE_DWORDS(data.ftCreationTime.dwHighDateTime, data.ftCreationTime.dwLowDateTime),
      .last_access_time = COMBINE_DWORDS(data.ftLastAccessTime.dwHighDateTime, data.ftLastAccessTime.dwLowDateTime),
      .last_modification_time = COMBINE_DWORDS(data.ftLastWriteTime.dwHighDateTime, data.ftLastWriteTime.dwLowDateTime),
      .file_size = COMBINE_DWORDS(data.nFileSizeHigh, data.nFileSizeLow),
      .filename = strdup(data.cFileName),
      .alt_filename = strdup(data.cAlternateFileName),
    };
    int r = callback(&metadata, extra);
    if (r) {
      FindClose(hfind);
      return r;
    }
  } while (FindNextFile(hfind, &data));

  FindClose(hfind);
  return 0;
}

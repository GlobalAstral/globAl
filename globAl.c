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
    return 1;
  }

  do {
    GlobMetadata metadata = {
      data.dwFileAttributes,
      COMBINE_DWORDS(data.ftCreationTime.dwHighDateTime, data.ftCreationTime.dwLowDateTime),
      COMBINE_DWORDS(data.ftLastAccessTime.dwHighDateTime, data.ftLastAccessTime.dwLowDateTime),
      COMBINE_DWORDS(data.ftLastWriteTime.dwHighDateTime, data.ftLastWriteTime.dwLowDateTime),
      COMBINE_DWORDS(data.nFileSizeHigh, data.nFileSizeLow),
      data.cFileName,
      data.cAlternateFileName
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

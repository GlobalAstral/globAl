#pragma once

#include <windows.h>

#ifdef UNICODE
  #define __CHAR WCHAR
#else
  #define __CHAR char
#endif

typedef struct GlobMetadata {
  int attributes;
  unsigned long long creation_time;
  unsigned long long last_access_time;
  unsigned long long last_modification_time;
  size_t file_size;
  __CHAR* filename;
  __CHAR* alt_filename;
} GlobMetadata;

typedef int (*GlobCallback)(GlobMetadata*, void*);

int glob_foreach(__CHAR* location, GlobCallback callback, void* extra);

#undef __CHAR

#ifndef _XCLEANUP_H
#define _XCLEANUP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CLEANUP_MALLOC_ERR -1
#define _CLEANUP_REALLOC_ERR -2

static void _fd_cleanup(FILE *fd) {
  if (fd != NULL) {
    fclose(fd);
  }
}

static void _alloc_cleanup(void *ptr) {
  if (ptr != NULL) {
    free(ptr);
  }
}

static size_t _max_fd_cleanups = 16;
static size_t _max_alloc_cleanups = 16;

static FILE **_fd_cleanups_args;
static void **_alloc_cleanups_args;

static size_t _fd_cleanups_i = 0;
static size_t _alloc_cleanups_i = 0;

static void _cleanup() {
  size_t i = 0;
  for (i = 0; i < _fd_cleanups_i; ++i) {
    _fd_cleanup(_fd_cleanups_args[i]);
  }
  for (i = 0; i < _alloc_cleanups_i; ++i) {
    _alloc_cleanup(_alloc_cleanups_args[i]);
  }

  free(_fd_cleanups_args);
  free(_alloc_cleanups_args);
}

static void _realloc_fd_cleanups_args() {
  _max_fd_cleanups *= 2;
  _fd_cleanups_args = (FILE **)realloc(
      _fd_cleanups_args, _max_fd_cleanups * sizeof(*_fd_cleanups_args));
  if (_fd_cleanups_args == NULL) {
    perror("realloc() for '_fd_cleanups_args'");
    exit(_CLEANUP_REALLOC_ERR);
  }
}

static void _realloc_alloc_cleanups_args() {
  _max_alloc_cleanups *= 2;
  _alloc_cleanups_args =
      (void *)realloc(_alloc_cleanups_args,
                      _max_alloc_cleanups * sizeof(*_alloc_cleanups_args));
  if (_alloc_cleanups_args == NULL) {
    perror("realloc() for '_alloc_cleanups_args'");
    exit(_CLEANUP_REALLOC_ERR);
  }
}

#define _is_same_type(t1, t2) __builtin_types_compatible_p(t1, t2)

#define AUTO_CLEANUP_FD(fd)                                                    \
  if (_fd_cleanups_i >= _max_fd_cleanups) {                                    \
    _realloc_fd_cleanups_args();                                               \
  }                                                                            \
  _fd_cleanups_args[_fd_cleanups_i] = (FILE *)fd;                              \
  ++_fd_cleanups_i

#define AUTO_CLEANUP_ALLOC(ptr)                                                \
  if (_alloc_cleanups_i >= _max_alloc_cleanups) {                              \
    _realloc_alloc_cleanups_args();                                            \
  }                                                                            \
  _alloc_cleanups_args[_alloc_cleanups_i] = (void *)ptr;                       \
  ++_alloc_cleanups_i

#define AUTO_CLEANUP(desc)                                                     \
  if (_is_same_type(typeof(desc), typeof(FILE *))) {                           \
    AUTO_CLEANUP_FD(desc);                                                     \
  } else {                                                                     \
    AUTO_CLEANUP_ALLOC(desc);                                                  \
  }

#define REMOVE_CLEANUP_FD(fd)                                                  \
  for (size_t _remove_fd_cleanups_i = 0;                                       \
       _remove_fd_cleanups_i < _fd_cleanups_i; ++_remove_fd_cleanups_i) {      \
    if (_fd_cleanups_args[_remove_fd_cleanups_i] == (FILE *)fd) {              \
      if (_remove_fd_cleanups_i == _fd_cleanups_i - 1) {                       \
        --_fd_cleanups_i;                                                      \
        break;                                                                 \
      }                                                                        \
      FILE **_temp_arg;                                                        \
      size_t _temp_arg_size =                                                  \
          (_fd_cleanups_i - _remove_fd_cleanups_i - 1) * sizeof(*_temp_arg);   \
      _temp_arg = (FILE **)malloc(_temp_arg_size);                             \
      memcpy(_temp_arg, &_fd_cleanups_args[_remove_fd_cleanups_i + 1],         \
             _temp_arg_size);                                                  \
      memcpy(&_fd_cleanups_args[_remove_fd_cleanups_i], _temp_arg,             \
             _temp_arg_size);                                                  \
      free(_temp_arg);                                                         \
      --_fd_cleanups_i;                                                        \
      break;                                                                   \
    }                                                                          \
  }

#define REMOVE_CLEANUP_ALLOC(ptr)                                              \
  for (size_t _remove_alloc_cleanups_i = 0;                                    \
       _remove_alloc_cleanups_i < _alloc_cleanups_i;                           \
       ++_remove_alloc_cleanups_i) {                                           \
    if (_alloc_cleanups_args[_remove_alloc_cleanups_i] == (void *)ptr) {       \
      if (_remove_alloc_cleanups_i == _alloc_cleanups_i - 1) {                 \
        --_alloc_cleanups_i;                                                   \
        break;                                                                 \
      }                                                                        \
      void **_temp_arg;                                                        \
      size_t _temp_arg_size =                                                  \
          (_alloc_cleanups_i - _remove_alloc_cleanups_i - 1) *                 \
          sizeof(*_temp_arg);                                                  \
      _temp_arg = (void **)malloc(_temp_arg_size);                             \
      memcpy(_temp_arg, &_alloc_cleanups_args[_remove_alloc_cleanups_i + 1],   \
             _temp_arg_size);                                                  \
      memcpy(&_alloc_cleanups_args[_remove_alloc_cleanups_i], _temp_arg,       \
             _temp_arg_size);                                                  \
      free(_temp_arg);                                                         \
      --_alloc_cleanups_i;                                                     \
      break;                                                                   \
    }                                                                          \
  }

#define REMOVE_CLEANUP(desc)                                                   \
  if (_is_same_type(typeof(desc), typeof(FILE *))) {                           \
    REMOVE_CLEANUP_FD(desc);                                                   \
  } else {                                                                     \
    REMOVE_CLEANUP_ALLOC(desc);                                                \
  }

#define CLEANUP_INIT()                                                         \
  _fd_cleanups_args =                                                          \
      (FILE **)malloc(_max_fd_cleanups * sizeof(*_fd_cleanups_args));          \
  if (_fd_cleanups_args == NULL) {                                             \
    perror("malloc() for '_fd_cleanups_args'");                                \
    exit(_CLEANUP_MALLOC_ERR);                                                 \
  }                                                                            \
  _alloc_cleanups_args =                                                       \
      (void **)malloc(_max_alloc_cleanups * sizeof(*_alloc_cleanups_args));    \
  if (_alloc_cleanups_args == NULL) {                                          \
    perror("malloc() for '_alloc_cleanups_args'");                             \
    exit(_CLEANUP_MALLOC_ERR);                                                 \
  }                                                                            \
  atexit(_cleanup)

#endif // _XCLEANUP_H

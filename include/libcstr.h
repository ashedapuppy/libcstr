/// @file clib.h
#ifndef LIBCSTR_H
#define LIBCSTR_H

#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

/// Defining a new type called `string_t` that is a pointer to a struct called `string_s`.
typedef struct string_s string_t;

size_t len_str(char *str);
size_t len_string(string_t *any);
void free_string(string_t *any);
string_t *new_string_t_from_str(char *str);
string_t *new_string_t_of_len(size_t len);
string_t *concat_string_ts(string_t *a, string_t *b);
string_t *concat_string_ts_consume(string_t *a, string_t *b);
ssize_t write_string_t(int fd, string_t *any);
string_t *read_string_t(int fd);


#endif // LIBCSTR_H

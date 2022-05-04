#include <stdlib.h>
#include <unistd.h>
#include <limits.h>

#define READ_BUFFER_SIZE 1024

/**
 * A string is a structure with two fields: an integer length and a character pointer.
 * @property {int} len - The length of the string.
 * @property {char} str - The string itself.
 */
typedef struct {
	int len;
	char *str;
} string;

/**
 * It returns the length of a C string
 * 
 * @param str The string to be measured.
 * 
 * @return The length of the string.
 */
int len_str(char *str) {
	int i = 0;

	while (str[i] != '\0') {
		++i;
	}
	return i;
}

/**
 * Return the length of the string.
 * 
 * @param any The string to get the length of.
 * 
 * @return The length of the string.
 */
int len_string(string *any) {
	return any->len;
}

/**
 * Free the memory allocated for the C string and the string itself.
 * 
 * @param any The string to free.
 */
void free_string(string *any) {
	free(any->str);
	free(any);
}

/**
 * It takes a C string and returns a pointer to a string struct
 * 
 * @param str The string to be copied.
 * 
 * @return A pointer to a string.
 */
string *new_string_from(char *str) {
	int i;
	int len = len_str(str);
	string *new_str = (string *)malloc(sizeof(string));

	new_str->str = (char *)malloc(sizeof(char) * len + 1);
	for (i = 0; i < len; ++i) {
		new_str->str[i] = str[i];
	}
	new_str->str[i] = '\0';
	return new_str;
}

/**
 * It creates a new string with a given length
 * 
 * @param len The length of the string.
 * 
 * @return A pointer to a string.
 */
string *new_string_of_len(int len) {
	string *new_str = (string *)malloc(sizeof(string));

	new_str->str = (char *)malloc(sizeof(char) * len + 1);
	new_str->str[len] = '\0';
	return new_str;
}

/**
 * It takes two strings, allocates a new string of the appropriate length, copies the first string into
 * the new string, then copies the second string into the new string
 * 
 * @param a The first string to concatenate.
 * @param b the string to be appended to the end of a
 * 
 * @return A pointer to a string.
 */
string *concat_strings(string *a, string *b) {
	int i;
	int j;
	string *new_str = new_string_of_len(a->len + b->len);

	for (i = 0; i < a->len; ++i) {
		new_str->str[i] = a->str[i];
	}
	for (j = 0; j < b->len; ++j) {
		new_str->str[i++] = b->str[j];
	}
	return new_str;	
}

/**
 * It takes two strings, concatenates them, and returns the result, 
 * and frees the two original strings
 * 
 * @param a The first string to concatenate.
 * @param b the string to be appended to a
 * 
 * @return A pointer to a string.
 */
string *concat_strings_consume(string *a, string *b) {
	string *new_str = new_string_of_len(a->len + b->len);
	int i;
	int j;

	for (i = 0; i < a->len; ++i) {
		new_str->str[i] = a->str[i];
	}
	for (j = 0; j < b->len; ++j) {
		new_str->str[i++] = b->str[j];
	}
	free_string(a);
	free_string(b);
	return new_str;	
}

/**
 * It writes a string to a file descriptor
 * 
 * @param fd The file descriptor to write to.
 * @param any The string to write.
 * 
 * @return The number of bytes written. (-1 on error)
 */
ssize_t write_string(int fd, string *any) {
	return write(fd, any->str, any->len);
}

/**
 * It reads from a file descriptor until it reaches the end of the file, and returns the contents of
 * the file as a string
 * 
 * @param fd the file descriptor to read from
 * 
 * @return A string.
 */
string *read_string(int fd) {
	ssize_t bytes_read = 1;
	char* buf = (char *)malloc(READ_BUFFER_SIZE);
	string *final = new_string_from((char *)"");

	do {
		bytes_read = read(fd, buf, READ_BUFFER_SIZE);
		string *current_buf = new_string_from(buf);
		final = concat_strings_consume(final, current_buf);
		// store buf somewhere else so you can use it in the next iteration
	} while (bytes_read > 0);
	free(buf);
	return final;
}
/// @file string.c

#include "libcstr.h"

#define READ_BUFFER_SIZE 1024


/**
 * A string_s is a structure that contains an integer, a pointer to a character, and a pointer to
 * another string_s.
 * @property {int} len - The length of the string.
 * @property {char} str - The string itself.
 * @property {string_s} next - This is a pointer to the next string_s structure in the list.
 */
struct string_s {
	size_t len;
	char *str;
	struct string_s *next;
};

/**
 * It returns the length of a string
 * 
 * @param str The string to be measured.
 * 
 * @return The length of the string.
 */
size_t len_str(char *str) {
	size_t i = 0;

	while (str[i] != '\0') {
		++i;
	}
	return i;
}

/**
 * It returns the length of a string
 * 
 * @param any The string to get the length of.
 * 
 * @return The length of the string.
 */
size_t len_string(string_t *any) {
	return any->len;
}

/**
 * It frees the memory allocated for the string and then frees the memory allocated for the string_t
 * struct
 * 
 * @param any The string_t object to free.
 */
void free_string(string_t *any) {
	free(any->str);
	free(any);
}

/**
 * It takes a C string and returns a string_t
 * 
 * @param str The string to be copied.
 * 
 * @return A pointer to a string_t struct.
 */
string_t *new_string_from_str(char *str) {
	int i;
	int len = len_str(str);
	string_t *new_str = (string_t *)malloc(sizeof(string_t));

	new_str->str = (char *)malloc(sizeof(char) * len + 1);
	for (i = 0; i < len; ++i) {
		new_str->str[i] = str[i];
	}
	new_str->str[i] = '\0';
	new_str->next = NULL;
	return new_str;
}

/**
 * It allocates memory for a string_t struct, and then allocates memory for the string itself
 * 
 * @param len The length of the string.
 * 
 * @return A pointer to a string_t struct.
 */
string_t *new_string_of_len(int len) {
	string_t *new_str = (string_t *)malloc(sizeof(string_t));

	new_str->str = (char *)malloc(sizeof(char) * len + 1);
	new_str->str[len] = '\0';
	new_str->next = NULL;
	return new_str;
}

/**
 * It takes two strings, allocates a new string of the combined length of the two strings, copies the
 * characters from the first string into the new string, then copies the characters from the second
 * string into the new string, and returns the new string, also sets the next string to be that of the
 * first string
 * 
 * @param a The first string to concatenate.
 * @param b the string to be appended to a
 * 
 * @return A pointer to a string_t struct.
 */
string_t *concat_strings(string_t *a, string_t *b) {
	size_t i;
	size_t j;
	string_t *new_str = new_string_of_len(a->len + b->len);

	for (i = 0; i < a->len; ++i) {
		new_str->str[i] = a->str[i];
	}
	for (j = 0; j < b->len; ++j) {
		new_str->str[i++] = b->str[j];
	}
	new_str->next = a->next;
	return new_str;	
}

/**
 * Same as concat_strings but frees the original strings
 */
string_t *concat_strings_consume(string_t *a, string_t *b) {
	string_t *new_str = new_string_of_len(a->len + b->len);
	size_t i;
	size_t j;

	for (i = 0; i < a->len; ++i) {
		new_str->str[i] = a->str[i];
	}
	for (j = 0; j < b->len; ++j) {
		new_str->str[i++] = b->str[j];
	}
	free_string(a);
	free_string(b);
	new_str->next = a->next;
	return new_str;	
}

/**
 * It writes a string to a file descriptor
 * 
 * @param fd The file descriptor to write to.
 * @param any The string to write.
 * 
 * @return The number of bytes written.
 */
ssize_t write_string(int fd, string_t *any) {
	return write(fd, any->str, any->len);
}

/**
 * It writes the strings in a string list to a file descriptor
 * 
 * @param fd The file descriptor to write to.
 * @param any A pointer to the first string in the list.
 * 
 * @return The number of bytes written to the file descriptor.
 */
ssize_t write_string_list(int fd, string_t *any) {
	ssize_t len;
	ssize_t add;
	while (any->next != NULL) {
		len = 0;
		add = 0;
		// Writing the string to the file descriptor, and then checking if the write function
		// returned an error. If it did, it returns -1. If it didn't, it adds the number of bytes written to
		// the total number of bytes written, and then moves on to the next string in the list.
		add = write(fd, any->str, any->len);
		if (add == -1) {
			return -1;
		} else {
			len += add;
		}
		any  = any->next;
	}
	return len;
}

/**
 * It reads from a file descriptor until it reaches the end of the file, and then returns the contents
 * of the file as a string
 * 
 * @param fd The file descriptor to read from.
 * 
 * @return A string_t object.
 */
string_t *read_string(int fd) {
	ssize_t bytes_read = 1;
	char* buf = (char *)malloc(READ_BUFFER_SIZE);
	string_t *final = new_string_from_str((char *)"");

	// Reading from the file descriptor until it reaches the end of the file.
	do {
		bytes_read = read(fd, buf, READ_BUFFER_SIZE);
		// Checking if the read function returned an error. If it did, it returns NULL.
		if (bytes_read == -1) {
			return NULL;
		}
		// Adding a null terminator to the end of the string.
		buf[bytes_read] = '\0';
		// Creating a new string from the buffer, and then concatenating it to the final string.
		string_t *current_buf = new_string_from_str(buf);
		final = concat_strings_consume(final, current_buf);
	} while (bytes_read > 0);
	free(buf);
	final->next = NULL;
	return final;
}

/**
 * It takes a string, a start index, and an end index, and returns a new string that is a slice of the
 * original string
 * 
 * @param any The string to slice.
 * @param start The index of the first character to include in the slice.
 * @param end The index of the last character to be included in the slice.
 * 
 * @return A pointer to a string_t.
 */
string_t *slice_string(const string_t *any, size_t start, size_t end) {
	string_t *new_string = new_string_of_len(end - start);
	size_t new_string_index = 0;
	for (size_t i = start; i < end; ++i) {
		new_string[new_string_index] = any[i];
	}
	return new_string;
}

void add_string(string_t *a, string_t *b) {
	a->next = b;
}

// TODO 
// string_t *split_string_on(string_t *any, char c) {
// 	size_t start_index = 0;
// 	size_t end_index = 0;

// 	while (end_index < any->len) {
// 		if (any->str[end_index] == c) {
// 			string_t *new_str = slice_string(any, start_index, end_index);
// 		}
// 	}
	
// }
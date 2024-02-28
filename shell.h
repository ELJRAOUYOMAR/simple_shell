#ifndef SHELL_H
#define SHELL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <limits.h>
#include <fcntl.h>
#include <errno.h>

/* for read/write buffers */
#define SHELL_READ_BUF_SIZE 1024
#define SHELL_WRITE_BUF_SIZE 1024
#define SHELL_BUF_FLUSH -1

/* for command chaining */
#define SHELL_CMD_NORM    0
#define SHELL_CMD_OR      1
#define SHELL_CMD_AND     2
#define SHELL_CMD_CHAIN   3

/* for convert_number() */
#define SHELL_CONVERT_LOWERCASE   1
#define SHELL_CONVERT_UNSIGNED    2

/* 1 if using system getline() */
#define SHELL_USE_GETLINE 0
#define SHELL_USE_STRTOK 0

#define SHELL_HIST_FILE   ".shell_history"
#define SHELL_HIST_MAX    4096

extern char **environ;

/**
 * struct liststr - singly linked list
 * @num: the number field
 * @str: a string
 * @next: points to the next node
 */
typedef struct liststr
{
    int num;
    char *str;
    struct liststr *next;
} shell_list_t;

/**
 *struct passinfo - contains pseudo-arguments to pass into a function,
 *                  allowing uniform prototype for function pointer struct
 *@arg: a string generated from getline containing arguments
 *@argv: an array of strings generated from arg
 *@path: a string path for the current command
 *@argc: the argument count
 *@line_count: the error count
 *@err_num: the error code for exit()s
 *@linecount_flag: if on count this line of input
 *@fname: the program filename
 *@env: linked list local copy of environ
 *@environ: custom modified copy of environ from LL env
 *@history: the history node
 *@alias: the alias node
 *@env_changed: on if environ was changed
 *@status: the return status of the last exec'd command
 *@cmd_buf: address of pointer to cmd_buf, on if chaining
 *@cmd_buf_type: CMD_type ||, &&, ;
 *@readfd: the fd from which to read line input
 *@histcount: the history line number count
 */
typedef struct passinfo
{
    char *arg;
    char **argv;
    char *path;
    int argc;
    unsigned int line_count;
    int err_num;
    int linecount_flag;
    char *fname;
    shell_list_t *env;
    shell_list_t *history;
    shell_list_t *alias;
    char **environ;
    int env_changed;
    int status;

    char **cmd_buf; /* pointer to cmd ; chain buffer, for memory management */
    int cmd_buf_type; /* CMD_type ||, &&, ; */
    int readfd;
    int histcount;
} shell_info_t;

#define SHELL_INFO_INIT \
{NULL, NULL, NULL, 0, 0, 0, 0, NULL, NULL, NULL, NULL, NULL, 0, 0, NULL, \
    0, 0, 0}

/**
 *struct builtin - contains a builtin string and related function
 *@type: the builtin command flag
 *@func: the function
 */
typedef struct builtin
{
    char *type;
    int (*func)(shell_info_t *);
} shell_builtin_table;

/* shell_hsh.c */
int shell_hsh(shell_info_t *info, char **av);
int shell_find_builtin(shell_info_t *info);
void shell_find_cmd(shell_info_t *info);
void shell_fork_cmd(shell_info_t *info);

/* shell_path.c */
int shell_is_cmd(shell_info_t *info, char *path);
char *shell_dup_chars(char *pathstr, int start, int stop);
char *shell_find_path(shell_info_t *info, char *pathstr, char *cmd);

/* shell_loophsh.c */
int shell_loophsh(char **);

/* shell_err_string_functions.c */
void shell_eputs(char *err_str);
int shell_eputchar(char c);
int shell_putfd(char c, int fd);
int shell_putsfd(char *str, int fd);

/* shell_string_functions.c */
int shell_strlen(char *s);
int shell_strcmp(char *s1, char *s2);
char *shell_starts_with(const char *haystack, const char *needle);
char *shell_strcat(char *dest, char *src);

/* shell_string_functions2.c */
char *shell_strcpy(char *dest, char *src);
char *shell_strdup(const char *str);
void shell_puts(char *str);
int shell_putchar(char c);

/* shell_string_functions3.c */
char *shell_strncpy(char *dest, char *src, int n);
char *shell_strncat(char *dest, char *src, int n);
char *shell_strchr(char *s, char c)

/* shell_string_functions4.c */
char **shell_strtow(char *str, char *delim)
char **shell_strtow2(char *str, char delim)

/* shell_memory_functions.c */
char *shell_memset(char *s, char b, unsigned int n);
void shell_ffree(char **pp);
void *shell_realloc(void *ptr, unsigned int old_size, unsigned int new_size);

/* shell_memory_functions2.c */
int shell_bfree(void **ptr);

/* shell_more_functions.c */
int shell_interactive(shell_info_t *info);
int shell_is_delim(char c, char *delim);
int shell_isalpha(int c);
int shell_atoi(char *s);

/* shell_more_functions2.c */
int shell_erratoi(char *s);
void shell_print_error(shell_info_t *info, char *estr);
int shell_print_d(int input, int fd);
char *shell_convert_number(long int num, int base, int flags);
void shell_remove_comments(char *buf);

/* shell_builtin_emulators.c */
int shell_myexit(shell_info_t *);
int shell_mycd(shell_info_t *);
int shell_myhelp(shell_info_t *);

/* shell_builtin_emulators2.c */
int shell_myhistory(shell_info_t *);
int shell_myalias(shell_info_t *);

/* shell_getline.c module */
ssize_t shell_get_input(shell_info_t *info);
int shell_getline(shell_info_t *info, char **ptr, size_t *length);
void shell_sigintHandler(__attribute__((unused))int sig_num)

/* shell_info.c module */
void shell_clear_info(shell_info_t *info);
void shell_set_info(shell_info_t *info, char **av);
void shell_free_info(shell_info_t *info, int all);

/* shell_env.c module */
char *shell_getenv(shell_info_t *info, const char *name);
int shell_myenv(shell_info_t *info);
int shell_mysetenv(shell_info_t *info);
int shell_myunsetenv(shell_info_t *info);
int shell_populate_env_list(shell_info_t *info);

/* shell_env2.c module */
char **shell_get_environ(shell_info_t *info);
int shell_unsetenv(shell_info_t *info, char *var);
int shell_setenv(shell_info_t *info, char *var, char *value);

/* shell_file_io_functions.c */
char *shell_get_history_file(shell_info_t *info);
int shell_write_history(shell_info_t *info);
int shell_read_history(shell_info_t *info);
int shell_build_history_list(shell_info_t *info, char *buf, int line_count);
int shell_renumber_history(shell_info_t *info);

/* shell_liststr.c module */
shell_list_t *shell_add_node(shell_list_t **head_ptr, const char *str, int num);
shell_list_t *shell_add_node_end(shell_list_t **head_ptr, const char *str, int num);
size_t shell_print_list_str(const shell_list_t *head):
int shell_delete_node_at_index(shell_list_t **head_ptr, unsigned int index);
void shell_free_list(shell_list_t **head_ptr);

/* shell_liststr2.c module */
size_t shell_list_len(const shell_list_t *head);
char **shell_list_to_strings(shell_list_t *head);
size_t shell_print_list(const shell_list_t *head);
shell_list_t *shell_node_starts_with(shell_list_t *head, char *prefix, char c);
ssize_t shell_get_node_index(shell_list_t *head, shell_list_t *node);

/* shell_chain.c */
int shell_is_chain(shell_info_t *info, char *buf, size_t *p);
void shell_check_chain(shell_info_t *info, char *buf, size_t *p, size_t i, size_t len);
int shell_replace_alias(shell_info_t *info);
int shell_replace_vars(shell_info_t *info);
int shell_replace_string(char **old, char *new);


#endif

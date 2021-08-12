#define PATH_MAX 4096

typedef struct test_config{
    int f_min_len;
    int f_max_len;
    int f_char_start;
    int f_char_range;

    char* binary_path[PATH_MAX];

    char* cmd_args;

    int timeout;
    int trial;

    int (* oracle)(char* dir_name);
}test_config_t;
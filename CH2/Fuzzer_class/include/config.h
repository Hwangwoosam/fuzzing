#ifndef CONFIG
#define CONFIG
#define PATH_MAX 4096

typedef struct input_arg{
    int f_min_len;
    int f_max_len;
    int f_char_start;
    int f_char_range;
}input_arg_t;

typedef struct run_arg{
    char binary_path[PATH_MAX];
    char** cmd_args;
    int args_num;
    int timeout;
}run_arg_t;

typedef struct test_config{
    
    input_arg_t inp_arg;
    run_arg_t run_arg;

    int trial;
    int (* oracle)(char* dir_name,int trial,int ret_code);
    
}test_config_t;

void init_config(test_config_t *);

#endif
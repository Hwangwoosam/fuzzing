#ifndef CONFIG
#define CONFIG
#define PATH_MAX 4096

#define NUM_OF_MAX 100
#define BUFFER_SIZE 4096
#define FILE_NAME_MAX 1024

typedef struct input_arg{
    int f_min_len;
    int f_max_len;
    int f_char_start;
    int f_char_range;
}input_arg_t;

typedef struct run_arg{
    char* binary_path;
    char* src_file[NUM_OF_MAX];
    
    int src_file_num;
    
    char src_dir[PATH_MAX];
    char seed_dir[PATH_MAX];
<<<<<<< HEAD
    char* seed_inp[NUM_OF_MAX];
=======
    char* seed_file_name[NUM_OF_MAX];
>>>>>>> 6136e7fd96e41ca5795b0d9602032d4e1652a254
    int seed_length[NUM_OF_MAX];
    char** cmd_args;
    
    int seed_file_num;
    int args_num;
    int timeout;
    int fuzz_type; // 0 = stdin , 1 = argument 
}run_arg_t;

typedef struct config{
    input_arg_t inp_arg;
    run_arg_t run_arg;

    int coverage;
    int trial;
    int (* oracle)(char* dir_name,int trial,int ret_code);
    
}config_t;

void init_config(config_t *);

#endif
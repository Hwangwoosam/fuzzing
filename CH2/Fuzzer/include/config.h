#ifndef CONFIG
#define CONFIG
#define PATH_MAX 4096

//contains information about input

typedef struct input_arg{
    int f_min_len;      // minimum length about random string
    int f_max_len;      // max length about random string
    int f_char_start;   // start point and range in character
    int f_char_range;
}input_arg_t;

// contains information need to run fuzzer
typedef struct run_arg{
    char* binary_path; 
    char* src_path;
    char** cmd_args;    // list of argument
    int args_num;       // number of argument
    int timeout;        
    int fuzz_type;      // 0 = stdin , 1 = argument 
}run_arg_t;



typedef struct config{
    input_arg_t inp_arg;
    run_arg_t run_arg;

    int trial;
    int (* oracle)(char* dir_name,int trial,int ret_code);
    
}config_t;

void init_config(config_t *);

#endif
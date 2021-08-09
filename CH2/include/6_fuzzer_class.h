typedef struct Fuzzer{
    int max_length;
    int min_length;
    int char_start;
    int char_range;
}fuzzer_t;

fuzzer_t* init_class(int max_length,int min_length,int char_start,int char_range);

char* fuzz(fuzzer_t* input);

void run_class(char* program, fuzzer_t* input,int num);
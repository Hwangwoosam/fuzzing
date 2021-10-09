#define PASS "PASS"
#define FAIL "FAIL"
#define UNRESOLVED "UNRESOLVED"

typedef struct result{
    char* output ;
    char* error;
    char* outcome;
    int return_code;
}result_t;

result_t* subprocess(char* program,char* str);

void delete_result(result_t* tmp); 

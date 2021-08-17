typedef struct item{
    char* key;
    char* value;
}item_t;

typedef struct item_list{
    int size;
    item_t ** list;
}item_list_t;

item_list_t* list_init();

int code_repOK(char* code);

int airport_codes_repOK(item_list_t* temp_list);

void add_item(item_list_t* i_list,char* key,char* value);

void delet_list(item_list_t* i_list);
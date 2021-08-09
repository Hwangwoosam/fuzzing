#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include "../include/3_program_checker.h"

int main(){
   item_list_t* airport_code = list_init();
   add_item(airport_code,"YVR","Vancouver");
   add_item(airport_code,"JFK","New York-JFK");
   add_item(airport_code,"CDG","Paris-Charles de Gaulle");
   add_item(airport_code,"CAI","Cairo");
   add_item(airport_code,"LED","St. Petersburg");
   add_item(airport_code,"PEK","Beijing");
   add_item(airport_code,"HND","Tokyo-Haneda");
   add_item(airport_code,"AKL","Auckland");

   assert(code_repOK("SEA"));
   airport_codes_repOK(airport_code);
   for(int i = 0; i < airport_code->size; i++){
       printf("%s : %s\n",airport_code->list[i]->key,airport_code->list[i]->value);
   }

   delet_list(airport_code);
}
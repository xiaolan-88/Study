#include <stdio.h>

#include "hashmap.h"


int main(void)
{
	hashtable_t **h = NULL;
	
	datatype_t value[] = {
		{"hello",123},
		{"hello",889},
		{"world",456},
		{"adfdfd",789},
	};

	h = create_hashtable();

	insert_data_hash(h,"hello",value[0]);
	insert_data_hash(h,"hello",value[1]);
	insert_data_hash(h,"world",value[2]);
	insert_data_hash(h,"adfdfd",value[3]);
	
	int result[5] = {0};
	int ret = search_hash_table(h,"world",result);
	if (ret != 0){
		for (int i = 0;i < ret;i++){
			printf(" %d\n",result[i]);
		}
	}

	printf_hash_table(h);
	return 0;
}

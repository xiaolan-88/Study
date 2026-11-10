
#include "hashmap.h"


#define HTABLE_LEN 7


hashtable_t **create_hashtable()
{
	int i;
	hashtable_t **h = NULL;
	h = (hashtable_t **)malloc(HTABLE_LEN * sizeof(hashtable_t *));

	memset(h,0,sizeof(HTABLE_LEN * sizeof(hashtable_t *)));

	for (i = 0;i < HTABLE_LEN;i++){
		h[i] = NULL;
	}

	return h;
}

// hashmap : key(字符串)<----> value(结构体)
void insert_data_hash(hashtable_t **h,char * key,datatype_t value)
{
	hashtable_t *temp = NULL;
	hashtable_t **p = NULL;

	int index = 0,i = 0;
	int code = 0;	

	code = hashcode(key);

	index = code % HTABLE_LEN;
	
	for (p = &h[index];*p != NULL;p = &((*p)->next)){
		if ((*p)->data.pid > value.pid)
			break;
	}

	temp = (hashtable_t *)malloc(sizeof(hashtable_t));
	temp->data = value;
	temp->next = *p;
	*p = temp;
	return;
}

void printf_hash_table(hashtable_t **h)
{
	int i = 0;
	hashtable_t **p = NULL;

	for (i = 0;i < HTABLE_LEN;i++){
		printf("index = %d : ",i);
		for (p = &h[i];*p != NULL;p = &((*p)->next)){
			printf("%s ",(*p)->data.topic);
		}
		putchar('\n');
	}
	return;	
}

int hashcode(const char *string)
{
	int hashcode = 0;

	while (*string){
	    hashcode += *string++;	
	}

	return hashcode;
}

int search_hash_table(hashtable_t **h,char *key,pid_t result[])
{
	int code = hashcode(key);
	int cnt = 0;
	int index = code % HTABLE_LEN;
	hashtable_t **p = NULL;
	
	for (p = &h[index];*p != NULL;p = &((*p)->next)){
		if (strcmp(key,(*p)->data.topic) == 0){
			result[cnt++] = (*p)->data.pid;
		}
	}
	return cnt;
}




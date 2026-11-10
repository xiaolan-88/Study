#ifndef __HASH_MAP_H_
#define __HASH_MAP_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// hashmap 数据类型
typedef struct datatype{
	char topic[32];
	pid_t pid;
}datatype_t;

// hashmap 链表节点
typedef struct node{
	datatype_t data;
	struct node *next;
}hashtable_t;


extern hashtable_t **create_hashtable();// 创建 hashmap

// 插入数据 : key - value
extern void insert_data_hash(hashtable_t **h,char * key,datatype_t value);

// 查询数据
extern int search_hash_table(hashtable_t **h,char * key,pid_t result[]);

// 输出 hashmap 
extern void printf_hash_table(hashtable_t **h);

// 计算 key 的 hashcode
extern int hashcode(const char *string);

#endif

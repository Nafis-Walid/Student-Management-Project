#ifndef __DATA_H__
#define __DATA_H__

#include <stdint.h>
#include "input.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX(a, b) ((a) > (b)) ? (a) : (b)

typedef struct double_list_node_t 
{
    void* data; /* as any type of data can be fit */
    struct double_list_node_t* prev;
    struct double_list_node_t* next;
}double_list_node_t;

extern bool_t Update_saved;

extern void load_data();
extern void insert_node(double_list_node_t** head, void* data, uint32_t (*get_id)(void*));
extern void delete_node(double_list_node_t** head, void* data, bool_t (*match)(void*, void*), void (*free_data)(void*));
extern void update_node(double_list_node_t* head, void* new_data, bool_t (*match)(void*, void*), void (*free_data)(void*));
extern void save_data();
extern void free_and_exit();
extern bool_t data_exists(double_list_node_t** head, const char* data_label);
extern void save();
extern void free_list(double_list_node_t** head, void(*free_data)(void*));

#endif /* __DATA_H__ */

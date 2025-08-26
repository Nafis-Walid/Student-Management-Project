#ifndef __STUDENT_H__
#define __STUDENT_H__

#include "input.h"
#include "data.h"
#include "app.h"

#define STDNT_NAME_LEN 20

extern double_list_node_t** Stdnt_head;

typedef struct 
{
    uint32_t id;
    char* name;
    char gender; /*'M' or 'F'*/
    uint32_t dept_id;
}student_t;

extern void stdnt_load();
extern student_t* stdnt_search_by_id(double_list_node_t* head, uint32_t id);
extern uint32_t get_stdnt_id(void* data);
extern void stdnt_add();
extern void stdnt_delete();
extern void stdnt_update();
extern void stdnt_display();
extern void stdnt_save();
extern void stdnt_free(void* ptr);
extern void stdnt_list_free();

#endif/*__STUDENT_H__*/
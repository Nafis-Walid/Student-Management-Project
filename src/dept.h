#ifndef __DEPT_H__
#define __DEPT_H__

#include <stdio.h>
#include "input.h"
#include "data.h"
#include "app.h"

#define DEPT_ID_LEN 11
#define DEPT_NAME_LEN 20
#define DEPT_ITEMS 2
#define FEMALE 0
#define MALE 1

typedef struct
{
    uint32_t id;
    char* name;
    uint32_t* gender_count;/*0 index female count, 1 index male count*/
}department_t;

extern double_list_node_t** Dept_head;

extern void dept_load();
extern void dept_add();
extern void dept_delete();
extern void dept_update();
extern void dept_display();
extern void dept_save();
extern bool_t dept_match(void* dept1, void* dept2);
extern uint32_t get_dept_id(void* data);
extern void* dept_search_by_id(double_list_node_t* head, uint32_t id);
extern void dept_list_free();
extern char **parse_line(char *line, unsigned char *lengths, const char* delimeter);

#endif/*__DEPT_H__*/
#ifndef __GRADE_H__
#define __GRADE_H__

#include "data.h"

#define NUM_OF_GRADES 3

extern double_list_node_t** Grade_head;

typedef struct
{
    uint32_t stdnt_id;
    uint8_t* grades;
} grade_t;
enum
{
    ENGLISH = 0,
    MATH,
    HISTORY
};

extern void grade_load();
bool_t grade_match(void* grade1, void* grade2);
grade_t* grade_search_by_id(double_list_node_t* head, uint32_t stdnt_id);
extern void grade_add();
extern void grade_delete();
extern void grade_update();
extern void grade_display();
extern void grade_save();
extern void grade_free(void* ptr);
extern void grade_list_free();

#endif /*__GRADE_H__*/

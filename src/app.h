#ifndef __APP_H__
#define __APP_H__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdint.h>
#include "input.h"

#define MENU_FIELD_LEN 50
#define NUM_MAIN_MENU 6
#define NUM_DEPT_MENU 8
#define NUM_STDNT_MENU 8
#define NUM_GRADE_MENU 8
#define NUM_SAVE_MENU 3
#define NUM_EXIT_MENU 3
#define MAIN_MENUS_ELEMENT 5

typedef struct 
{
    const char** menu_list;
    uint8_t num_menu;
}menu;

extern void (*Last_menu[4][3])(void);
extern const char *Main_menu[NUM_MAIN_MENU];
extern menu Main_menus[MAIN_MENUS_ELEMENT];

extern void main_menu();
extern void second_menu();
extern void Exit();
extern void print_border(uint32_t width);
extern void print_centered(const char* menu_header, uint32_t field_len);
extern void run_project();

#endif /* __APP_H__ */

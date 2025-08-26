#include "dept.h"
#include "student.h"
#include <ctype.h>  // For isdigit(), isalpha(), toupper()
#include <stdint.h> // For uint16_t
#include <stdlib.h> // For atoi()
#include <string.h>
#include <stdio.h>

void* dept_search_by_name(double_list_node_t* head, char* name);
void dept_free(void* ptr);
department_t* dept_alloc();

char* Department_file = "database/department-table.txt"; 
double_list_node_t** Dept_head;
static uint32_t Max_dept_id;
uint32_t Selection = 0;
uint8_t dept_item_lengths[DEPT_ITEMS + 1] = {DEPT_ID_LEN, DEPT_NAME_LEN, 0};

department_t* dept_parse(char* line)
{
    char** items = NULL;
    department_t* dept = NULL;

    if(!line || !*line)
        return NULL;
    items = parse_line(line, dept_item_lengths, ",");
    if(!items)
        return NULL;
    dept = dept_alloc();
    dept->id = atoi(items[0]);
    strncpy(dept->name, items[1], DEPT_NAME_LEN);
    free(items);

    return dept;
}

void dept_load()
{
    char line[INPUT_BUF_SIZE];
    department_t* dept = NULL;
    FILE *file = NULL;

    Dept_head = CALLOC(1, double_list_node_t*);
    if (!(file = fopen(Department_file, "r")))
    {
        printf("No Department Database Found !!!\n");
        return;
    }
    while(get_str(line, INPUT_BUF_SIZE, isprint, file))
    {
        if(!(dept = dept_parse(line)))
            printf("Could Not Parse line: \t%s\n", line);
        else
        {
            Max_dept_id = MAX(Max_dept_id, dept->id);
            insert_node(Dept_head, dept, get_dept_id);
        }
    }
    fclose(file);
    printf("Department Data Loaded from Department Database!!!\n");

    return;
}

void* dept_search_by_name(double_list_node_t* head, char* name)
{   
    department_t* curr_dept = NULL;
    
    for(double_list_node_t* curr = head; curr != NULL; curr = curr->next)
    {
        curr_dept = curr->data;
        if(!strcmp(curr_dept->name, name))
            return curr_dept;
    }

    return NULL;
}

void* dept_search_by_id(double_list_node_t* head, uint32_t id)
{   
    department_t* curr_dept = NULL;
    for(double_list_node_t* curr = head; curr != NULL; curr = curr->next)
    {
        curr_dept = curr->data;
        if(curr_dept->id == id)
            return curr_dept;
    }

    return NULL;
}

uint32_t get_dept_id(void* data)
{
    return ((department_t*)data)->id;
}

void dept_add()
{
    department_t* new_dept = NULL;
    department_t* dept_found = NULL;
    char name[DEPT_NAME_LEN] = {0};

    CLEAR_SCREEN;
    printf("ID is auto Increased\nEnter Department Name");
    printf("%d, ", Max_dept_id+1);
    while(!get_str(name, DEPT_NAME_LEN, isprint, stdin))
    {
        printf("Empty, Overflow or Unwanted characters\n");
        PRESS_ENTER_TO_TRY_AGAIN;
        return dept_add();
    }
    trim_whitespaces(name);
    if(dept_found = dept_search_by_name(*Dept_head, name))
    {
        printf("Department already exists with ID: %d\n", dept_found->id);
        PRESS_ENTER_TO_TRY_AGAIN;
        return dept_add();
    }
    new_dept->id = ++Max_dept_id;
    insert_node(Dept_head, new_dept, get_dept_id);
    Update_saved = FALSE;
    printf("Department Added Successfully\n");
    PRESS_ENTER_TO_GO_BACK;
    
    return second_menu();
}

void dept_delete()
{
    department_t* dept = dept_alloc();
    department_t* found = NULL;

    if(!data_exists(Dept_head, "Department"))
    {
        printf("Nothing to DELETE\n");
        PRESS_ENTER_TO_GO_BACK;
        return second_menu();
    }
    CLEAR_SCREEN;
    print_border(MENU_FIELD_LEN);
    print_centered("DELETE  DEPARTMENT", MENU_FIELD_LEN);
    print_border(MENU_FIELD_LEN);

    printf("Enter Department ID: ");
    if(!get_uint32(&(dept->id)) || dept->id == 0)
    {
        printf( "WRONG INPUT!!!\n"
                "Empty, Overflow or Unwanted Characters\n");
        PRESS_ENTER_TO_TRY_AGAIN;
        dept_free(dept);
        return dept_delete();
    }
    
    if(!(found = dept_search_by_id(*Dept_head, dept->id)))
    {
        printf("No department with ID: %d!!!\n", dept->id);
        PRESS_ENTER_TO_TRY_AGAIN;
        dept_free(dept);
        return dept_delete();
    }
    delete_node(Dept_head, found, dept_match, dept_free);
    if(dept->id == Max_dept_id)
        Max_dept_id--;
    
    CLEAR_SCREEN;
    printf("DEPARTMENT  DELETED  SUCCESSFULLY !!!\n");
    Update_saved = FALSE;
    for(double_list_node_t* curr = *Stdnt_head; curr != NULL; curr = curr->next) 
        if(((student_t*)curr->data)->dept_id == found->id)
            ((student_t*)curr->data)->dept_id = 0;
    PRESS_ENTER_TO_GO_BACK;
    
    return second_menu();
}

void dept_update()
{
    department_t* updated_dept = NULL;
    department_t* dept_found = NULL;
    
    if(!data_exists(Dept_head, "Department"))
    {
        printf("Nothing to UPDATE\n");
        PRESS_ENTER_TO_GO_BACK;
        return second_menu();
    }
    CLEAR_SCREEN;
    print_border(MENU_FIELD_LEN);
    print_centered("UPDATE  DEPARTMENT", MENU_FIELD_LEN);
    print_border(MENU_FIELD_LEN);
    printf("Enter Department ID: ");

    updated_dept = dept_alloc();
    if(!get_uint32(&(updated_dept->id)) || updated_dept->id == 0)
    {
        printf( "WRONG INPUT !!!\n"
                "Empty, Overflow or Unwanted Characters\n");
        PRESS_ENTER_TO_TRY_AGAIN;
        dept_free(updated_dept);
        return dept_update();
    }

    /*No Department Found*/
    if(!(dept_found = dept_search_by_id(*Dept_head, updated_dept->id)))
    {
        printf("No department with ID: %d!!!\n", updated_dept->id);
        PRESS_ENTER_TO_TRY_AGAIN;
        dept_free(updated_dept);
        return dept_update();
    }

    /*Department Found*/
    printf("Enter new Department Name for %s: ", dept_found->name);
    if(!get_str(updated_dept->name, DEPT_NAME_LEN, is_valid_char, stdin) || *(updated_dept->name) == '\0')
    {
        printf( "WRONG INPUT !!!\n"
                "Empty, Overflow or Unwanted Characters\n");
        PRESS_ENTER_TO_TRY_AGAIN;
        dept_free(updated_dept);
        return dept_update();
    }
    if(dept_found = dept_search_by_name(*Dept_head, updated_dept->name))
    {
        CLEAR_SCREEN;
        printf( "Department %s Already Exists with ID: %d\n", dept_found->name, dept_found->id);
        PRESS_ENTER_TO_TRY_AGAIN;
        dept_free(updated_dept);
        return dept_update();
    }
    
    update_node(*Dept_head, updated_dept, dept_match, dept_free);
    CLEAR_SCREEN;
    printf( "+--------------------------------------+\n"
            "|  DEPARTMENT UPDATED SUCCESSFULLY !!  |\n"
            "+---------------+----------------------+\n"
            "| Department ID | New Department Name  |\n"
            "+---------------+----------------------+\n"
            "|"" %-14d"     "|"" %-21s"            "|\n"
            "+---------------+----------------------+\n",
            updated_dept->id, updated_dept->name);
    Update_saved = FALSE;
    PRESS_ENTER_TO_GO_BACK;
    
    return second_menu();
}

void dept_display()
{
    department_t* curr_dept = NULL;

    if(!data_exists(Dept_head, "Department"))
    {
        printf("Nothing to DISPLAY\n");
        PRESS_ENTER_TO_GO_BACK;
        return second_menu();
    }
        

    CLEAR_SCREEN;
    printf(     "+--------------------------------------------------------+\n"
                "|                    DEPARTMENT Table                    |\n"
                "+---------------+----------------------+--------+--------+\n"
                "| Department ID | Department Name      | Male   | Female |\n"
                "+---------------+----------------------+--------+--------+\n");
    for(double_list_node_t* curr = *Dept_head; curr != NULL; curr = curr->next) 
    {
        curr_dept = curr->data;
        printf( "|"" %-14d"     "|"" %-21s"            "| %-7d" "| %-7d" "|\n",
                curr_dept->id, curr_dept->name, curr_dept->gender_count[MALE], curr_dept->gender_count[FEMALE]);
    }
    printf(     "+---------------+----------------------+--------+--------+\n");
    PRESS_ENTER_TO_GO_BACK;
    
    return second_menu();
}

void dept_save()
{
    department_t* dept = NULL;
    FILE* fw = NULL;

    if(Dept_head)
    {
        fw = fopen(Department_file, "w");
        if(fw == NULL)
        {
            PRINT_ERROR_AND_EXIT("Could Not Open Department File!");
        }
        for(double_list_node_t* curr = *Dept_head; curr != NULL; curr = curr->next)
        {
            dept = curr->data;
            fprintf(fw, "%d, %s\n", dept->id, dept->name);
        }
        fclose(fw);    
    }

    return;
}

bool_t dept_match(void* dept1, void* dept2)
{
    if(((department_t*)dept1)->id == ((department_t*)dept2)->id)
        return TRUE;
    else
        return FALSE;
    
    return FALSE;
}

department_t* dept_alloc()
{
    department_t* dept = CALLOC(1, department_t);
    dept->name = CALLOC(DEPT_NAME_LEN, char);
    dept->gender_count = CALLOC(2, uint32_t);

    return dept;
}

void dept_free(void* ptr)
{
    if(!ptr)
        return;
    free(((department_t*)ptr)->name);
    free(((department_t*)ptr)->gender_count);
    free(ptr);

    return;
}

void dept_list_free()
{
    free_list(Dept_head, dept_free);

    return;
}
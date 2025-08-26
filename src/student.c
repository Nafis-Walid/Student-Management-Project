#include "student.h"
#include "grade.h"
#include "dept.h"
#include <ctype.h>  // For isdigit(), isalpha(), toupper()
#include <stdint.h> // For uint16_t
#include <stdlib.h> // For atoi()
#include <string.h>

#define MAX_STDNT_ID 999
#define STDNT_ID_LEN 4
#define STDNT_NAME_LEN 20
#define STDNT_GENDER_LEN 2
#define STDNT_ITEMS 4

student_t* stdnt_alloc();
bool_t stdnt_match(void* stdnt1, void* stdnt2);
void stdnt_free(void* ptr);

char* Student_file = "database/student-table.txt"; 
double_list_node_t** Stdnt_head;
static uint32_t Selection;
uint8_t stdnt_item_lengths[STDNT_ITEMS + 1] = {STDNT_ID_LEN, STDNT_NAME_LEN, STDNT_GENDER_LEN, DEPT_ID_LEN};

student_t* stdnt_parse(char* line)
{
    char** items = NULL;
    student_t* stdnt = NULL;

    if(!line || !*line)
        return NULL;
    items = parse_line(line, stdnt_item_lengths, ",");
    if(!items)
        return NULL;
    stdnt = stdnt_alloc();
    stdnt->id = atoi(items[0]);
    strncpy(stdnt->name, items[1], STDNT_NAME_LEN);
    stdnt->gender = *items[2];
    stdnt->dept_id = atoi(items[3]);
    printf("%d, %s, %c, %d\n", stdnt->id, stdnt->name, stdnt->gender, stdnt->dept_id);
    free(items);

    return stdnt;
}

void stdnt_load()
{
    char line[INPUT_BUF_SIZE];
    student_t* stdnt = NULL;
    department_t* dept = NULL;
    FILE *file = NULL;

    Stdnt_head = CALLOC(1, double_list_node_t*);
    if (!(file = fopen(Student_file, "r")))
    {
        printf("No Student Database Found\n");
        return;
    }
    while (get_str(line, INPUT_BUF_SIZE, isprint, file))
    {
        if(!(stdnt = stdnt_parse(line)))
            printf("Could not parse line: \t%s\n", line);
        else
        {
            insert_node(Stdnt_head, stdnt, get_stdnt_id);
            if(dept = dept_search_by_id(*Dept_head, stdnt->dept_id))
            dept->gender_count[stdnt->gender%2]++;
        }
    }
    fclose(file);
    printf("Student Data Loaded from Student Database!!!\n");

    return;
}

student_t* stdnt_search_by_id(double_list_node_t* head, uint32_t id)
{
    student_t* curr_stdnt = NULL;

    for(double_list_node_t* curr = head; curr != NULL; curr = curr->next)
    {
        curr_stdnt = curr->data;
        if((curr_stdnt->id == id))
            return curr_stdnt;
    }

    return NULL;
}

uint32_t get_stdnt_id(void* data)
{
    return ((student_t*)data)->id;
}

void stdnt_add()
{
    student_t* new_stdnt = NULL;
    student_t* stdnt_found = NULL;
    department_t* dept = NULL;
    char line[INPUT_BUF_SIZE] = {0};
    char gend[3] = {'\0', 'M', 'F'};

    CLEAR_SCREEN;
    print_border(MENU_FIELD_LEN);
    print_centered("Add Student", MENU_FIELD_LEN);
    print_border(MENU_FIELD_LEN);
    printf("Enter in Below Format(comma separated)\nID(3 digital), Name(19 char max), Gender(M/F), Department Id\n");
    while(!get_str(line, INPUT_BUF_SIZE, isprint, stdin))
    {
        printf("Empty, Overflow or Unwanted characters\n");
        PRESS_ENTER_TO_TRY_AGAIN;
        return stdnt_add();
    }
    trim_whitespaces(line);
    if(!(new_stdnt = stdnt_parse(line)) || stdnt_search_by_id(*Stdnt_head, new_stdnt->id))
    {
        printf("Wrong format or student with same id exists");
        PRESS_ENTER_TO_TRY_AGAIN;
        return stdnt_add();
    }
    insert_node(Stdnt_head, new_stdnt, get_stdnt_id);
    if(dept = dept_search_by_id(*Dept_head, new_stdnt->dept_id))
        dept->gender_count[new_stdnt->gender % 2]++;
    CLEAR_SCREEN;
    printf("STUDENT  ADDED  SUCCESSFULLY !!!\n");;
    Update_saved = FALSE;
    PRESS_ENTER_TO_GO_BACK;

    return second_menu();
}

void stdnt_delete()
{
    student_t* stdnt = stdnt_alloc();
    grade_t* grade_found = NULL;
    student_t* stdnt_found = NULL;
    department_t* dept = NULL;

    if(!data_exists(Stdnt_head, "Student"))
    {
        printf("Nothing to DELETE\n");
        PRESS_ENTER_TO_GO_BACK;
        return second_menu();
    }
    CLEAR_SCREEN;
    print_border(MENU_FIELD_LEN);
    print_centered("Delete Student", MENU_FIELD_LEN);
    print_border(MENU_FIELD_LEN);

    printf("Enter Student ID: ");
    if(!get_uint32(&(stdnt->id)) || stdnt->id > MAX_STDNT_ID || stdnt->id == 0)
    {
        printf( "WRONG INPUT !!!\n"
                "Empty, Overflow or Unwanted Characters\n");
        PRESS_ENTER_TO_TRY_AGAIN;
        stdnt_free(stdnt);
        return stdnt_delete();
    }
    stdnt_found = stdnt_search_by_id(*Stdnt_head, stdnt->id);
    if(!stdnt_found)
    {
        printf("No Student with ID: %d!!!\n", stdnt->id);
        PRESS_ENTER_TO_TRY_AGAIN;
        stdnt_free(stdnt);
        return stdnt_delete();
    }
    if(dept = dept_search_by_id(*Dept_head, stdnt_found->dept_id))
        dept->gender_count[stdnt_found->gender%2]--; 
    delete_node(Stdnt_head, stdnt_found, stdnt_match, stdnt_free);

    CLEAR_SCREEN;
    printf("STUDENT  DELETED  SUCCESSFULLY !!!\n");
    Update_saved = FALSE;

    if(grade_found = grade_search_by_id(*Grade_head, stdnt_found->id))
    {
        delete_node(Grade_head, grade_found, grade_match, grade_free);
        printf("Grades of the Student is Also DELETED!\n");
    }
    PRESS_ENTER_TO_GO_BACK;
    
    return second_menu();
}

void stdnt_update()
{
    student_t* updated_stdnt = NULL;
    student_t* old_stdnt = NULL;
    department_t* dept = NULL;
    char gend[3] = {'\0', 'M', 'F'};
    
    if(!data_exists(Stdnt_head, "Student"))
    {
        printf("Nothing to UPDATE\n");
        PRESS_ENTER_TO_GO_BACK;
        return second_menu();
    }
    CLEAR_SCREEN;
    print_border(MENU_FIELD_LEN);
    print_centered("Update Student", MENU_FIELD_LEN);
    print_border(MENU_FIELD_LEN);

    printf("Enter Student ID: ");
    updated_stdnt = stdnt_alloc();
    if (!get_uint32(&(updated_stdnt->id)) || updated_stdnt->id > MAX_STDNT_ID || updated_stdnt->id == 0)
    {
        printf("WRONG INPUT !!!\n"
               "Empty, Overflow, or Unwanted Characters\n");
        PRESS_ENTER_TO_TRY_AGAIN;
        stdnt_free(updated_stdnt);
        return stdnt_update();
    }
    if (!(old_stdnt = stdnt_search_by_id(*Stdnt_head, updated_stdnt->id)))
    {
        printf("No Student with ID: %d!!!\n", updated_stdnt->id);
        PRESS_ENTER_TO_TRY_AGAIN;
        stdnt_free(updated_stdnt);
        return stdnt_update();
    }
    printf("Enter new Student Name for %s: ", old_stdnt->name);
    if (!get_str(updated_stdnt->name, STDNT_NAME_LEN, is_valid_char, stdin) || *(updated_stdnt->name) == '\0')
    {
        printf("WRONG INPUT !!!\n"
               "Empty, Overflow, or Unwanted Characters\n");
        PRESS_ENTER_TO_TRY_AGAIN;
        stdnt_free(updated_stdnt);
        return stdnt_update();
    }
    trim_whitespaces(updated_stdnt->name);

    printf("Choose Gender\n"
           "1. Male\n"
           "2. Female\n"
           ">> ");
    if (!get_uint32(&Selection) || Selection < 1 || Selection > 2)
    {
        printf("Invalid Selection\n");
        PRESS_ENTER_TO_TRY_AGAIN;
        stdnt_free(updated_stdnt);
        return stdnt_update();
    }
    updated_stdnt->gender = gend[Selection];

    if(data_exists(Dept_head, ""))
    {
        printf("Enter Department ID: ");
        if(!get_uint32(&(updated_stdnt->dept_id)))
        {
            printf( "WRONG INPUT !!!\n"
                "Empty, Overflow or Unwanted Characters\n");
            PRESS_ENTER_TO_TRY_AGAIN;
            stdnt_free(updated_stdnt);
            return stdnt_add();
        }
        if(updated_stdnt->dept_id != 0 && dept_search_by_id(*Dept_head, updated_stdnt->dept_id) == NULL)
        {
            printf("No Department with ID %d\n", updated_stdnt->dept_id);
            PRESS_ENTER_TO_TRY_AGAIN;
            stdnt_free(updated_stdnt);
            return stdnt_add();
        }
    }
    else
    {
        printf("Department ID for student is autometically set to 0, as no department exists\n");
        updated_stdnt->dept_id = 0;
        PRESS_ENTER_TO_CONTINUE;
    }

    if(dept = dept_search_by_id(*Dept_head, old_stdnt->dept_id))
        dept->gender_count[old_stdnt->gender%2]--;
    if(dept = dept_search_by_id(*Dept_head, updated_stdnt->dept_id))
        dept->gender_count[updated_stdnt->gender%2]++;
    update_node(*Stdnt_head, updated_stdnt, stdnt_match, stdnt_free);
    Update_saved = FALSE;
    printf("STUDENT  UPDATED  SUCCESSFULLY !!!\n");
    PRESS_ENTER_TO_GO_BACK;

    return second_menu();
}

void stdnt_display()
{
    student_t* curr_stdnt = NULL;
    department_t* dept = NULL;
    grade_t* grade = NULL;

    if(!data_exists(Stdnt_head, "Student"))
    {
        printf("Nothing to DISPLAY\n");
        PRESS_ENTER_TO_GO_BACK;
        return second_menu();
    }
    CLEAR_SCREEN;
    printf(     "+----------------------------------------------------------------------------------------------+\n"
                "|                                         STUDENT  TABLE                                       |\n"
                "+------------+----------------------+--------+----------------------+---------+------+---------+\n"
                "| Student ID | Student Name         | Gender | Department Name      | English | Math | History |\n"
                "+------------+----------------------+--------+----------------------+---------+------+---------+\n");
    for(double_list_node_t* curr = *Stdnt_head; curr != NULL; curr = curr->next) 
    {
        curr_stdnt = curr->data;
        dept = dept_search_by_id(*Dept_head, curr_stdnt->dept_id);
        grade = grade_search_by_id(*Grade_head, curr_stdnt->id);
        printf( "| %-11d"    "| %-21s"              "| %-7c" "| %-21s",
                curr_stdnt->id, curr_stdnt->name, curr_stdnt->gender, dept ? dept->name : "");
        grade ? printf("| %-8u"  "| %-4u | %-8u"  "|\n", grade->grades[ENGLISH], grade->grades[MATH], grade->grades[HISTORY])
            : printf("| %-8s"  "| %-4s | %-8s"  "|\n", "", "", "");
    }
    printf(     "+------------+----------------------+--------+----------------------+---------+------+---------+\n");
    PRESS_ENTER_TO_GO_BACK;
    
    return second_menu();
}

void stdnt_save()
{
    student_t* stdnt = NULL;
    FILE* fw = NULL;

    if(Stdnt_head)
    {
        fw = fopen(Student_file, "w");
        if(fw == NULL)
        {
            PRINT_ERROR_AND_EXIT("Could Not Open Department File!");
        }
        for(double_list_node_t* curr = *Stdnt_head; curr != NULL; curr = curr->next)
        {
            stdnt = curr->data;
            fprintf(fw, "%d, %s, %c, %d\n", stdnt->id, stdnt->name,
                    stdnt->gender, stdnt->dept_id);
        }
        fclose(fw);    
    }

    return;
}

bool_t stdnt_match(void* stdnt1, void* stdnt2)
{
    if(((student_t*)stdnt1)->id == ((student_t*)stdnt2)->id)
        return TRUE;
    else
        return FALSE;
    
    return FALSE;
}

student_t *stdnt_alloc()
{
    student_t* stdnt = CALLOC(1, student_t);
    stdnt->name = CALLOC(STDNT_NAME_LEN, char);

    return stdnt;
}

void stdnt_free(void* ptr)
{
    if(!ptr)
        return;
    free(((student_t*)ptr)->name);
    free(ptr);

    return;
}

void stdnt_list_free()
{
    free_list(Stdnt_head, stdnt_free);
    
    return;
}
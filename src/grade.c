#include "grade.h"
#include "student.h"
#include "input.h"
#include "app.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define MAX_STDNT_ID 999

uint32_t get_grade_id(void* data);
grade_t* grade_alloc();
void grade_free(void* ptr);

char* Grade_file = "database/grade-table.txt"; 
double_list_node_t** Grade_head;
static uint32_t Selection;
char* subjects[NUM_OF_GRADES] = {"English", "Math", "History"}; 

void grade_load()
{
    char line[INPUT_BUF_SIZE];
    grade_t* grade = NULL;
    FILE* file = NULL;

    Grade_head = CALLOC(1, double_list_node_t*);
    if (!(file = fopen(Grade_file, "r")))
    {
        printf("No Grade Database Found\n");
        return;
    }
    while (fgets(line, sizeof(line), file))
    {
        grade = grade_alloc();
        if (sscanf(line, "%d , %hhu , %hhu , %hhu", &(grade->stdnt_id), 
                   &(grade->grades[ENGLISH]), &(grade->grades[MATH]), 
                   &(grade->grades[HISTORY])) != 4)
        {
            grade_free(grade);
            PRINT_ERROR_AND_EXIT("ERROR PARSING LINE FROM GRADE DATABASE!");
        }
        insert_node(Grade_head, grade, get_grade_id);
    }
    fclose(file);
    printf("Grade Data Loaded from Grade Database!!!\n");

    return;
}

grade_t* grade_search_by_id(double_list_node_t* head, uint32_t stdnt_id)
{
    grade_t* curr_grade = NULL;
    
    for (double_list_node_t* curr = head; curr != NULL; curr = curr->next)
    {
        curr_grade = curr->data;
        if (curr_grade->stdnt_id == stdnt_id)
            return curr_grade;
    }

    return NULL; // Return NULL if no grade is found with the given student ID
}

uint32_t get_grade_id(void* data)
{
    return ((grade_t*)data)->stdnt_id;
}

void grade_add()
{
    grade_t* grade = NULL;
    student_t* found_grade = NULL;
    uint32_t grd = 101;
    uint32_t i = 0;

    if(!data_exists(Stdnt_head, "Student"))
    {
        printf("No Student exists to Add grade\n");
        PRESS_ENTER_TO_GO_BACK;
        return second_menu();
    }
    CLEAR_SCREEN;
    print_border(MENU_FIELD_LEN);
    print_centered("Add Grade", MENU_FIELD_LEN);
    print_border(MENU_FIELD_LEN);

    grade = grade_alloc();
    printf("Enter Student ID: ");
    if (!get_uint32(&(grade->stdnt_id))  || grade->stdnt_id > MAX_STDNT_ID || grade->stdnt_id == 0)
    {
        printf("Invalid Student ID\n");
        grade_free(grade);
        PRESS_ENTER_TO_TRY_AGAIN;
        return grade_add();
    }
    if (!(stdnt_search_by_id(*Stdnt_head, grade->stdnt_id)))
    {
        printf("No student with ID: %d\n", grade->stdnt_id);
        grade_free(grade);
        PRESS_ENTER_TO_TRY_AGAIN;
        return grade_add();
    }
    if((grade_search_by_id(*Grade_head, grade->stdnt_id)))
    {
        printf("Grades for student ID: %u Already Exists!\n", grade->stdnt_id);
        grade_free(grade);
        PRESS_ENTER_TO_TRY_AGAIN;
        return grade_add();
    }

    for (i = 0; i < NUM_OF_GRADES; i++)
    {
        printf("Enter grade for %s: ", subjects[i]);
        if (!get_uint32(&grd) || grd > 100)
        {
            printf("Invalid Grade\n");
            grade_free(grade);
            PRESS_ENTER_TO_TRY_AGAIN;
            return grade_add();
        }
        grade->grades[i] = grd;
    }

    insert_node(Grade_head, grade, get_grade_id);
    printf("Grade Added Successfully!\n");
    Update_saved = FALSE;
    PRESS_ENTER_TO_GO_BACK;

    return second_menu();
}

void grade_delete()
{
    grade_t* grade = grade_alloc();
    grade_t* found = NULL;
    
    if(!data_exists(Grade_head, "Grade"))
    {
        printf("Nothing to DELETE\n");
        PRESS_ENTER_TO_GO_BACK;
        return second_menu();
    }
    CLEAR_SCREEN;
    print_border(MENU_FIELD_LEN);
    print_centered("Delete Grade", MENU_FIELD_LEN);
    print_border(MENU_FIELD_LEN);

    printf("Enter Student ID: ");
    if (!get_uint32(&(grade->stdnt_id))   || grade->stdnt_id > MAX_STDNT_ID || grade->stdnt_id == 0)
    {
        printf("Invalid Student ID\n");
        grade_free(grade);
        PRESS_ENTER_TO_TRY_AGAIN;
        return grade_delete();
    }
    if (!(stdnt_search_by_id(*Stdnt_head, grade->stdnt_id)))
    {
        printf("No student with ID: %d\n", grade->stdnt_id);
        grade_free(grade);
        PRESS_ENTER_TO_TRY_AGAIN;
        return grade_delete();
    }
    if(!(found = grade_search_by_id(*Grade_head, grade->stdnt_id)))
    {
        printf("No Grades Found for Student ID: %u !\n", grade->stdnt_id);
        grade_free(grade);
        PRESS_ENTER_TO_TRY_AGAIN;
        return grade_delete();
    }
    delete_node(Grade_head, found, grade_match, grade_free);
    printf("Grade Deleted Successfully!\n");
    Update_saved = FALSE;
    PRESS_ENTER_TO_GO_BACK;
    
    return second_menu();
}

void grade_update()
{
    grade_t* updated_grade = grade_alloc();
    grade_t* found_grade = NULL;
    uint32_t grd = 101;
    uint32_t i = 0;

    if(!data_exists(Grade_head, "Grade"))
    {
        printf("Nothing to UPDATE\n");
        PRESS_ENTER_TO_GO_BACK;
        return second_menu();
    }
    CLEAR_SCREEN;
    print_border(MENU_FIELD_LEN);
    print_centered("Update Grade", MENU_FIELD_LEN);
    print_border(MENU_FIELD_LEN);

    printf("Enter Student ID: ");
    if (!get_uint32(&(updated_grade->stdnt_id))   || updated_grade->stdnt_id > MAX_STDNT_ID || updated_grade->stdnt_id == 0)
    {
        printf("Invalid Student ID\n");
        grade_free(updated_grade);
        PRESS_ENTER_TO_TRY_AGAIN;
        return grade_update();
    }
    if (!(stdnt_search_by_id(*Stdnt_head, updated_grade->stdnt_id)))
    {
        printf("No student with ID: %d\n", updated_grade->stdnt_id);
        grade_free(updated_grade);
        PRESS_ENTER_TO_TRY_AGAIN;
        return grade_update();
    }
    if(!(grade_search_by_id(*Grade_head, updated_grade->stdnt_id)))
    {
        printf("No Grades Found for Student ID: %u !\n", updated_grade->stdnt_id);
        grade_free(updated_grade);
        PRESS_ENTER_TO_TRY_AGAIN;
        return grade_update();
    }

    for (i = 0; i < NUM_OF_GRADES; i++)
    {
        printf("Enter New Grade for %s: ", subjects[i]);
        if (!get_uint32(&grd) || grd > 100)
        {
            printf("Invalid Grade\n");
            grade_free(updated_grade);
            PRESS_ENTER_TO_TRY_AGAIN;
            return grade_update();
        }
        updated_grade->grades[i] = grd;
    }

    update_node(*Grade_head, updated_grade, grade_match, grade_free);
    printf("Grade Updated Successfully!\n");
    Update_saved = FALSE;
    PRESS_ENTER_TO_GO_BACK;

    return second_menu();
}

void grade_display()
{
    grade_t* curr_grade = NULL;
    student_t* stdnt = NULL;

    if(!data_exists(Grade_head, "Grade"))
    {
        printf("Nothing to DISPLAY\n");
        PRESS_ENTER_TO_GO_BACK;
        return second_menu();
    }

    CLEAR_SCREEN;
    printf(     "+----------------------+---------+------+---------+\n"
                "|  Student Name        | English | Math | History |\n"
                "+----------------------+---------+------+---------+\n");
    for (double_list_node_t* curr = *Grade_head; curr != NULL; curr = curr->next)
    {
        curr_grade = (grade_t*)curr->data;
        stdnt = stdnt_search_by_id(*Stdnt_head, curr_grade->stdnt_id);
        printf( "| %-21s"              "| %-4u    | %-4u | %-4u    |\n", stdnt ? stdnt->name : "",
               curr_grade->grades[ENGLISH], curr_grade->grades[MATH], curr_grade->grades[HISTORY]);
    }
    printf(     "+----------------------+---------+------+---------+\n");
    PRESS_ENTER_TO_GO_BACK;

    return second_menu();
}

void grade_save()
{
    grade_t* grade = NULL;
    FILE* fw = NULL;
    
    if(Grade_head)
    {
        fw = fopen(Grade_file, "w");
        if(fw == NULL)
        {
            PRINT_ERROR_AND_EXIT("Could Not Open Department File!");
        }
        for(double_list_node_t* curr = *Grade_head; curr != NULL; curr = curr->next)
        {
            grade = curr->data;
            fprintf(fw, "%d, %d, %d, %d\n", grade->stdnt_id, grade->grades[0],
                    grade->grades[1], grade->grades[2]);
        }
        fclose(fw);    
    }

    return;
}

grade_t* grade_alloc()
{
    grade_t* grade = CALLOC(1, grade_t);
    grade->grades = CALLOC(NUM_OF_GRADES, uint8_t);

    return grade;
}

bool_t grade_match(void* grade1, void* grade2)
{
    return (((grade_t*)grade1)->stdnt_id == ((grade_t*)grade2)->stdnt_id);
}

void grade_free(void* ptr)
{
    if(!ptr)
        return;
    free(((grade_t*)ptr)->grades);
    free(ptr);

    return;
}

void grade_list_free()
{
    free_list(Grade_head, grade_free);
    
    return;
}
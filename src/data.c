#include "data.h"
#include "dept.h"
#include "student.h"
#include "grade.h"
#include "app.h"

#define DEPT_NAME_SIZE 20

double_list_node_t* create_node(void* data);
void free_node(void* ptr, void (*free_data)(void*));
int insert_cmp(void* data1, void* data2,  uint32_t (*get_id)(void*));

bool_t Update_saved = TRUE;

void load_data()
{
    dept_load();
    stdnt_load();
    grade_load();
    
    return;
}

void save()
{
    if(Update_saved)
    {
        printf("Nothing to Save\n");
        return;
    }
    dept_save();
    stdnt_save();
    grade_save();
    Update_saved = TRUE;
    printf("Data Saved Successfully\n");

    return;
}

void save_data()
{
    CLEAR_SCREEN;
    save();
    PRESS_ENTER_TO_CONTINUE;
    main_menu();
}

double_list_node_t* create_node(void* data)
{
    double_list_node_t *new_node = CALLOC(1, double_list_node_t);
    new_node->data = data;
    new_node->prev = NULL;
    new_node->next = NULL;

    return new_node;
}

int insert_cmp(void* data1, void* data2,  uint32_t (*get_id)(void*))
{
    if (data1 == NULL || data2 == NULL) 
    {
        printf("data1 or data2 can't be NULL\n");
        exit(EXIT_FAILURE);
    }

    return (int64_t)(get_id(data1)) - (int64_t)(get_id(data2));
}

void insert_node( double_list_node_t** head, void* data, uint32_t (*get_id)(void*))
{
    double_list_node_t* new_node = create_node(data);
    if(*head == NULL)
    {
        *head = new_node;
        return;
    }
    if(insert_cmp(data, (*head)->data, get_id) < 0) 
    {
        new_node->next = *head;
        (*head)->prev = new_node;
        *head = new_node;
        return;
    }
    double_list_node_t* curr = *head;
    while((curr->next != NULL) && (insert_cmp(data, curr->next->data, get_id) > 0))
        curr = curr->next;
    if(curr->next)
    {
        new_node->next = curr->next;
        curr->next->prev = new_node;
    }
    new_node->prev = curr;
    curr->next = new_node;

    return;
}

void delete_node(double_list_node_t** head, void* data, bool_t (*match)(void*, void*), void (*free_data)(void*))
{
    if (head == NULL || *head == NULL || data == NULL)
    {
        PRINT_ERROR_AND_EXIT("Head, *head, or data cannot be NULL!");
    }
        

    double_list_node_t* curr = *head;

    while (curr != NULL)
    {
        if (match(curr->data, data)) 
        {
            if (curr->prev != NULL) 
                curr->prev->next = curr->next;
            else 
                *head = curr->next;
            if (curr->next != NULL) 
                curr->next->prev = curr->prev;
            free_node(curr, free_data);

            return;
        }
        curr = curr->next;
    }

    return;
}

void update_node(double_list_node_t* head, void* new_data, bool_t (*match)(void*, void*), void (*free_data)(void*))
{
    if (head == NULL  || new_data == NULL)
    {
        PRINT_ERROR_AND_EXIT("Head, *head, or data cannot be NULL!");
    }
        

    double_list_node_t* curr = head;

    while (curr != NULL)
    {
        if (match(curr->data, new_data))
        {
            free_data(curr->data);
            curr->data = new_data;
            return;
        }
            
        curr = curr->next;
    }

    return;
}

void free_node(void* ptr, void (*free_data)(void*))
{
    free_data(((double_list_node_t*)ptr)->data);
    free(ptr);

    return;
}

void free_list(double_list_node_t** head, void(*free_data)(void*))
{
    double_list_node_t* curr = NULL;

    for(curr = *head; curr != NULL; curr = curr->next)
        free_node(curr, free_data);
    free(head);
    
    return;
}

void free_and_exit()
{
    dept_list_free();
    stdnt_list_free();
    grade_list_free();
    
    exit(EXIT_SUCCESS);

    return;
}

/*****************************************************************************************************
 * Name:    data_exists
 * Input:   head        an address of the head of a doubly linked list
 *          data_label  if NULL, the error message wont be printed
 *                      if a valid label then the error message along with the label will be printed
 * Return:  TRUE        if the list  exists
 *          FALSE       if the list does not exist
 * Description          to check if there is any data in the list
 *****************************************************************************************************/
bool_t data_exists(double_list_node_t** head, const char* data_label)
{
    if(!head || !*head)
    {
        if(*data_label)
            printf("No %s Database Found\n"
                "or\n"
                "No Data in %s Database Found\n", data_label, data_label);
        return FALSE;
    }

    return TRUE;
}
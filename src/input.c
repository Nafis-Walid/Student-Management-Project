#include "input.h"

#define ITEM_LIMIT 20

void* custom_calloc(size_t num_el, size_t size_el)
{

    void *ptr = NULL;
    ptr = calloc(num_el, size_el);

    if(!ptr)
    {
        PRINT_ERROR_AND_EXIT("Memmory Allocation Failed!!!");
    }
        
    return ptr;
}

bool_t if_file_exists(uint8_t* filename)
{
    FILE* file = NULL;

    if(!filename)
    {
        PRINT_ERROR_AND_EXIT("File name cannot be empty.");
    }
    if(!(file = fopen(filename, "r")))
        return FALSE;
    fclose(file);

    return TRUE;
}

bool_t get_str(char* str, uint32_t size, int (*check_func)(int), FILE* fptr)
{
    uint32_t len = 0;
    uint32_t c = 0;
    
    if(str == NULL || size == 0)
    {
        PRINT_ERROR_AND_EXIT("storage or size cannot be null");
    }
    if (!fgets(str, size+1, fptr))
        return FALSE;
    
    len = strcspn(str, "\n");
    if (len == size && str[size-1] != '\n')/*overflow*/
    {
        while ((c = fgetc(fptr)) != '\n' && c != EOF);
        perror("Buffer Overflow");
        return FALSE;
    }
    
    str[len] = '\0';
    
    if (!*str)
        return FALSE;
    if (check_func)
        for (char *p = str; *p != '\0'; p++)
            if (!check_func(*p))
                return FALSE;

    return TRUE;
}

void trim_whitespaces(char* str)
{
    char* start = NULL;
    char* end = NULL;
    uint8_t limit = 255;

    start = str;
    while ((*start && isspace(*start)) && (limit-- > 0))
        *start++ = '\0';

    if(*start)
    {
        end = start + strlen(start) - 1;
        while (end > start && isspace(*end))
            *end-- = '\0';
    }
    memmove(str, start, strlen(start) + 1);
    
    return;
}


bool_t get_uint32(int* a)
{
    char* num_str = NULL;
    char* end = NULL;

    num_str = CALLOC(UINT32_T_LEN, char);
    if(!get_str(num_str, UINT32_T_LEN, isdigit, stdin))
    {
        free(num_str);
        return FALSE;
    }
    uint64_t num_int = strtoul(num_str, &end, 10);
    if(*end || num_int > UINT32_MAX)
    {
        free(num_str);
        return FALSE;
    }
    *a = num_int;
    free(num_str);

    return TRUE;
}

int is_valid_char(int c)
{
    return isprint(c) && (c != ',');
}

int is_valid_name(const char* str)
{
    while (*str != '\0') 
    {
        if(!is_valid_char(*str))
            return FALSE;
        str++;
    }
    
    return TRUE;
}

char **parse_line(char *line, unsigned char *lengths, const char* delimeter)
{
    char** items = NULL;
    char* tmp = NULL;
    uint32_t i = 0;
    uint32_t num_el = 0;
    if(!*line || !*lengths)
    {
        PRINT_ERROR_AND_EXIT("line or lengths cannot be empty");
    }
    num_el = strlen(lengths);
    if(num_el > 20)
    {
        PRINT_ERROR_AND_EXIT("number of items cannot be greater than %d", 20);
    }
    items = CALLOC(num_el, char*);
    tmp = strtok(line, delimeter);
    trim_whitespaces(tmp);
    num_el--;
    while(i<num_el)
    {
        items[i] = tmp;
        trim_whitespaces(tmp = strtok(NULL, delimeter));
        i++;
    }
    items[i] = tmp;
    
    return items;
}

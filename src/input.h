#ifndef __INPUT_H__
#define __INPUT_H__

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

#define UINT32_T_LEN 11
#define INPUT_BUF_SIZE 255
#define CALLOC(num_el, type) \
    (type *)custom_calloc((num_el), sizeof(type))
#define PRESS_ENTER_TO_CONTINUE \
    printf("Press Enter to Continue..."); \
    while (getchar() != '\n')
#define PRESS_ENTER_TO_TRY_AGAIN \
    printf("Press Enter to Try Again..."); \
    while (getchar() != '\n')
#define PRESS_ENTER_TO_GO_BACK \
    printf("Press Enter to Go Back..."); \
    while (getchar() != '\n')
#define CLEAR_SCREEN \
    system("clear")
// #define PRINT_ERROR_AND_EXIT(msg) \
//     print_error_and_exit(msg, __FILE__, __FUNCTION__, __LINE__)
// #define PRINT_ERROR(msg) \
//     print_error(msg, __FILE__, __FUNCTION__, __LINE__)
#define PRINT_ERROR_AND_EXIT(fmt, ...) \
    if (*#__VA_ARGS__) \
        fprintf(stderr, "%s: In function \'%s\':\nLine %d:" fmt "\n", __FILE__, __LINE__, __func__, ##__VA_ARGS__); \
    else \
        fprintf(stderr, "%s: In function \'%s\':\nLine %d:" fmt "\n", __FILE__, __LINE__, __func__); \
    exit(EXIT_FAILURE)
typedef enum
{
    FALSE = 0,
    TRUE
}bool_t;

extern char** parse_line(char* line, unsigned char* lengths, const char* delimeter);
extern void* custom_calloc(size_t num_el, size_t size_el);
extern bool_t if_file_exists(uint8_t* filename);
extern void trim_whitespaces(char* str);
extern bool_t get_str(char* a, uint32_t size, int (*check_func)(int), FILE* fptr);
extern bool_t get_uint32(int* a);
extern int is_valid_char(int c);
extern int is_valid_name(const char* str);

#endif /* __INPUT_H__ */


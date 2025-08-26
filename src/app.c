#include "app.h"
#include "dept.h"
#include "student.h"
#include "grade.h"
#include "data.h"

#define MAIN_MENU 0
#define EXIT_MENU (MAIN_MENUS_ELEMENT - 1)

uint32_t show_menu(menu mnu);

static uint32_t First_selection;
static uint32_t Second_selection;
static uint32_t Third_selection;
const char *Main_menu[NUM_MAIN_MENU] = {"Main Menu",
                                    "Department Management",
                                    "Student Management",
                                    "Grade Management",
                                    "Save Data",
                                    "Exit" };
const char *Dept_menu[NUM_DEPT_MENU] = {"Department Management",
                                        "Add a New Department",
                                        "Delete Department",
                                        "Update Department",
                                        "Display All Departments",
                                        "Save Data",
                                        "Main Menu",
                                        "Exit"};
const char *Stdnt_menu[NUM_STDNT_MENU] = {"Student Management",
                                        "Add a New Student",
                                        "Delete Student",
                                        "Update Student",
                                        "Display All Students",
                                        "Save Data",
                                        "Main Menu",
                                        "Exit"};
const char *Grade_menu[NUM_GRADE_MENU] = {"Grade Management",
                                        "Add a New Grade",
                                        "Delete Grade",
                                        "Update Grade",
                                        "Display All Grades",
                                        "Save Data",
                                        "Main Menu",
                                        "Exit"};
const char *Exit_menu[NUM_EXIT_MENU] = {"Exit Confirm",
                                        "Save and Exit",
                                        "Exit Without Saving"};
menu Main_menus[MAIN_MENUS_ELEMENT]
    = {{Main_menu, NUM_MAIN_MENU},
    {Dept_menu, NUM_DEPT_MENU},
    {Stdnt_menu, NUM_STDNT_MENU},
    {Grade_menu, NUM_GRADE_MENU},
    {Exit_menu, NUM_EXIT_MENU}};
void (*Last_menu[4][3])(void)
    = {dept_add, stdnt_add, grade_add,
    dept_delete, stdnt_delete, grade_delete,
    dept_update, stdnt_update, grade_update,
    dept_display, stdnt_display, grade_display};

void run_project()
{
    
    load_data();
    PRESS_ENTER_TO_CONTINUE;

    return main_menu();
}

void main_menu()
{
    First_selection = show_menu(Main_menus[MAIN_MENU]);

    CLEAR_SCREEN;
    switch (First_selection)
    {
        case 4:
            return save_data();
        case 5:
            return Exit();
        default:
            return second_menu();
    }

    return;
}

/************************************************************************************
 * Function Name:   show_menu
 * Input:           an object of struct menu
 * Output:          A selection among the menu
 * Description      if wrong selection is entered it will prompt over and over again
 *************************************************************************************/
uint32_t show_menu(menu mnu) 
{
    uint32_t selected = 0;
    const char **menu_list = mnu.menu_list;
    uint32_t num_menu = mnu.num_menu;

    CLEAR_SCREEN;
    print_border(MENU_FIELD_LEN);
    print_centered(menu_list[0], MENU_FIELD_LEN);
    print_border(MENU_FIELD_LEN);
    for (uint32_t i = 1; i < num_menu; i++)
    {
        printf("| %2d. %-*s|\n", i, MENU_FIELD_LEN-5, menu_list[i]);
    }
    print_border(MENU_FIELD_LEN);
    printf("select >> ");
    if (!get_uint32(&selected) || selected >= num_menu || selected <= 0)
    {
        printf("Selection out of range or unwanted characters\n");
        PRESS_ENTER_TO_CONTINUE;
        return show_menu(mnu);
    }

    return selected;
}

void print_border(uint32_t width)
{
    if (width == 0)
    {
        PRINT_ERROR_AND_EXIT("width Cannot be 0!");
    }
    
    printf("+");
    while (width--)
        printf("-");
    printf("+\n");

    return;
}

void print_centered(const char* menu_header, uint32_t field_len)
{
    uint32_t header_len = 0;
    uint32_t if_odd = 0;

    header_len = strlen(menu_header);
    if_odd = (MENU_FIELD_LEN + header_len) % 2;
    printf("|%*s%*s|\n", (MENU_FIELD_LEN + header_len) / 2, menu_header,
           ((MENU_FIELD_LEN - header_len) / 2) + if_odd, "");

    return;
}

void second_menu()
{
    Second_selection = show_menu(Main_menus[First_selection]);
    switch (Second_selection)
    {
        case 5:
            return save_data();
        case 6:
            return main_menu();
        case 7:
            return Exit();
        default:
            CLEAR_SCREEN;
            return Last_menu[Second_selection-1][First_selection-1]();
    }

    return;
}

void Exit()
{
    if (!Update_saved)
    {
        Third_selection = show_menu(Main_menus[EXIT_MENU]);
        switch (Third_selection)
        {
            case 1:
                save();
            case 2:
                return free_and_exit();
            default:
                PRINT_ERROR_AND_EXIT("Error Occured in Selection");
                return;
        }
    }

    return free_and_exit();
}


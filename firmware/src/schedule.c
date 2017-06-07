#include "badge_menu.h"

#define NULL ((void *)0)

const struct menu_t sch_d1_7_m[] = {
    {"Closing", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  5:50pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"After Party", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  6:30pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Back", VERT_ITEM|LAST_ITEM|DEFAULT_ITEM, BACK, {NULL} }
};

const struct menu_t sch_d1_6_m[] = {
    {"Next Generation", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  4:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Threat Intel", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  4:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Vendor Break", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  4:50pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Zero Trust", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  5:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"More", VERT_ITEM, SMENU, {sch_d1_7_m}},
    {"Back", VERT_ITEM|LAST_ITEM|DEFAULT_ITEM, BACK, {NULL} }
};

const struct menu_t sch_d1_5_m[] = {
    {"Vendor Break", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  2:50pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Crowdsourced", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  3:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"AWS Survival", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  3:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Vendor Break", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  3:50pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"More", VERT_ITEM, SMENU, {sch_d1_6_m}},
    {"Back", VERT_ITEM|LAST_ITEM|DEFAULT_ITEM, BACK, {NULL} }
};

const struct menu_t sch_d1_4_m[] = {
    {"CTF Prep", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  1:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Vendor Break", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  1:50pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Network Segment", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  2:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"PowerShell", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  2:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"More", VERT_ITEM, SMENU, {sch_d1_5_m}},
    {"Back", VERT_ITEM|LAST_ITEM|DEFAULT_ITEM, BACK, {NULL} }
};

const struct menu_t sch_d1_3_m[] = {
    {"Maintainability", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 11:00am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Lunch", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 11:50am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Defenders", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  1:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Hybrid Cloud", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  1:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"More", VERT_ITEM, SMENU, {sch_d1_4_m}},
    {"Back", VERT_ITEM|LAST_ITEM|DEFAULT_ITEM, BACK, {NULL} }
};

const struct menu_t sch_d1_2_m[] = {
    {"CTF Intro", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 10:30am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Badge Intro", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 10:40am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Vendor Break", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 10:50am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Game Theory", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 11:00am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"More", VERT_ITEM, SMENU, {sch_d1_3_m}},
    {"Back", VERT_ITEM|LAST_ITEM|DEFAULT_ITEM, BACK, {NULL} }
};

const struct menu_t sch_d1_1_m[] = {
    {"Breakfast", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  8:00am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Welcome", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  9:00am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Keynote", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  9:10am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Vendor Break", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 10:10am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"More", VERT_ITEM, SMENU, {sch_d1_2_m}},
    {"Back", VERT_ITEM|LAST_ITEM|DEFAULT_ITEM, BACK, {NULL} }
};

const struct menu_t sch_d2_5_m[] = {
    {"Spy vs. Spy", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  3:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Closing", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  4:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Back", VERT_ITEM|LAST_ITEM|DEFAULT_ITEM, BACK, {NULL} }
};

const struct menu_t sch_d2_4_m[] = {
    {"Vendor Break", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  1:50pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Budget Pentest", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  2:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"RoboCop - CICD", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  2:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Vendor Break", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  2:50pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"More", VERT_ITEM, SMENU, {sch_d2_5_m}},
    {"Back", VERT_ITEM|LAST_ITEM|DEFAULT_ITEM, BACK, {NULL} }
};

const struct menu_t sch_d2_3_m[] = {
    {"DevOpSec", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 11:10am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Retailing TL", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 11:10am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Lunch", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 12:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Terry McAuliffe", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  1:00pm", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"More", VERT_ITEM, SMENU, {sch_d2_4_m}},
    {"Back", VERT_ITEM|LAST_ITEM|DEFAULT_ITEM, BACK, {NULL} }
};

const struct menu_t sch_d2_2_m[] = {
    {"OSINT", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 10:10am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Linkography", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 10:10am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"CTF", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 10:10am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Vendor Break", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 11:00am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"More", VERT_ITEM, SMENU, {sch_d2_3_m}},
    {"Back", VERT_ITEM|LAST_ITEM|DEFAULT_ITEM, BACK, {NULL} }
};

const struct menu_t sch_d2_1_m[] = {
    {"Breakfast", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  8:00am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Welcome", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  8:50am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Keynote", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"  9:00am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"Vendor Break", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" 10:00am", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {" ", VERT_ITEM|SKIP_ITEM, TEXT, {NULL}},
    {"More", VERT_ITEM, SMENU, {sch_d2_2_m}},
    {"Back", VERT_ITEM|LAST_ITEM|DEFAULT_ITEM, BACK, {NULL} }
};

const struct menu_t sch_main_m[] = {
    {"Day 1", VERT_ITEM, MENU, {sch_d1_1_m}},
    {"Day 2", VERT_ITEM, MENU, {sch_d2_1_m}},
    {"Back", VERT_ITEM|LAST_ITEM|DEFAULT_ITEM, BACK, {NULL} }
};



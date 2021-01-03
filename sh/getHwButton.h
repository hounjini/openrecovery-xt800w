/*
KEY VALUES FROM motorola XT800W linux kernel /include/linux/input.h
#define KEY_VOLUMEDOWN		114
#define KEY_VOLUMEUP		115
#define KEY_END			107
#define KEY_COMPOSE		127
#define KEY_HP			211
#define KEY_CAMERA		212
*/

/*
KEY VALUES USING
KEY_DOWN
KEY_UP
KEY_CENTER
KEY_ENDTER
*/

#define NO_ACTION					-1
#define ACTION_UP					-2
#define ACTION_DOWN					-3
#define ACTION_SELECT_ITEM			-4 

#define MAX_DEVICES 16

void ui_init(void);
int ui_get_key();
int ui_wait_key();
int ui_key_pressed(int key);
void ui_clear_key_queue();


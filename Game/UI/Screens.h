#ifndef MAIN_MENU_SCREEN_H
#define MAIN_MENU_SCREEN_H
#include <SDL_events.h>

struct Screen {
	void (*initialize)();
	int (*update)();
	void (*render)();
	void (*destroy)();
	short (*handle_event)(SDL_Event ev);
};

extern struct Screen main_menu_screen;
extern struct Screen fight_screen;
#endif

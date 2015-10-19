/**
* main.c
*
* The entry point for the app
*/
#include <pebble.h>
#include "main_menu_window.h"
#include "model.h"
// #include "old_entry_window.h"

void init(){
	init_model();
	main_menu_window_init();
// 	old_entry_window_init(0);
}

void deinit() {
	window_destroy(main_menu_window);
// 	window_destroy(old_entry_window);
	
	nuke_all_old_workouts(); // TODO: Remove this
}
	
int main(void) {
	init();
	app_event_loop();
	deinit();
}
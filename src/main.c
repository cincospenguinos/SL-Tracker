/**
* main.c
*
* The entry point for the app
*/
#include <pebble.h>
#include "main_menu_window.h"
#include "model.h"

void init(){
	init_model();
	main_menu_window_init();
}

void deinit() {
	window_destroy(main_menu_window);
	
	nuke_all_old_workouts(); // TODO: Remove this
}
	
int main(void){
	init();
	app_event_loop();
	deinit();
}
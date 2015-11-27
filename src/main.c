/**
* main.c
*
* The entry point for the app
*/
#include <pebble.h>
#include "main_menu_window.h"
#include "model.h"
#include "log_menu_window.h"
#include "old_entry_window.h"
#include "setup_workout_window.h"
#include "set_weight_window.h"
#include "workout_window.h"

void init(){
	init_model();
	main_menu_window_init();
}

void deinit() {
	window_destroy(main_menu_window);
}
	
int main(void) {
	init();
	app_event_loop();
	deinit();
}
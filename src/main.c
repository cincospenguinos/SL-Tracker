/**
* main.c
*
* The entry point for the app.
*
* Here's a list of things to do to better improve the app:
*
* - 
* - Get permission to post it onto the store
* - Translate it into french
* - Let the user set the weight to be in pounds or kilos
* - Migrate the app to pebble time users
* - Migrate the app to pebble round users
*/

#include <pebble.h>
#include "main_menu_window.h"
#include "model.h"
#include "log_menu_window.h"
#include "old_entry_window.h"
#include "setup_workout_window.h"
#include "set_weight_window.h"
#include "workout_window.h"
#include "set_day_type_window.h"

void init(){
	init_model();
	main_menu_window_init();
}

void deinit() {
	window_destroy(main_menu_window);
	
	if(log_menu_window != NULL)
		window_destroy(log_menu_window);
	
	if(old_entry_window != NULL)
		window_destroy(old_entry_window);
	
	if(setup_workout_window != NULL)
		window_destroy(setup_workout_window);
	
	if(set_day_type_window != NULL)
		window_destroy(set_day_type_window);
	
	if(set_weight_window != NULL)
		window_destroy(set_weight_window);
	
	if(workout_window != NULL)
		window_destroy(workout_window);
}
	
int main(void) {
	init();
	app_event_loop();
	deinit();
}
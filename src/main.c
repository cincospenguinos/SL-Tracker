/**
* main.c
*
* The entry point for the app
*/
#include <pebble.h>
#include "workout_window.h"

void init(){
	workout_window = window_create();
	window_set_window_handlers(workout_window, (WindowHandlers){
		.load = workout_window_load,
		.unload = workout_window_unload
	});
	
	window_stack_push(workout_window, true);
}

void deinit(){
	window_destroy(workout_window);
}
	
int main(void){
	init();
	app_event_loop();
	deinit();
}
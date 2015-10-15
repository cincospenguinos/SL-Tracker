/**
* main.c
*
* The entry point for the app
*/
#include <pebble.h>
#include "workout_window.h"

void init(){
	workout_window_init(false);
}

void deinit(){
	window_destroy(workout_window);
}
	
int main(void){
	init();
	app_event_loop();
	deinit();
}
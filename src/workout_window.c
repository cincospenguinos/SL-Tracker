#include <pebble.h>
#include "workout_window.h"

/*
 * Window Handlers
 */
	
/* Sets up the workout_window when it loads up on screen */
void workout_window_load(Window *window){
	// current_workout text layer
	current_workout = text_layer_create(GRect(0, 0, 144, 15));
	text_layer_set_text(current_workout, "Overhead Press");
	text_layer_set_font(current_workout, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text_alignment(current_workout, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(current_workout));
	
	// timer text layer
	timer = text_layer_create(GRect(36, 30, 72, 30));
	text_layer_set_font(timer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(timer, GTextAlignmentCenter);
	text_layer_set_text(timer, "00");
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(timer));
	
	// sets text layers
	for(int i = 0; i < 5; i++){
		sets[i] = text_layer_create(GRect(12 + (i * 28), 80, 14, 20));
		text_layer_set_font(sets[i], fonts_get_system_font(FONT_KEY_GOTHIC_14));
		text_layer_set_text_alignment(sets[i], GTextAlignmentCenter);
		text_layer_set_text(sets[i], "5");
		layer_add_child(window_get_root_layer(window), text_layer_get_layer(sets[i]));
	}
	
	// motivation text layer
	motivation = text_layer_create(GRect(12, 104, 120, 164));
	text_layer_set_font(motivation, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	text_layer_set_text_alignment(motivation, GTextAlignmentLeft);
	text_layer_set_text(motivation, "The quick brown fox jumped over the lazy dog. 63 characters"); // About sixty three characters are able to be dumped into this text layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(motivation));
	
	// add a tick timer to this window
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
	
	timer_running = false;
}

/* Tears down the workout_window when the window pops off the stack */
void workout_window_unload(Window *window){
	text_layer_destroy(current_workout);
	text_layer_destroy(timer);
	
	for(int i = 0; i < 5; i++)
		text_layer_destroy(sets[i]);
	
	text_layer_destroy(motivation);
}

/*
 * Drawing procedures
 */

void selected_triangle_drawing_procedure(Layer *layer, GContext *ctx){
	
}

/*
 * Tick Handlers
 */

/* Handles the time updates every second */
void tick_handler(struct tm *tick_time, TimeUnits units_changed){
	
	if(timer_running)
		update_timer();
}

/* Updates the timer */
void update_timer(){
	// Create a little buffer
	static char buffer[] = "000";
	
	// Throw in the number
	snprintf(buffer, sizeof(buffer), "%i", timer_count++);
	
	// Set the text up
	text_layer_set_text(timer, buffer);
}













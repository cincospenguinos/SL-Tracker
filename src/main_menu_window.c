#include <pebble.h>
#include "main_menu_window.h"
#include "workout_window.h"
#include "model.h"
#include "log_menu_window.h"

/*
 * Instance variables
 */

/* Init function */
void main_menu_window_init(){
	main_menu_window = window_create();
	
	window_set_window_handlers(main_menu_window, (WindowHandlers){
		.load = main_menu_window_load,
		.unload = main_menu_window_unload
	});
	
	window_stack_push(main_menu_window, true);
}

/* 
 * Window Handlers
 */

/* Called when the main_menu_window loads */
void main_menu_window_load(Window *window){
	// add all the various items
	menu_items[0] = (SimpleMenuItem){
		.title = "Start Workout",
		.subtitle = "Start a new workout",
		.callback = new_workout_callback
	};
	
	menu_items[1] = (SimpleMenuItem){
		.title = "Setup Workout",
		.subtitle = "Setup next workout",
		.callback = setup_workout_callback
	};
	
	menu_items[2] = (SimpleMenuItem){
		.title = "Check Log",
		.subtitle = "Check old workouts",
		.callback = view_log_callback
	};
	
	menu_items[3] = (SimpleMenuItem){
		.title = "Delete Log",
		.subtitle = "Delete old workouts",
		.callback = delete_log_callback
	};
	
	// setup the one section we have
	sections[0] = (SimpleMenuSection){
		.num_items = 4,
		.items = menu_items
	};
	
	// create the layer
	main_menu_layer = simple_menu_layer_create(layer_get_frame(window_get_root_layer(window)), window, sections, 1, NULL);
	
	// add it to the window
	layer_add_child(window_get_root_layer(window), simple_menu_layer_get_layer(main_menu_layer));
}

/* Called when the main_menu_window unloads */
void main_menu_window_unload(){
	simple_menu_layer_destroy(main_menu_layer);
	
	if(workout_window != NULL)
		window_destroy(workout_window);
	
	if(log_menu_window != NULL)
		window_destroy(log_menu_window);
}

/*
 * Selected callbacks
 */

/* Called when a new workout is to be created */
void new_workout_callback(){
	workout_window_init();
}

/* Called when the user wants to setup the workout */
void setup_workout_callback() {
	// TODO: Implement this
}

/* Called when the user wants to check the old workouts he/she has */
void view_log_callback(int index, void *ctx) {
	// If there's no workouts to show, let the user know
	if(get_workout_count() == 0){
		menu_items[index].subtitle = "No workouts to show!";
		layer_mark_dirty(simple_menu_layer_get_layer(main_menu_layer));
	} 
	
	// Otherwise, show them
	else {
		log_menu_init();
	}
}

/* Called when the user selects to delete the workout log */
void delete_log_callback(){
	// TODO: Implement this
}
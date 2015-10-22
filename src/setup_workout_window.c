#include <pebble.h>
#include "setup_workout_window.h"

/* The init function */
void setup_workout_window_init(){
	if(setup_workout_window == NULL)
		setup_workout_window = window_create();
	
	window_set_window_handlers(setup_workout_window, (WindowHandlers){
		.load = setup_workout_window_load,
		.unload = setup_workout_window_unload
	});
	
	window_stack_push(setup_workout_window, true);
}

/*
 * Window Handlers
 */

void setup_workout_window_load(Window *window){
	
	// Setup the simple menu items
	setup_workout_menu_items[0] = (SimpleMenuItem){
		.title = "Change day",
		.subtitle = "Change next workout",
		.callback = setup_workout_window_select_callback
	};
	
	setup_workout_menu_items[1] = (SimpleMenuItem){
		.title = "Squat Weight",
		.subtitle = "Set squat weight",
		.callback = setup_workout_window_select_callback
	};
	
	setup_workout_menu_items[2] = (SimpleMenuItem){
		.title = "Bench Weight",
		.subtitle = "Set bench press weight",
		.callback = setup_workout_window_select_callback
	};
	
	setup_workout_menu_items[3] = (SimpleMenuItem){
		.title = "Bent Rows Weight",
		.subtitle = "Set bent rows weight",
		.callback = setup_workout_window_select_callback
	};
	
	setup_workout_menu_items[4] = (SimpleMenuItem){
		.title = "Overhead Press Weight",
		.subtitle = "Set OHP weight",
		.callback = setup_workout_window_select_callback
	};
	
	setup_workout_menu_items[5] = (SimpleMenuItem){
		.title = "Deadlift Weight",
		.subtitle = "Change deadlift weight",
		.callback = setup_workout_window_select_callback
	};
	
	// Setup the sections
	sections[0] = (SimpleMenuSection){
		.num_items = 6,
		.items = setup_workout_menu_items
	};
	
	// add the menu to the window
	setup_workout_menu = simple_menu_layer_create(layer_get_frame(window_get_root_layer(window)), window, sections, 1, NULL);
	layer_add_child(window_get_root_layer(window), simple_menu_layer_get_layer(setup_workout_menu));
}

void setup_workout_window_unload(Window *window) {
	simple_menu_layer_destroy(setup_workout_menu);
	
// 	window_stack_pop(true);
}

/*
 * Select callbacks
 */

void setup_workout_window_select_callback(int index, void *ctx){
	// TODO: Implement this
}
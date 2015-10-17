#include <pebble.h>
#include "log_menu_window.h"
#include "model.h"

/* log_menu_window init function */
void log_menu_window_init(SimpleMenuItem *items) {
	// Get all the necessary data
	old_workout_count = get_workout_count();
	old_workouts = items;
	APP_LOG(APP_LOG_LEVEL_INFO, "Got all the data");
	
	// Create the window and setup the handlers
	log_menu_window = window_create();
	
	window_set_window_handlers(log_menu_window, (WindowHandlers){
		.load = log_menu_window_load,
		.unload = log_menu_window_unload
	});
	
	// Push it onto the stack
	window_stack_push(log_menu_window, true);
	APP_LOG(APP_LOG_LEVEL_INFO, "Pushed the window onto the stack");
}

/*
 * Window Handlers
 */

/* Called when the window is loaded onto the stack */
void log_menu_window_load(Window *window){
	APP_LOG(APP_LOG_LEVEL_INFO, "Loading up the window");
	for(int i = 0; i < old_workout_count; i++){
		APP_LOG(APP_LOG_LEVEL_INFO, "Pulling up a workout");
		WorkoutPeek workout = peek_workout(i);

		char *buffer = "DD-MM-YYYY - T"; // buffer to manage what shows up
		char day = 'A'; // Character representing A day or B day

		if(workout.day_type)
			day = 'B';

		snprintf(buffer, sizeof(buffer), "%i-%i-%i - %c", workout.day, workout.month, workout.year, day);

		old_workouts[i] = (SimpleMenuItem){
			.title = buffer,
			// 			.subtitle = "", // TODO: Subtitle?
			.callback = old_workout_selected_callback
		};
	}
	APP_LOG(APP_LOG_LEVEL_INFO, "Finished populating the old_workouts[] array");
	
	// Add on the menu section
	sections[0] = (SimpleMenuSection){
		.num_items = old_workout_count,
		.items = old_workouts
	};
	
	APP_LOG(APP_LOG_LEVEL_INFO, "Created the one section");
	
	// Create the log_menu layer:
	log_menu = simple_menu_layer_create(layer_get_frame(window_get_root_layer(window)), window, sections, 1, NULL);
	APP_LOG(APP_LOG_LEVEL_INFO, "Created the menu layer");
	
	// Add the menu to the window
	layer_add_child(window_get_root_layer(window), simple_menu_layer_get_layer(log_menu));
	APP_LOG(APP_LOG_LEVEL_INFO, "Added the menu to the window");
}

/* Called when the window is popped off the stack */
void log_menu_window_unload(Window *window){
	simple_menu_layer_destroy(log_menu);
}

/* Called when an old workout is selected to be viewed */
void old_workout_selected_callback(int index, void *ctx){
	// TODO: Implement the callback management
}







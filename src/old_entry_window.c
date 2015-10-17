#include <pebble.h>
#include "old_entry_window.h"

// TODO: Fix this technical debt soooo hard.

/* old_entry_init function */
void old_entry_window_init(int index){
	APP_LOG(APP_LOG_LEVEL_INFO, "Initializing old entry window");
	// We use the index to get the Workout that we need to keep track of
	current_index = index;
	
	if (old_entry_window == NULL)
		old_entry_window = window_create();
	
	window_set_window_handlers(old_entry_window, (WindowHandlers){
		.load = old_entry_window_load,
		.unload = old_entry_window_unload
	});
	
	window_set_click_config_provider(old_entry_window, (ClickConfigProvider) old_entry_window_click_config_provider);
	
	APP_LOG(APP_LOG_LEVEL_INFO, "Pushing old entry window...");
	window_stack_push(old_entry_window, true);
	APP_LOG(APP_LOG_LEVEL_INFO, "Pushed.");
}

/*
 * Window Handlers
 */

/* Runs when the window is loaded */
void old_entry_window_load(Window *window){
	APP_LOG(APP_LOG_LEVEL_INFO, "Loading old entry window");
	// Now let's create all the various text layers that we need
	date_text = text_layer_create(GRect(0, 2, 144, 28));
	text_layer_set_text_alignment(date_text, GTextAlignmentCenter);
	text_layer_set_font(date_text, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_text));
	
	// exercise1
	exercise1 = text_layer_create(GRect(0, 30, 72, 20));
	text_layer_set_text_alignment(exercise1, GTextAlignmentLeft);
	text_layer_set_font(exercise1, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(exercise1));
	
	// exercise2
	exercise2 = text_layer_create(GRect(0, 70, 72, 20));
	text_layer_set_text_alignment(exercise2, GTextAlignmentLeft);
	text_layer_set_font(exercise2, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(exercise2));
	
	// exercise3
	exercise1 = text_layer_create(GRect(0, 110, 72, 20));
	text_layer_set_text_alignment(exercise3, GTextAlignmentLeft);
	text_layer_set_font(exercise3, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(exercise3));
	
	// weight1
	weight1 = text_layer_create(GRect(72, 30, 72, 20));
	text_layer_set_text_alignment(weight1, GTextAlignmentRight);
	text_layer_set_font(weight1, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(weight1));
	
	// weight2
	weight2 = text_layer_create(GRect(72, 70, 72, 20));
	text_layer_set_text_alignment(weight2, GTextAlignmentRight);
	text_layer_set_font(weight2, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(weight2));
	
	// weight3
	weight3 = text_layer_create(GRect(72, 110, 72, 20));
	text_layer_set_text_alignment(weight3, GTextAlignmentRight);
	text_layer_set_font(weight3, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(weight3));
	
	// sets1
	sets1 = text_layer_create(GRect(0, 50, 144, 20));
	text_layer_set_text_alignment(sets1, GTextAlignmentCenter);
	text_layer_set_font(sets1, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(sets1));
	
	// sets2
	sets2 = text_layer_create(GRect(0, 90, 144, 20));
	text_layer_set_text_alignment(sets2, GTextAlignmentCenter);
	text_layer_set_font(sets2, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(sets2));
	
	// sets3
	sets3 = text_layer_create(GRect(0, 140, 144, 20));
	text_layer_set_text_alignment(sets3, GTextAlignmentCenter);
	text_layer_set_font(sets3, fonts_get_system_font(FONT_KEY_GOTHIC_18));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(sets3));
	
	// Now set all the text up just right
	int sets1_buf[5];
	int sets2_buf[5];
	int sets3_buf[5];
	
	Workout workout = get_workout(current_index, sets1_buf, sets2_buf, sets3_buf);
	update_text_layers(workout);
	APP_LOG(APP_LOG_LEVEL_INFO, "Loaded.");
}

/* Runs when the window is unloaded */
void old_entry_window_unload(Window *window){
	text_layer_destroy(date_text);
	text_layer_destroy(exercise1);
	text_layer_destroy(exercise2);
	text_layer_destroy(exercise3);
	text_layer_destroy(sets1);
	text_layer_destroy(sets2);
	text_layer_destroy(sets3);
	text_layer_destroy(weight1);
	text_layer_destroy(weight2);
	text_layer_destroy(weight3);
}

/*
 * Click Config providers
 */

/* Determines click callbacks */
void old_entry_window_click_config_provider(Window *window){}

/* Runs with a single up click */
void old_entry_window_single_up_click(ClickRecognizerRef recognizer, void *context){}

/* Runs with a single down click */
void old_entry_window_single_down_click(ClickRecognizerRef recognizer, void *context){}

/* Runs when the select button is held down for a while */
void old_entry_window_long_select_click(ClickRecognizerRef recognizer, void *context){}

/*
 * Helper functions
 */

/* Updates the text layers for the entire window */
void update_text_layers(Workout workout){
	APP_LOG(APP_LOG_LEVEL_INFO, "Updating the text layers");
	// First the date
	char date_buffer[] = "DD MMM YYYY";
	snprintf(date_buffer, sizeof(date_buffer), "%i %s %i", workout.day, int_to_month(workout.month), workout.year);
	text_layer_set_text(date_text, date_buffer);
	
	// Now the exercise layers
	char exercise_buffer1[] = "              ";
	snprintf(exercise_buffer1, sizeof(exercise_buffer1), "%s", workout.exercise1.name);
	text_layer_set_text(exercise1, exercise_buffer1);
	
	char exercise_buffer2[] = "              ";
	snprintf(exercise_buffer2, sizeof(exercise_buffer2), "%s", workout.exercise2.name);
	text_layer_set_text(exercise2, exercise_buffer2);
	
	char exercise_buffer3[] = "              ";
	snprintf(exercise_buffer3, sizeof(exercise_buffer3), "%s", workout.exercise3.name);
	text_layer_set_text(exercise3, exercise_buffer3);
	
	// weight layers
	char weight_buffer1[] = "999 lbs";
	snprintf(weight_buffer1, sizeof(weight_buffer1), "%i lbs", workout.exercise1.weight);
	text_layer_set_text(weight1, weight_buffer1);
	
	char weight_buffer2[] = "999 lbs";
	snprintf(weight_buffer2, sizeof(weight_buffer2), "%i lbs", workout.exercise2.weight);
	text_layer_set_text(weight2, weight_buffer2);
	
	char weight_buffer3[] = "999 lbs";
	snprintf(weight_buffer3, sizeof(weight_buffer3), "%i lbs", workout.exercise3.weight);
	text_layer_set_text(weight3, weight_buffer3);
	
	// Now the sets
	int sum1 = 0;
	int sum2 = 0;
	int sum3 = 0;
	
	for(int i = 0; i < 5; i++){
		sum1 += workout.exercise1.reps[i];
		sum2 += workout.exercise2.reps[i];
		sum3 += workout.exercise3.reps[i];
	}
	
	if(sum1 == 25)
		text_layer_set_text(sets1, "5 X 5");
	else {
		char buffer[] = "i i i i i";
		snprintf(buffer, sizeof(buffer), "%i %i %i %i %i", workout.exercise1.reps[0], workout.exercise1.reps[1], 
						 workout.exercise1.reps[2], workout.exercise1.reps[3], workout.exercise1.reps[4]);
		text_layer_set_text(sets1, buffer);
	}
	
	if(sum2 == 25)
		text_layer_set_text(sets2, "5 X 5");
	else {
		char buffer[] = "i i i i i";
		snprintf(buffer, sizeof(buffer), "%i %i %i %i %i", workout.exercise2.reps[0], workout.exercise2.reps[1], 
						 workout.exercise2.reps[2], workout.exercise2.reps[3], workout.exercise2.reps[4]);
		text_layer_set_text(sets2, buffer);
	}
	
	if(sum3 == 25)
		text_layer_set_text(sets3, "5 X 5");
	else {
		char buffer[] = "i i i i i";
		snprintf(buffer, sizeof(buffer), "%i %i %i %i %i", workout.exercise3.reps[0], workout.exercise3.reps[1], 
						 workout.exercise3.reps[2], workout.exercise3.reps[3], workout.exercise3.reps[4]);
		text_layer_set_text(sets3, buffer);
	}
	
	APP_LOG(APP_LOG_LEVEL_INFO, "Updated");
}






















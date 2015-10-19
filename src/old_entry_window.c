#include <pebble.h>
#include "old_entry_window.h"

/* old_entry_init function */
void old_entry_window_init(int *index){
	current_index = index;
	
	if(old_entry_window == NULL){
		old_entry_window = window_create();
		window_set_click_config_provider(old_entry_window, (ClickConfigProvider)old_entry_window_click_config_provider);
	}
	
	window_set_window_handlers(old_entry_window, (WindowHandlers){
		.load = old_entry_window_load,
		.unload = old_entry_window_unload
	});
	
	window_stack_push(old_entry_window, true);
}

/*
 * Window Handlers
 */

/* Runs when the window is loaded */
void old_entry_window_load(Window *window){
	
	// Create the date text layer
	date_text = text_layer_create(GRect(0, 0, 144, 30));
	text_layer_set_font(date_text, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(date_text, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(date_text));
	
	for(int i = 0; i < 3; i++){
		// Create the exercise text layer
		exercises[i] = text_layer_create(GRect(0, 30 + i * 40, 108, 20));
		text_layer_set_font(exercises[i], fonts_get_system_font(FONT_KEY_GOTHIC_18));
		text_layer_set_text_alignment(exercises[i], GTextAlignmentLeft);
		layer_add_child(window_get_root_layer(window), text_layer_get_layer(exercises[i]));
		
		// the weights text layer
		weights[i] = text_layer_create(GRect(108, 30 + i * 40, 36, 20));
		text_layer_set_font(weights[i], fonts_get_system_font(FONT_KEY_GOTHIC_18));
		text_layer_set_text_alignment(weights[i], GTextAlignmentRight);
		layer_add_child(window_get_root_layer(window), text_layer_get_layer(weights[i]));
		
		// the sets text layer
		sets[i] = text_layer_create(GRect(0, 50 + i * 40, 144, 20));
		text_layer_set_font(sets[i], fonts_get_system_font(FONT_KEY_GOTHIC_18));
		text_layer_set_text_alignment(sets[i], GTextAlignmentCenter);
		layer_add_child(window_get_root_layer(window), text_layer_get_layer(sets[i]));
	}
	
	// Go ahead and grab the workout info from persistent storage
	int sets1[5];
	int sets2[5];
	int sets3[5];
	
	Workout current_workout = get_workout(*current_index, sets1, sets2, sets3);
	
	// Then update the text layers
	update_text_layers(current_workout);
}

/* Runs when the window is unloaded */
void old_entry_window_unload(Window *window){
	text_layer_destroy(date_text);
	
	// Destroy all the other text layers
	for(int i = 0; i < 3; i++){
		text_layer_destroy(exercises[i]);
		text_layer_destroy(sets[i]);
		text_layer_destroy(weights[i]);
	}
}

/*
 * Click Config providers
 */

/* Determines click callbacks */
void old_entry_window_click_config_provider(Window *window){
	window_single_click_subscribe(BUTTON_ID_UP, (ClickHandler)old_entry_window_single_up_click);
	window_single_click_subscribe(BUTTON_ID_DOWN, (ClickHandler)old_entry_window_single_down_click);
}

/* Runs with a single up click */
void old_entry_window_single_up_click(ClickRecognizerRef recognizer, void *context){
	// If the current index is bigger than zero, then we will move down a workout and show that
	if(*current_index > 0){
		// TODO: this
	}
}

/* Runs with a single down click */
void old_entry_window_single_down_click(ClickRecognizerRef recognizer, void *context){
	// If there are more workouts, get the next one and show it
	if(*current_index < get_workout_count() - 1){
		// TODO: this
	}
}

/*
 * Helper functions
 */

int get_sum(const int *array, int array_length){
	int sum = 0;
	for(int i = 0; i < array_length; i++)
		sum += array[array_length];
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "sum: %i", sum);
	
	return sum;
}

/* Changes all of the text layers to look nice and stuff */
void update_text_layers(Workout workout){
	// Setup the buffers
	static char exercise_text[3][15] = {"Overhead Press", "Overhead Press", "Overhead Press"};
	static char weights_text[3][8] = {"999 lbs", "999 lbs", "999 lbs"};
	static char sets_text[3][10] = {"5 5 5 5 5", "5 5 5 5 5", "5 5 5 5 5"};
	static char date_buffer[] = "DD MMM YYYY";
	
	// snprinf'em
	snprintf(date_buffer, sizeof(date_buffer), "%i %s %i", workout.day, int_to_month(workout.month), workout.year);
	
	snprintf(exercise_text[0], sizeof(exercise_text[0]), "%s", workout.exercise1.name);
	snprintf(exercise_text[1], sizeof(exercise_text[1]), "%s", workout.exercise2.name);
	snprintf(exercise_text[2], sizeof(exercise_text[2]), "%s", workout.exercise3.name);
	
	snprintf(weights_text[0], sizeof(weights_text[0]), "%i lbs", workout.exercise1.weight);
	snprintf(weights_text[1], sizeof(weights_text[1]), "%i lbs", workout.exercise2.weight);
	snprintf(weights_text[2], sizeof(weights_text[2]), "%i lbs", workout.exercise3.weight);
	
	// We do something special with the sets layers
	int sum1 = get_sum(workout.exercise1.reps, 5);
	int sum2 = get_sum(workout.exercise2.reps, 5);
	int sum3 = get_sum(workout.exercise3.reps, 5);
	
	if(sum1 == 25){
		snprintf(sets_text[0], sizeof(sets_text[0]), "%s", "5x5");
	} else {
		snprintf(sets_text[0], sizeof(sets_text[0]), "%i %i %i %i %i", workout.exercise1.reps[0], workout.exercise1.reps[1],
						workout.exercise1.reps[2], workout.exercise1.reps[3], workout.exercise1.reps[4]);
	}
	
	if(sum2 == 25){
		snprintf(sets_text[1], sizeof(sets_text[1]), "%s", "5x5");
	} else {
		snprintf(sets_text[1], sizeof(sets_text[1]), "%i %i %i %i %i", workout.exercise2.reps[0], workout.exercise2.reps[1],
						workout.exercise2.reps[2], workout.exercise2.reps[3], workout.exercise2.reps[4]);
	}
	
	if(sum3 == 25){
		snprintf(sets_text[2], sizeof(sets_text[2]), "%s", "5x5");
	} else {
		snprintf(sets_text[2], sizeof(sets_text[2]), "%i %i %i %i %i", workout.exercise3.reps[0], workout.exercise3.reps[1],
						workout.exercise3.reps[2], workout.exercise3.reps[3], workout.exercise3.reps[4]);
	}
	
	// Store the text in their various layers
	text_layer_set_text(date_text, date_buffer);
	
	for(int i = 0; i < 3; i++){
		text_layer_set_text(exercises[i], exercise_text[i]);
		text_layer_set_text(weights[i], weights_text[i]);
		text_layer_set_text(sets[i], sets_text[i]);
	}
}



















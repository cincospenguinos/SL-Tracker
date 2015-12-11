#include <pebble.h>
#include "model.h"
#include "workout_window.h"

Window *workout_window; // the main workout window

/* Variables for the view */
static TextLayer *current_exercise_layer; // the workout that is currently being done
static TextLayer *weight_text; // the amount of weight for the current exercise
static TextLayer *timer; // the timer managing the various sets and reps
static TextLayer *sets[5]; // the collection of sets that are currently being managed
static InverterLayer *selected_set_layer; // the collection of layers that indicate what is the currently selected set
static TextLayer *motivation; // a text layer to provide motivation to the user

/* Variables for the model */
static Workout current_workout;

/* Variables to manage the view */
static bool timer_running; // dictates whether or not the timer is running

static int timer_count; // the current number of seconds on the timer
static int current_set_index; // the current set that the user is working on
static int current_exercise_index; // manages the current exercise

static char set_buffers[5][2]; // An array of buffers to manage the sets text layers
static char weight_buffer[8]; // Buffer to hold the weight of the various exercises

static bool finished_workout; // Indicates whether we need to save the workout or not
static bool failed_set; // Whether or not the user failed a set

/* Returns whatever the current exercise may be */
Exercise get_current_exercise();

/* Initializes and pushes this window */
void workout_window_init(){
	
	// Get the current workout information put together
	current_workout.day_type = get_workout_day_type();
	
	current_workout.exercise1.name = "Squats";
	current_workout.exercise1.weight = get_squat_weight();
	
	if(!current_workout.day_type){
		current_workout.exercise2.name = "Bench Press";
		current_workout.exercise2.weight = get_bench_weight();
		
		current_workout.exercise3.name = "Bent Rows";
		current_workout.exercise3.weight = get_bent_rows_weight();
	} 
	else {
		current_workout.exercise2.name = "Bench Press";
		current_workout.exercise2.weight = get_bench_weight();
		
		current_workout.exercise3.name = "Bent Rows";
		current_workout.exercise2.weight = get_bent_rows_weight();
	}
	
	// Set all of the values in each exercise to 5 to avoid any memory weirdness
	for(int i = 0; i < 5; i++){
		current_workout.exercise1.reps[i] = 5;
		current_workout.exercise2.reps[i] = 5;
		current_workout.exercise3.reps[i] = 5;
	}
	
	// Make sure the date is set properly
	time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);
	
	current_workout.day = tick_time->tm_mday;
	current_workout.month = tick_time->tm_mon;
	current_workout.year = tick_time->tm_year + 1900;
	
	current_exercise_index = 0;
	timer_count = 0;
	current_set_index = 0;
	finished_workout = false;
	
	// Create the window and define the handlers
	if(workout_window == NULL)
		workout_window = window_create();
	
	window_set_window_handlers(workout_window, (WindowHandlers){
		.load = workout_window_load,
		.unload = workout_window_unload
	});
	
	// Set up the clicks
	window_set_click_config_provider(workout_window, (ClickConfigProvider) workout_window_click_config_provider);
	
	// Push it onto the stack
	window_stack_push(workout_window, true);
}

/* Window handlers */
void workout_window_load(Window *window){
	// current_exercise text layer
	current_exercise_layer = text_layer_create(GRect(0, 0, 144, 20));
	text_layer_set_text(current_exercise_layer, "Squat");
	text_layer_set_font(current_exercise_layer, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text_alignment(current_exercise_layer, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(current_exercise_layer));
	
	// weight text layer
	weight_text = text_layer_create(GRect(36, 20, 72, 16));
	text_layer_set_font(weight_text, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	text_layer_set_text_alignment(weight_text, GTextAlignmentCenter);
	update_weight_text();
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(weight_text));
	
	// timer text layer
	timer = text_layer_create(GRect(36, 36, 72, 30));
	text_layer_set_font(timer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(timer, GTextAlignmentCenter);
	text_layer_set_text(timer, "0:00");
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(timer));
	
	// sets text layers
	for(int i = 0; i < 5; i++){
		sets[i] = text_layer_create(GRect(10 + (i * 28), 80, 16, 20));
		text_layer_set_font(sets[i], fonts_get_system_font(FONT_KEY_GOTHIC_14));
		text_layer_set_text_alignment(sets[i], GTextAlignmentCenter);
		text_layer_set_text(sets[i], "5");
		layer_add_child(window_get_root_layer(window), text_layer_get_layer(sets[i]));
	}
	
	// selected set layer to show what is the current working set
	selected_set_layer = inverter_layer_create(GRect(10, 80, 16, 20));
	layer_add_child(window_get_root_layer(window), inverter_layer_get_layer(selected_set_layer));
	
	// motivation text layer
	motivation = text_layer_create(GRect(12, 104, 120, 164));
	text_layer_set_font(motivation, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	text_layer_set_text_alignment(motivation, GTextAlignmentLeft);
	text_layer_set_text(motivation, "");
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(motivation));
	
	// add a tick timer to this window
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
	
	// Update the text layers
	update_rep_text();
	update_weight_text();
}

void workout_window_unload(Window *window){
	text_layer_destroy(current_exercise_layer);
	text_layer_destroy(weight_text);
	text_layer_destroy(timer);
	
	for(int i = 0; i < 5; i++){
		text_layer_destroy(sets[i]);
	}
	
	inverter_layer_destroy(selected_set_layer);
	text_layer_destroy(motivation);
	
	tick_timer_service_unsubscribe();
	
	// Store the workout if we finished it
	if(finished_workout)
		store_new_workout(current_workout);
}

/* Click functions */
void workout_window_click_config_provider(Window *window){
	window_single_click_subscribe(BUTTON_ID_UP, workout_window_single_up_click);
	window_single_click_subscribe(BUTTON_ID_DOWN, workout_window_single_down_click);
	window_single_click_subscribe(BUTTON_ID_SELECT, workout_window_single_select_click);
	window_single_click_subscribe(BUTTON_ID_BACK, workout_window_single_back_click);
}

void workout_window_single_up_click(ClickRecognizerRef recognizer, void *context){
	
	// Increment if needed
	switch(current_exercise_index){
		case 0:
			if(current_workout.exercise1.reps[current_set_index] < 5)
				current_workout.exercise1.reps[current_set_index] += 1;
			break;
		case 1:
			if(current_workout.exercise2.reps[current_set_index] < 5)
				current_workout.exercise2.reps[current_set_index] += 1;
			break;
		default:
			if(current_workout.exercise3.reps[current_set_index] < 5)
				current_workout.exercise3.reps[current_set_index] += 1;
			break;
	}
	
	update_rep_text();
}

void workout_window_single_down_click(ClickRecognizerRef recognizer, void *context){
		// Decrement if needed
	switch(current_exercise_index){
		case 0:
			if(current_workout.exercise1.reps[current_set_index] > 0)
				current_workout.exercise1.reps[current_set_index] -= 1;
			break;
		case 1:
			if(current_workout.exercise2.reps[current_set_index] > 0)
				current_workout.exercise2.reps[current_set_index] -= 1;
			break;
		default:
			if(current_workout.exercise3.reps[current_set_index] > 0)
				current_workout.exercise3.reps[current_set_index] -= 1;
			break;
	}
	
	update_rep_text();
}

void workout_window_single_select_click(ClickRecognizerRef recognizer, void *context){
	timer_running = true;
	timer_count = 0;
	
	// Indicate whether or not we failed a set
	if(get_current_exercise().reps[current_set_index] != 5)
		failed_set = true;
	else
		failed_set = false;
	
	// If we've finished deadlifts, then just change the current_set_index and call it a day
	if(current_workout.day_type && current_exercise_index == 2)
		current_set_index = 4;
	
	if(++current_set_index == 5){
		// This runs if we've completed an exercise
		current_set_index = 0;
		failed_set = false;
		timer_running = false;
		timer_count = 0;
		text_layer_set_text(motivation, "");
		
		if(++current_exercise_index == 3){
			// This runs if we've completed the workout
			finished_workout = true;
			window_stack_pop(true);
			return;
		} else {
			// This runs if we still have other exercises
			update_exercise_text();
			update_weight_text();
			update_rep_text();
		}
	} else {
		// This runs if we just moved sets
		update_motivation_text();
	}
	
	update_timer();
	update_working_set_inverter_layer();
}

void workout_window_single_back_click(ClickRecognizerRef recognizer, void *context){
	
	// If we were on deadlifts, then just reset the current_set_index and call it a day
	if(current_workout.day_type && current_exercise_index == 2)
		current_set_index = 0;
	
	if(--current_set_index < 0){
		// This runs if we are going back from an exercise
		
		current_set_index = 4;
		
		if(--current_exercise_index < 0){
			// This runs if we are exiting the workout
			finished_workout = false;
			window_stack_pop(true);
			return;
		} else {
			// This runs if we are looking at other exercises
			update_exercise_text();
			update_weight_text();
			update_rep_text();
		}
	}
	
	update_working_set_inverter_layer();
}

/* Layer management functions */
void update_rep_text(){
	Exercise current_exercise = get_current_exercise();
	snprintf(set_buffers[current_set_index], sizeof(set_buffers[current_set_index]), "%i", current_exercise.reps[current_set_index]);
	text_layer_set_text(sets[current_set_index], set_buffers[current_set_index]);
}

void update_working_set_inverter_layer(){
	inverter_layer_destroy(selected_set_layer);
	selected_set_layer = inverter_layer_create(GRect(10 + (current_set_index * 28), 80, 16, 20));
	layer_add_child(window_get_root_layer(workout_window), inverter_layer_get_layer(selected_set_layer));
}

void update_motivation_text(){
	switch(timer_count){
		case 0:
			if(!failed_set)
				text_layer_set_text(motivation, "Nice job! If it was easy, wait 90 seconds. Otherwise, wait 180.");
			else
				text_layer_set_text(motivation, "Failure is part of the game. Wait five minutes.");
			break;
		case 90:
			if(!failed_set){
				text_layer_set_text(motivation, "90 seconds have passed. Move on to your next set.");
				vibes_short_pulse();
			}
			break;
		case 180:
			if(!failed_set){
				text_layer_set_text(motivation, "3 minutes have passed. Move on to your next set.");
				vibes_long_pulse();
			}
			break;
		case 300:
			failed_set = false;
			vibes_long_pulse();
			text_layer_set_text(motivation, "5 minutes have passed. Move on to your next set.");
			break;
		case 600:
			vibes_long_pulse();
			text_layer_set_text(motivation, "10 minutes have passed. Move on to your next set.");
			break;
		case 900:
			timer_count = 0;
			timer_running = false;
			vibes_double_pulse();
			text_layer_set_text(motivation, "");
			break;
	}
}

Exercise get_current_exercise(){
	if(current_exercise_index < 0 || current_exercise_index > 2)
		APP_LOG(APP_LOG_LEVEL_ERROR, "current_exercise_index is not 0, 1, or 2!");
	
	switch(current_exercise_index){
		case 0:
			return current_workout.exercise1;
		case 1:
			return current_workout.exercise2;
		default:
			return current_workout.exercise3;
	}
}

const char * get_exercise_text(){
	switch(current_exercise_index){
		case 0:
			return "Squats";
		case 1:
			if(!current_workout.day_type) // A day
				return "Bench Press";
			else
				return "Overhead Press";
		case 2:
			if(!current_workout.day_type) // A day
				return "Bent Rows";
			else
				return "Deadlift";
	}
	
	APP_LOG(APP_LOG_LEVEL_ERROR, "Requested exercise text that does not exist!");
	return "";
}

void update_exercise_text(){
	// If it's currently a B day and we're working on deadlifts, we need to do some things
	if(current_workout.day_type && current_exercise_index == 2){
		current_set_index = 2; // First we will need to indicate that the current working set is 2
		
		// Set all the sets text layers to be empty
		text_layer_set_text(sets[0], " ");
		text_layer_set_text(sets[1], " ");
		text_layer_set_text(sets[3], " ");
		text_layer_set_text(sets[4], " ");
	} else {
		// Get the old values of this exercise and show them
		for(int i = 0; i < 5; i++){
			snprintf(set_buffers[i], sizeof(set_buffers[i]), "%i", get_current_exercise().reps[i]);
			text_layer_set_text(sets[i], set_buffers[i]);
		}
	}
	
	text_layer_set_text(current_exercise_layer, get_exercise_text());
}

void update_weight_text(){
	switch(current_exercise_index){
		case 0:
			snprintf(weight_buffer, sizeof(weight_buffer), "%i lbs", current_workout.exercise1.weight);
		case 1:
			snprintf(weight_buffer, sizeof(weight_buffer), "%i lbs", current_workout.exercise2.weight);
		case 2:
			snprintf(weight_buffer, sizeof(weight_buffer), "%i lbs", current_workout.exercise3.weight);
	}
	
	text_layer_set_text(weight_text, weight_buffer);
}

/* Time functions */
void tick_handler(struct tm *tick_time, TimeUnits units_changed){
	if(timer_running){
		timer_count++;
		update_timer();
		
		if(timer_count % 90 == 0 || timer_count % 100 == 0 || timer_count == 999)
			update_motivation_text();
	}
}

void update_timer(){
	// Create a little buffer
	static char buffer[] = "00:00";
	
	// calculate the number of seconds and minutes
	int minutes = timer_count / 60;
	int seconds = timer_count % 60;
	
	
	// Throw in the elapsed time
	if(seconds < 10)
		snprintf(buffer, sizeof(buffer), "%i:0%i", minutes, seconds);
	else
		snprintf(buffer, sizeof(buffer), "%i:%i", minutes, seconds);
	
	// Set the text up
	text_layer_set_text(timer, buffer);
}
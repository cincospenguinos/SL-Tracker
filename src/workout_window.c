#include <pebble.h>
#include "model.h"
#include "workout_window.h"

Window *workout_window; // the main workout window

/* Variables for the view */
static TextLayer *current_exercise; // the workout that is currently being done
static TextLayer *weight_text; // the amount of weight for the current exercise
static TextLayer *timer; // the timer managing the various sets and reps
static TextLayer *sets[5]; // the collection of sets that are currently being managed
static InverterLayer *selected_set_layer; // the collection of layers that indicate what is the currently selected set
static TextLayer *motivation; // a text layer to provide motivation to the user

/* Variables for the model */
static Workout current_workout;

WorkoutWindowWorkout some_workout;

/* Variables to manage the view */
static bool timer_running; // dictates whether or not the timer is running

static int timer_count; // the current number of seconds on the timer
static int current_set_index; // the current set that the user is working on
static int current_exercise_index; // manages the current exercise

static char set_buffers[5][2]; // An array of buffers to manage the sets text layers
static char weight_buffer[8]; // Buffer to hold the weight of the various exercises

static bool finished_workout; // Indicates whether we need to save the workout or not

/* Returns whatever the current exercise may be */
Exercise* get_current_exercise();

/* Initializes and pushes this window */
void workout_window_init(){
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Default values: %i", current_workout.exercise1.reps[0]);
	
	// Get the current workout information put together
	current_workout.day_type = get_workout_day_type();
	
	current_workout.exercise1.name = "Squats";
	current_workout.exercise1.weight = get_squat_weight();
	
	if(!current_workout.day_type){
		current_workout.exercise2.name = "Bench Press";
		current_workout.exercise2.weight = get_bench_weight();
		
		current_workout.exercise3.name = "Bent Rows";
		current_workout.exercise2.weight = get_bent_rows_weight();
	} 
	else {
		current_workout.exercise2.name = "Bench Press";
		current_workout.exercise2.weight = get_bench_weight();
		
		current_workout.exercise3.name = "Bent Rows";
		current_workout.exercise2.weight = get_bent_rows_weight();
	}
	
	// Set all of the values in each exercise to 0 to avoid any memory weirdness
	for(int i = 0; i < 5; i++){
		current_workout.exercise1.reps[i] = 0;
		current_workout.exercise2.reps[i] = 0;
		current_workout.exercise3.reps[i] = 0;
	}
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "After resetting values reps: %i", current_workout.exercise1.reps[0]);
	
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
	current_exercise = text_layer_create(GRect(0, 0, 144, 20));
	text_layer_set_text(current_exercise, "Squat");
	text_layer_set_font(current_exercise, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text_alignment(current_exercise, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(current_exercise));
	
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
}

void workout_window_unload(Window *window){
	text_layer_destroy(current_exercise);
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
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Reps: %i", current_workout.exercise1.reps[0]);
	
	Exercise current_exercise = *get_current_exercise();
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "From exercise pointer: %i", current_exercise.reps[0]);
	
	if(current_exercise.reps[current_set_index] < 5)
		current_exercise.reps[current_set_index]++;
	
	update_rep_text();
}

void workout_window_single_down_click(ClickRecognizerRef recognizer, void *context){
	Exercise current_exercise = *get_current_exercise();
	
	if(current_exercise.reps[current_set_index] > 0)
		current_exercise.reps[current_set_index]--;
	
	update_rep_text();
}

void workout_window_single_select_click(ClickRecognizerRef recognizer, void *context){}

void workout_window_single_back_click(ClickRecognizerRef recognizer, void *context){
	// TODO: Is this all we need to do?
	if(--current_set_index < 0)
		window_stack_pop(true);
}

/* Layer management functions */
void update_rep_text(){
	Exercise current_exercise = *get_current_exercise();
	snprintf(set_buffers[current_set_index], sizeof(set_buffers[current_set_index]), "%i", current_exercise.reps[current_set_index]);
	text_layer_set_text(sets[current_set_index], set_buffers[current_set_index]);
	
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Updating the rep text --> %i", current_exercise.reps[current_set_index]);
}
void update_working_set_inverter_layer(){}
void update_motivation_text(){}

Exercise* get_current_exercise(){
	switch(current_exercise_index){
		case 0:
			return &current_workout.exercise1;
		case 1:
			return &current_workout.exercise2;
		case 2:
			return &current_workout.exercise3;
		default:
			APP_LOG(APP_LOG_LEVEL_ERROR, "current_exercise_index was not 0, 1, or 2!");
			return NULL;
	}
}

const char * get_exercise_text(){
	return ""; // TODO: Implement this
}
void update_exercise_text(){}

void update_weight_text(){}

/* Time functions */
void tick_handler(struct tm *tick_time, TimeUnits units_changed){}
void update_timer(){}
/**
* workout_window.h
*
* Window managing the workout timer and counts the number of sets.
*/
#pragma once
#include <pebble.h>
	
/* Instance variables */
Window *workout_window; // the main workout window

TextLayer *current_exercise; // the workout that is currently being done
TextLayer *weight_text; // the amount of weight for the current exercise
TextLayer *timer; // the timer managing the various sets and reps
TextLayer *sets[5]; // the collection of sets that are currently being managed
InverterLayer *selected_set_layer; // the collection of layers that indicate what is the currently selected set
TextLayer *motivation; // a text layer to provide motivation to the user

/* Variables to manage the view */
bool timer_running; // dictates whether or not the timer is running
bool day_type; // false for A day, true for B day

int timer_count; // the current number of seconds on the timer
int current_rep_count; // the current number of reps the user has listed
int current_working_set; // the current set that the user is working on
int current_exercise_index; // manages the current exercise

char buffers[5][2]; // An array of buffers to manage the sets text layers
int reps_per_exercise[3][5]; // An array of integers to manage the number of reps for each set in each exercise

// TODO: Figure out how to store all of the data with the reps and show it back to the user

bool on_deadlifts; // True if the user is currently working on deadlifts
bool failed_set; // True if the user failed a set

/* Initializes and pushes this window */
void workout_window_init(bool);

/* Window handlers */
void workout_window_load(Window *window);
void workout_window_unload(Window *window);

/* Click functions */
void workout_window_click_config_provider(Window *window);
void workout_window_single_up_click(ClickRecognizerRef recognizer, void *context);
void workout_window_single_down_click(ClickRecognizerRef recognizer, void *context);
void workout_window_single_select_click(ClickRecognizerRef recognizer, void *context);
void workout_window_single_back_click(ClickRecognizerRef recognizer, void *context);
void workout_window_long_back_click(ClickRecognizerRef recognizer, void *context);

/* Layer management functions */
void update_rep_text();
void update_working_set_inverter_layer();
void update_motivation_text();

const char * get_exercise_text();
void update_exercise_text();

/* Time functions */
void tick_handler(struct tm *tick_time, TimeUnits units_changed);
void update_timer();
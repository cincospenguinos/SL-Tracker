/**
* workout_window.h
*
* Window managing the workout timer and counts the number of sets.
*/
#pragma once
#include <pebble.h>
	
/* Instance variables */
Window *workout_window; // the main workout window

TextLayer *current_workout; // the workout that is currently being done
TextLayer *timer; // the timer managing the various sets and reps
TextLayer *sets[5]; // the collection of sets that are currently being managed
TextLayer *motivation; // a text layer to provide motivation to the user

// TODO: Add triangles to go above each of the sets

/* Variables to manage the view */
bool timer_running; // dictates whether or not the timer is running
int timer_count; // the current number of seconds on the timer
int current_rep_count; // the current number of reps the user has listed
int current_working_set; // the current set that the user is working on
char buffers[5][3]; // An array of buffers to manage the sets text layers

/* Initializes and pushes this window */
void workout_window_init();

/* Window handlers */
void workout_window_load(Window *window);
void workout_window_unload(Window *window);

/* Drawing procedures */
void selected_triangle_drawing_procedure(Layer *layer, GContext *ctx);

/* Click functions */
void workout_window_click_config_provider(Window *window);
void workout_window_single_up_click(ClickRecognizerRef recognizer, void *context);
void workout_window_single_down_click(ClickRecognizerRef recognizer, void *context);
void workout_window_single_select_click(ClickRecognizerRef recognizer, void *context);
void workout_window_single_back_click(ClickRecognizerRef recognizer, void *context);
void workout_window_long_back_click(ClickRecognizerRef recognizer, void *context);

void update_rep_text();

/* Time functions */
void tick_handler(struct tm *tick_time, TimeUnits units_changed);
void update_timer();
/**
* workout_window.h
*
* Window managing the workout timer and counts the number of sets.
*/
#pragma once
#include <pebble.h>
	
/* Instance variables */
extern Window *workout_window; // the main workout window

/* Initializes and pushes this window */
void workout_window_init();

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

void update_weight_text();

/* Time functions */
void tick_handler(struct tm *tick_time, TimeUnits units_changed);
void update_timer();
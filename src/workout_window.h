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

bool timer_running; // indicates whether the timer should be counting up or not
int timer_count; // the current value of the timer

/* Window handlers */
void workout_window_load(Window *window);
void workout_window_unload(Window *window);

/* Drawing procedures */
void selected_triangle_drawing_procedure(Layer *layer, GContext *ctx);

/* Time functions */
void tick_handler(struct tm *tick_time, TimeUnits units_changed);
void update_timer();
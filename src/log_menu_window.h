/**
 * log_menu_window.h
 *
 * The menu that shows all of the old workouts
 */

#pragma once
#include <pebble.h>
#include "model.h"


/*
 * Instance variables
 */

Window *log_menu_window; // The window itself

int old_workout_count; // The total number of menu items that we will show

SimpleMenuLayer *log_menu; // The menu layer that will show all of the old workouts
SimpleMenuItem old_workouts[old_workout_count]; // This will be a pointer to an array of all of the items we need
SimpleMenuSection sections[1]; // We will only have one single section

/* log_menu_window init function */
void log_menu_window_init(SimpleMenuItem *items);

/*
 * Window Handlers
 */

/* Called when the window is loaded onto the stack */
void log_menu_window_load(Window *window);

/* Called when the window is popped off the stack */
void log_menu_window_unload(Window *window);

/* Called when an old workout is selected to be viewed */
void old_workout_selected_callback(int index, void *ctx);
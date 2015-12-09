#pragma once
#include <pebble.h>

// TODO: What do for indicating to the user what the weight is

extern Window *set_weight_window; // The window holding it all together

/*
 * NOTE: Here is how the exercises will work
 *
 * 1 - Squat
 * 2 - Bench
 * 3 - Bent Rows
 * 4 - Overhead Press
 * 5 - Deadlift
 *
 * The exercise type passed into the init function determines what weight is being changed.
 */

/* The initializer */
void set_weight_window_init(int exercise_type);

/* Load/Unload functions */
void set_weight_window_load(Window *window);
void set_weight_window_unload(Window *window);

/* The window click config provider */
void set_weight_window_config_provider(Window *window);

/* Up and down click handlers */
void set_weight_window_up_single_click_handler(ClickRecognizerRef recognizer, void *context);
void set_weight_window_down_single_click_handler(ClickRecognizerRef recognizer, void *context);

/* Helper function to facilitate getting the weight for an exercise */
void get_weight_of(int exercise_type);

/* Helper function to help set the text */
void set_text_layer_text();

/* Helper function that goes and sets the weight of the current exercise to the model */
void set_weight_of_exercise();
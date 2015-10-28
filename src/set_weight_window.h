/**
 * set_weight_window.h
 *
 * Window that is used to set the weight for some given workout.
 */
#pragma once
#include <pebble.h>

/* Instance variables */

Window *set_weight_window; // The window holding everything

TextLayer *exercise_name; // The name of the workout
TextLayer *exercise_weight; // The weight associated with this workout

BitmapLayer *up_arrow_layer; // Shows an up arrow
BitmapLayer *down_arrow_layer; // Shows a down arrow

GBitmap *up_arrow_bitmap; // The up arrow bitmap
GBitmap *down_arrow_bitmap; // The down arrow bitmap

int current_weight; // The current weight of this exercise
int current_exercise; // The current exercise

/* Init function and window handlers */
void set_weight_window_init(int);

void set_weight_window_load(Window *);
void set_weight_window_unload(Window *);

/*
 * Click providers
 */

void set_weight_window_click_provider(Window *);
void single_up_click_handler(ClickRecognizerRef recognizer, void *context);
void single_down_click_handler(ClickRecognizerRef recognizer, void *context);

/*
 * Helper functions
 */

void set_weight_update_text();
void get_proper_weight();
void save_proper_weight();
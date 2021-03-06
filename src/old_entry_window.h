/**
 * old_entry_window.h
 *
 * Window that shows an old workout. All it needs is an index to the model.
 */
#pragma once
#include <pebble.h>
#include "model.h"

extern Window *old_entry_window; // The window that shows the old workout

/* old_entry_init function */
void old_entry_window_init(int index); // The index to the workout that we are checking out

/*
 * Window Handlers
 */

/* Runs when the window is loaded */
void old_entry_window_load(Window *window);

/* Runs when the window is unloaded */
void old_entry_window_unload(Window *window);

/*
 * Click Config providers
 */

/* Determines click callbacks */
void old_entry_window_click_config_provider(Window *window);

/* Runs with a single up click */
void old_entry_window_single_up_click(ClickRecognizerRef recognizer, void *context);

/* Runs with a single down click */
void old_entry_window_single_down_click(ClickRecognizerRef recognizer, void *context);

/* Runs when the select button is held down for a while */
void old_entry_window_long_select_click(ClickRecognizerRef recognizer, void *context);

/*
 * Helper functions
 */

/* Changes all of the text layers to look nice and stuff */
void update_text_layers(Workout workout);

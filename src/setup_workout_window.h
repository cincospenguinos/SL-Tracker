/**
 * setup_workout_window.h
 *
 * The window that permits the user to setup a workout ahead of time. Change weights, select day, etc.
 */

#pragma once
#include <pebble.h>

Window *setup_workout_window; // The window we are going to use

SimpleMenuLayer *setup_workout_menu; // The menu that will be used to show everything to the user

SimpleMenuItem setup_workout_menu_items[6]; // The various menu items for this menu
SimpleMenuSection sections[1]; // the various sections - just one in our case

/* The init function */
void setup_workout_window_init();

/*
 * Window Handlers
 */

void setup_workout_window_load(Window *);
void setup_workout_window_unload(Window *);

/*
 * Select callbacks
 */

void setup_workout_window_select_callback(int index, void *ctx);
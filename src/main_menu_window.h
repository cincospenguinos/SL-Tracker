/**
 * main_menu_window.h
 *
 * The top window that is shown when the user starts the app.
 */

#pragma once
#include<pebble.h>
#include "model.h"

/*
 * Instance variables
 */

Window *main_menu_window; // The window itself

SimpleMenuLayer *main_menu_layer; // The menu layer that will contain everything
SimpleMenuSection sections[1]; // We will only have one single section because that's all we need here
SimpleMenuItem menu_items[3]; // The collection of items listed on the menu

/* Init function */
void main_menu_window_init();

/* 
 * Window Handlers
 */

/* Called when the main_menu_window loads */
void main_menu_window_load();

/* Called when the main_menu_window unloads */
void main_menu_window_unload();

/*
 * Selection callbacks
 */

/* Called when a new workout is requested */
void new_workout_callback();

/* Called when the user wants to setup the workout */
void setup_workout_callback();

/* Called when the user wants to check the old workouts he/she has */
void view_log_callback();
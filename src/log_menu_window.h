/**
 * log_menu_window.h
 *
 * Contains all the old workouts for the application. Loads them up dynamically.
 */
#pragma once
#include <pebble.h>
#include "model.h"

Window *log_menu_window; // The window that houses it all
MenuLayer *log_menu_layer; // The layer that houses it all

WorkoutPeek *workouts; // The collection of workouts that we need to worry about to avoid slowness when redrawing
int workout_count; // The total number of workouts

/* log_menu_init() function */
void log_menu_init();

/*
 * Window Handlers
 */

void log_menu_window_load(Window *window);

void log_menu_window_unload(Window *window);

/*
 * Callbacks - so many callbacks...
 */

/* Returns the number of sections for the menu */
uint16_t  get_num_sections_callback(MenuLayer *layer, void *data);

/* Returns the number of rows (or old workouts) */
uint16_t  get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);

/* Returns the header height */
int16_t  get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);

/* Draws the header of the section or whatever */
void draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data);

/* Draws the row at the index given */
void draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);

/* Runs when a row is selected */
void row_selected_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);

/*
 * Helper methods
 */

/* Returns three letter code to represent the month */
const char * int_to_month(int month);
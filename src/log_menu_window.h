/**
 * log_menu_window.h
 *
 * Contains all the old workouts for the application. Loads them up dynamically.
 */
#pragma once
#include <pebble.h>

Window *log_menu_window; // The window that houses it all

MenuLayer *log_menu_layer; // The layer that houses it all

/* log_menu_init() function */
void log_menu_init();

/*
 * Window Handlers
 */

void log_menu_load();

void log_menu_unload();

/*
 * Callbacks - so many callbacks...
 */

/* Returns the number of sections for the menu */
int get_num_sections_callback(MenuLayer *layer, void *data);

/* Returns the number of rows (or old workouts) */
int get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);

/* Returns the header height */
int get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);

/* Draws the row at the index given */
void draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);

/* Runs when a row is selected */
void row_selected_drawback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);
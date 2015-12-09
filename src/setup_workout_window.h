/**
 * setup_workout_window.h
 *
 * The window that permits the user to setup a workout ahead of time. Change weights, select day, etc.
 */

#pragma once
#include <pebble.h>

extern Window *setup_workout_window; // The window we are going to use

/* The init function */
void setup_workout_window_init();

/*
 * Window Handlers
 */

void setup_workout_window_load(Window *);
void setup_workout_window_unload(Window *);

/*
 * All of the callbacks
 */

uint16_t setup_workout_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data);
uint16_t setup_workout_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
int16_t setup_workout_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
void setup_workout_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data);
void setup_workout_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);
void setup_workout_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);
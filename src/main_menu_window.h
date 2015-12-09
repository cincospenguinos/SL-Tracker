/**
 * main_menu_window.h
 *
 * The top window that is shown when the user starts the app.
 */

#pragma once
#include<pebble.h>

/*
 * Instance variables
 */

extern Window *main_menu_window; // The window itself

/* Init function */
void main_menu_window_init();

/* 
 * Window Handlers
 */

/* Called when the main_menu_window loads */
void main_menu_window_load();

/* Called when the main_menu_window unloads */
void main_menu_window_unload();

uint16_t main_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data);
uint16_t main_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
int16_t main_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data);
void main_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data);
void main_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data);
void main_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data);

/*
 * Selection callbacks
 */

/* Called when a new workout is requested */
void create_new_workout();

/* Called when the user wants to setup the workout */
void setup_next_workout();

/* Called when the user wants to check the old workouts he/she has */
void view_workout_log();

/* Called when the user requests to nuke the whales */
void delete_workout_log();
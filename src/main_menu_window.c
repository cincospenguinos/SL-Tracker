#include <pebble.h>
#include "main_menu_window.h"
#include "workout_window.h"
#include "model.h"
#include "log_menu_window.h"
#include "setup_workout_window.h"

/* Init function */
void main_menu_window_init(){	
	main_menu_window = window_create();
	
	window_set_window_handlers(main_menu_window, (WindowHandlers){
		.load = main_menu_window_load,
		.unload = main_menu_window_unload
	});
	
	window_stack_push(main_menu_window, true);
}

/* 
 * Window Handlers
 */

/* Called when the main_menu_window loads */
void main_menu_window_load(Window *window){
	// Create the main menu layer
	main_menu_layer = menu_layer_create(layer_get_bounds(window_get_root_layer(window)));
	
	menu_layer_set_callbacks(main_menu_layer, NULL, (MenuLayerCallbacks){
		.get_num_sections = main_menu_get_num_sections_callback,
		.get_num_rows = main_menu_get_num_rows_callback,
		.get_header_height = main_menu_get_header_height_callback,
		.draw_header = main_menu_draw_header_callback,
		.draw_row = main_menu_draw_row_callback,
		.select_click = main_menu_select_callback
	});
	
	menu_layer_set_click_config_onto_window(main_menu_layer, window);
	
	layer_add_child(window_get_root_layer(window), menu_layer_get_layer(main_menu_layer));
}

/* Called when the main_menu_window unloads */
void main_menu_window_unload(Window *window){
	menu_layer_destroy(main_menu_layer);
	
	if(workout_window != NULL)
		window_destroy(workout_window);
}

uint16_t main_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data){
	return 1; // Just one section
}

uint16_t main_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
	return 4; // Four different selections to make
}

int16_t main_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
	return MENU_CELL_BASIC_HEADER_HEIGHT;
}

void main_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data){
	menu_cell_basic_header_draw(ctx, cell_layer, "SL TRACKER");
}

void main_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data){
	switch(cell_index->row){
		case 0:
			menu_cell_basic_draw(ctx, cell_layer, "Start Workout", "Start a new workout", NULL);
			break;
		case 1:
			menu_cell_basic_draw(ctx, cell_layer, "Set Workout", "Setup next workout", NULL);
			break;
		case 2:
			if(get_workout_count() != 0)
				menu_cell_basic_draw(ctx, cell_layer, "View Log", "View old workouts", NULL);
			else
				menu_cell_basic_draw(ctx, cell_layer, "View Log", "No workouts to show!", NULL);
			break;
		case 3:
			menu_cell_basic_draw(ctx, cell_layer, "Delete Log", "Delete all old workouts", NULL);
			break;
		default:
			break;
	}
}

void main_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data){
	switch(cell_index->row){
		case 0:
			create_new_workout();
			break;
		case 1:
			setup_next_workout();
		  break;
		case 2:
			view_workout_log();
			break;
		case 3:
			delete_workout_log();
	}
}


/*
 * Selection callbacks
 */

/* Called when a new workout is requested */
void create_new_workout(){
	workout_window_init();
}

/* Called when the user wants to setup the workout */
void setup_next_workout(){
	setup_workout_window_init();
}

/* Called when the user wants to check the old workouts he/she has */
void view_workout_log(){
	if(get_workout_count() != 0)
		log_menu_init();
	else {
		// TODO: Indicate that there are no workouts?
	}
}

/* Called when the user requests to nuke the whales */
void delete_workout_log(){
	// TODO: This
}
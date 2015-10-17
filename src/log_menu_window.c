#include <pebble.h>
#include "log_menu_window.h"
#include "model.h"

/* log_menu_init() function */
void log_menu_init(){
	if(log_menu_window == NULL)
		log_menu_window = window_create();
	
	window_set_window_handlers(log_menu_window, (WindowHandlers){
		.load = log_menu_window_load,
		.unload = log_menu_window_unload
	});
	
	window_stack_push(log_menu_window, true);
}

/*
 * Window Handlers
 */

void log_menu_window_load(Window *window){
	// Create and setup the menu_layer
	log_menu_layer = menu_layer_create(layer_get_frame(window_get_root_layer(window)));
	menu_layer_set_callbacks(log_menu_layer, NULL, (MenuLayerCallbacks){
		.get_num_sections =  get_num_sections_callback,
		.get_num_rows = get_num_rows_callback,
		.get_header_height = get_header_height_callback,
		.draw_header = draw_header_callback,
		.draw_row = draw_row_callback,
		.select_click = row_selected_callback
	});
	
	// Subscribe the clicks to this window
	menu_layer_set_click_config_onto_window(log_menu_layer, window);
	
	// Add the layer to the window
	layer_add_child(window_get_root_layer(window), menu_layer_get_layer(log_menu_layer));
}

void log_menu_window_unload(Window *window){
	menu_layer_destroy(log_menu_layer);
}

/*
 * Callbacks - so many callbacks...
 */

/* Returns the number of sections for the menu */
uint16_t get_num_sections_callback(MenuLayer *layer, void *data){
	return 1;
}

/* Returns the number of rows (or old workouts) */
uint16_t get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
	return get_workout_count();
}

/* Returns the header height */
int16_t get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
	return MENU_CELL_BASIC_HEADER_HEIGHT;
}

/* Draws the header of the section or whatever */
void draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data){
	menu_cell_basic_header_draw(ctx, cell_layer, "Old Workouts");
}

/* Draws the row at the index given */
void draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data){	
	// Get the workout peek structure from wherever the index is
	WorkoutPeek peek = peek_workout(cell_index->row);
	
	// Throw it into a buffer to write out the title and subtitle
	char title_buffer[] = "DD MMM YYYY - T";
	char day = 'A';
	
	if(peek.day_type)
		day = 'B';
	
	snprintf(title_buffer, sizeof(title_buffer), "%i %s %i - %c", peek.day, int_to_month(peek.month), peek.year, day);
	menu_cell_basic_draw(ctx, cell_layer, title_buffer, "", NULL);
}

/* Runs when a row is selected */
void row_selected_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data){
	// TODO: Implement this
}

/*
 * Helper functions
 */

/* Converts an integer to a three letter code representing the month */
const char * int_to_month(int month){
	switch(month){
		case 0:
			return "JAN";
		case 1:
			return "FEB";
		case 2:
			return "MAR";
		case 3:
			return "APR";
		case 4:
			return "MAY";
		case 5:
			return "JUN";
		case 6:
			return "JUL";
		case 7:
			return "AUG";
		case 8:
			return "SEP";
		case 9:
			return "OCT";
		case 10:
			return "NOV";
		default:
			return "DEC";
	}
}


















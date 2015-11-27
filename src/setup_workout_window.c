#include <pebble.h>
#include "setup_workout_window.h"
#include "set_weight_window.h"
#include "set_day_type_window.h"

/* The init function */
void setup_workout_window_init(){
	setup_workout_window = window_create();
	
	window_set_window_handlers(setup_workout_window, (WindowHandlers){
		.load = setup_workout_window_load,
		.unload = setup_workout_window_unload
	});
	
	window_stack_push(setup_workout_window, true);
}

/*
 * Window Handlers
 */

void setup_workout_window_load(Window *window){
	setup_workout_menu = menu_layer_create(layer_get_bounds(window_get_root_layer(window)));
	
	menu_layer_set_callbacks(setup_workout_menu, NULL, (MenuLayerCallbacks){
		.get_num_sections = setup_workout_menu_get_num_sections_callback,
		.get_num_rows = setup_workout_menu_get_num_rows_callback,
		.get_header_height = setup_workout_menu_get_header_height_callback,
		.draw_header = setup_workout_menu_draw_header_callback,
		.draw_row = setup_workout_menu_draw_row_callback,
		.select_click = setup_workout_menu_select_callback
	});
	
	menu_layer_set_click_config_onto_window(setup_workout_menu, window);
	
	layer_add_child(window_get_root_layer(window), menu_layer_get_layer(setup_workout_menu));
}

void setup_workout_window_unload(Window *window){
	menu_layer_destroy(setup_workout_menu);
}

/*
 * Select callbacks
 */

uint16_t setup_workout_menu_get_num_sections_callback(MenuLayer *menu_layer, void *data){
	return 1;
}

uint16_t setup_workout_menu_get_num_rows_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
	return 6;
}

int16_t setup_workout_menu_get_header_height_callback(MenuLayer *menu_layer, uint16_t section_index, void *data){
	return 0; // We won't have a header
}

void setup_workout_menu_draw_header_callback(GContext* ctx, const Layer *cell_layer, uint16_t section_index, void *data){
// 	menu_cell_basic_header_draw(ctx, cell_layer, "");
}

void setup_workout_menu_draw_row_callback(GContext* ctx, const Layer *cell_layer, MenuIndex *cell_index, void *data){
	switch(cell_index->row){
		case 0:
			menu_cell_basic_draw(ctx, cell_layer, "Change Day", "Set to A or B", NULL);
			break;
		case 1:
			menu_cell_basic_draw(ctx, cell_layer, "Squat Weight", "Set squat weight", NULL);
			break;
		case 2:
			menu_cell_basic_draw(ctx, cell_layer, "Bench Weight", "Set bench weight", NULL);
			break;
		case 3:
			menu_cell_basic_draw(ctx, cell_layer, "Bent Weight", "Set bent rows weight", NULL);
			break;
		case 4:
			menu_cell_basic_draw(ctx, cell_layer, "OH Weight", "Set OHP weight", NULL);
			break;
		case 5:
			menu_cell_basic_draw(ctx, cell_layer, "Deadlift Weight", "Set deadlift weight", NULL);
			break;
	}
}

void setup_workout_menu_select_callback(MenuLayer *menu_layer, MenuIndex *cell_index, void *data){
	switch(cell_index->row){
		case 0: // A or B day
			set_day_type_window_init();
			break;
		case 1:
			set_weight_window_init(1);
			break;
		case 2:
			set_weight_window_init(2);
			break;
		case 3:
			set_weight_window_init(3);
			break;
		case 4:
			set_weight_window_init(4);
			break;
		case 5:
			set_weight_window_init(5);
			break;
		default:
			break;
	}
}
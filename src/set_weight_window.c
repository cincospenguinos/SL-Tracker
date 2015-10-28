#include <pebble.h>
#include "set_weight_window.h"
#include "model.h"

/* Init function and window handlers */
void set_weight_window_init(int exercise){
	
	if(set_weight_window == NULL)
		set_weight_window = window_create();
	
	current_exercise = exercise;
	get_proper_weight();
	
	window_set_window_handlers(set_weight_window, (WindowHandlers){
		.load = set_weight_window_load,
		.unload = set_weight_window_unload
	});
	
	window_set_click_config_provider(set_weight_window, (ClickConfigProvider) set_weight_window_click_provider);
	
	window_stack_push(set_weight_window, true);
}

void set_weight_window_load(Window *window){
	// Setup the text layers
	exercise_name = text_layer_create(GRect(30, 10, 94, 30));
	text_layer_set_font(exercise_name, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(exercise_name));
	
	exercise_weight = text_layer_create(GRect(0, 108, 144, 30));
	text_layer_set_font(exercise_weight, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(exercise_weight));
	
	// Setup the arrows
	up_arrow_bitmap = gbitmap_create_with_resource(RESOURCE_ID_UP_ARROW_IMAGE);
	up_arrow_layer = bitmap_layer_create(GRect(114, 10, 30, 30));
	bitmap_layer_set_bitmap(up_arrow_layer, up_arrow_bitmap);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(up_arrow_layer));
	
	down_arrow_bitmap = gbitmap_create_with_resource(RESOURCE_ID_DOWN_ARROW_IMAGE);
	down_arrow_layer = bitmap_layer_create(GRect(114, 138, 30, 30));
	bitmap_layer_set_bitmap(down_arrow_layer, down_arrow_bitmap);
	layer_add_child(window_get_root_layer(window), bitmap_layer_get_layer(down_arrow_layer));
	
	// Now update the text layers
	set_weight_update_text();
}
void set_weight_window_unload(Window *window){
	text_layer_destroy(exercise_name);
	text_layer_destroy(exercise_weight);
	bitmap_layer_destroy(down_arrow_layer);
	bitmap_layer_destroy(up_arrow_layer);
	
	// Also save to the model
	save_proper_weight();
}

/*
 * Click providers
 */

void set_weight_window_click_provider(Window * window){
	window_single_click_subscribe(BUTTON_ID_UP, single_up_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, single_down_click_handler);
}
void single_up_click_handler(ClickRecognizerRef recognizer, void *context){
	// TODO: Implement this
}

void single_down_click_handler(ClickRecognizerRef recognizer, void *context){
	// TODO: Implement this
}

/*
 * Helper functions
 */

void set_weight_update_text(){
	switch(current_exercise){
		case 0:
			text_layer_set_text(exercise_name, "Squat");
			break;
		case 1:
			text_layer_set_text(exercise_name, "Bench Press");
			break;
		case 2:
			text_layer_set_text(exercise_name, "Bent Rows");
			break;
		case 3:
			text_layer_set_text(exercise_name, "Overhead Press");
			break;
		case 4:
			text_layer_set_text(exercise_name, "Deadlift");
			break;
	}
	
	static char weight_buffer[] = "999 lbs";
	snprintf(weight_buffer, sizeof(weight_buffer), "%i lbs", current_weight);
	text_layer_set_text(exercise_weight, weight_buffer);
}

/* Grabs the proper weight and throws it into our program. */
void get_proper_weight(){
	switch(current_exercise){
		case 0:
			current_weight = get_squat_weight();
			break;
		case 1:
			current_weight = get_bench_weight();
			break;
		case 2:
			current_weight = get_bent_rows_weight();
			break;
		case 3:
			current_weight = get_overhead_weight();
			break;
		case 4:
			current_weight = get_deadlifts_weight();
			break;
	}
}

/* Saves the weight set into its proper place */
void save_proper_weight(){
	switch(current_exercise){
		case 0:
			set_squat_weight(current_weight);
			break;
		case 1:
			set_bench_weight(current_weight);
			break;
		case 2:
			set_bent_rows_weight(current_weight);
			break;
		case 3:
			set_overhead_weight(current_weight);
			break;
		case 4:
			set_deadlifts_weight(current_weight);
			break;
	}
}
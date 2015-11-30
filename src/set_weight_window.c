#include <pebble.h>
#include "set_weight_window.h"
#include "model.h"


/* The initializer */
void set_weight_window_init(int exercise_type){
	exercise = exercise_type;
	get_weight_of(exercise);
	
	if(set_weight_window == NULL)
		set_weight_window = window_create();
	
	window_set_window_handlers(set_weight_window, (WindowHandlers){
		.load = set_weight_window_load,
		.unload = set_weight_window_unload
	});
	
	window_set_click_config_provider(set_weight_window, (ClickConfigProvider)set_weight_window_config_provider);
	
	window_stack_push(set_weight_window, true);
}

/* Load/Unload functions */
void set_weight_window_load(Window *window){
	exercise_text = text_layer_create(GRect(0, 0, 144, 30));
	text_layer_set_font(exercise_text, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(exercise_text, GTextAlignmentCenter);
	
	weight_text = text_layer_create(GRect(36, 53, 72, 30));
	text_layer_set_font(weight_text, fonts_get_system_font(FONT_KEY_GOTHIC_24));
	text_layer_set_text_alignment(weight_text, GTextAlignmentCenter);
	
	set_text_layer_text();
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(exercise_text));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(weight_text));
}

void set_weight_window_unload(Window *window){
	text_layer_destroy(exercise_text);
	text_layer_destroy(weight_text);
	
	set_weight_of_exercise();
}

/* The window click config provider */
void set_weight_window_config_provider(Window *window){
	window_single_click_subscribe(BUTTON_ID_UP, set_weight_window_up_single_click_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, set_weight_window_down_single_click_handler);
}

/* Up and down click handlers */
void set_weight_window_up_single_click_handler(ClickRecognizerRef recognizer, void *context){
	if(weight < 995)
		weight += 5;
	
	set_text_layer_text();
}

void set_weight_window_down_single_click_handler(ClickRecognizerRef recognizer, void *context){
	if(weight > 45)
		weight -= 5;
	
	set_text_layer_text();
}

/* Helper function to manage getting the proper weight */
void get_weight_of(int exercise_type){
	
	switch(exercise_type){
		case 1:
			weight = get_squat_weight();
			break;
		case 2:
			weight = get_bench_weight();
			break;
		case 3:
			weight = get_bent_rows_weight();
			break;
		case 4:
			weight = get_overhead_weight();
			break;
		case 5:
			weight = get_deadlifts_weight();
			break;
	}
}

/* Helper function to help set the text */
void set_text_layer_text(){
	// declare some buffers
	static char exercise_buffer[] = "Overhead Press  ";
	static char weight_buffer[] = "    lbs";
	
	// write out the text for the exercise
	switch(exercise){
		case 1:
			snprintf(exercise_buffer, sizeof(exercise_buffer), "%s", "Squat");
			break;
		case 2:
			snprintf(exercise_buffer, sizeof(exercise_buffer), "%s", "Bench Press");
			break;
		case 3:
			snprintf(exercise_buffer, sizeof(exercise_buffer), "%s", "Bent Rows");
			break;
		case 4:
			snprintf(exercise_buffer, sizeof(exercise_buffer), "%s", "Overhead Press");
			break;
		case 5:
			snprintf(exercise_buffer, sizeof(exercise_buffer), "%s", "Deadlift");
			break;
	}
	
	// Write out the text for the weight
	snprintf(weight_buffer, sizeof(weight_buffer), "%i lbs", weight);
	
	// Set the text on the screen
	text_layer_set_text(exercise_text, exercise_buffer);
	text_layer_set_text(weight_text, weight_buffer);
}

/* Helper function that goes and sets the proper weight to the model */
void set_weight_of_exercise(){
		switch(exercise){
		case 1:
			set_squat_weight(weight);
			break;
		case 2:
			set_bench_weight(weight);
			break;
		case 3:
			set_bent_rows_weight(weight);
			break;
		case 4:
			set_overhead_weight(weight);
			break;
		case 5:
			set_deadlifts_weight(weight);
			break;
	}
}
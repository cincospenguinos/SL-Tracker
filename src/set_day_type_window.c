#include <pebble.h>
#include "set_day_type_window.h"
#include "model.h"

Window *set_day_type_window;

static TextLayer *info_text; // Indicates stuff to the user
static TextLayer *day_type_text; // Indicates whether it's an A day or B day

static bool is_b_day; // false = a; true = b;

/* The init function */
void set_day_type_window_init(){
	is_b_day = get_workout_day_type();
	
	if(set_day_type_window == NULL)
		set_day_type_window = window_create();
	
	window_set_window_handlers(set_day_type_window, (WindowHandlers){
		.load = set_day_type_window_load,
		.unload = set_day_type_window_unload
	});
	
	window_set_click_config_provider(set_day_type_window, (ClickConfigProvider)set_day_type_click_config_provider);
	
	window_stack_push(set_day_type_window, true);
}

/* Load/Unload functions */
void set_day_type_window_load(Window *window){
	info_text = text_layer_create(GRect(0, 0, 144, 35));
	text_layer_set_font(info_text, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(info_text, GTextAlignmentCenter);
	text_layer_set_text(info_text, "Set Day Type");
	
	day_type_text = text_layer_create(GRect(36, 53, 72, 35));
	text_layer_set_font(day_type_text, fonts_get_system_font(FONT_KEY_GOTHIC_28));
	text_layer_set_text_alignment(day_type_text, GTextAlignmentCenter);
	set_day_type_layer_text();
	
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(info_text));
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(day_type_text));
}

void set_day_type_window_unload(Window *window){
	text_layer_destroy(info_text);
	text_layer_destroy(day_type_text);
	
	set_workout_day_type(is_b_day);
}

/* Click config provider */
void set_day_type_click_config_provider(Window *window){
	window_single_click_subscribe(BUTTON_ID_UP, click_received_handler);
	window_single_click_subscribe(BUTTON_ID_DOWN, click_received_handler);
}

/* The clicks - one for up and down clicks*/
void click_received_handler(ClickRecognizerRef recognizer, void *context){
	is_b_day = !is_b_day;
	set_day_type_layer_text();
}

void set_day_type_layer_text(){
	if(is_b_day)
		text_layer_set_text(day_type_text, "B");
	else
		text_layer_set_text(day_type_text, "A");
}
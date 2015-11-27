#pragma once
#include <pebble.h>

Window *set_day_type_window;

TextLayer *info_text; // Indicates stuff to the user
TextLayer *day_type_text; // Indicates whether it's an A day or B day

bool is_b_day; // false = a; true = b;

/* The init function */
void set_day_type_window_init();

/* Load/Unload functions */
void set_day_type_window_load(Window *window);
void set_day_type_window_unload(Window *window);

/* Click config provider */
void set_day_type_click_config_provider(Window *window);

/* The clicks - one for up and down clicks*/
void click_received_handler(ClickRecognizerRef recognizer, void *context);

/* Sets the text layer text for the day_type_text layer */
void set_day_type_layer_text();
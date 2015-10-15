#include <pebble.h>
#include "workout_window.h"
	
/* Creates and pushes this window onto the stack */
void workout_window_init(bool day){
	// Set the current day_type
	day_type = day;
	
	// Create the window and define the handlers
	workout_window = window_create();
	
	window_set_window_handlers(workout_window, (WindowHandlers){
		.load = workout_window_load,
		.unload = workout_window_unload
	});
	
	// Set up the clicks
	window_set_click_config_provider(workout_window, (ClickConfigProvider) workout_window_click_config_provider);
	
	// Push it onto the stack
	window_stack_push(workout_window, true);
}
	
/*
 * Window Handlers
 */
	
/* Sets up the workout_window when it loads up on screen */
void workout_window_load(Window *window){
	// current_workout text layer
	current_exercise = text_layer_create(GRect(0, 0, 144, 15));
	text_layer_set_text(current_exercise, "Squat");
	text_layer_set_font(current_exercise, fonts_get_system_font(FONT_KEY_GOTHIC_14_BOLD));
	text_layer_set_text_alignment(current_exercise, GTextAlignmentCenter);
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(current_exercise));
	
	// timer text layer
	timer = text_layer_create(GRect(36, 30, 72, 30));
	text_layer_set_font(timer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
	text_layer_set_text_alignment(timer, GTextAlignmentCenter);
	text_layer_set_text(timer, "");
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(timer));
	
	// sets text layers
	for(int i = 0; i < 5; i++){
		sets[i] = text_layer_create(GRect(12 + (i * 28), 80, 16, 20));
		text_layer_set_font(sets[i], fonts_get_system_font(FONT_KEY_GOTHIC_14));
		text_layer_set_text_alignment(sets[i], GTextAlignmentCenter);
		text_layer_set_text(sets[i], "0");
		layer_add_child(window_get_root_layer(window), text_layer_get_layer(sets[i]));
	}
	
	// selected set layer to show what is the current working set
	selected_set_layer = inverter_layer_create(GRect(12, 80, 16, 20));
	layer_add_child(window_get_root_layer(window), inverter_layer_get_layer(selected_set_layer));
	
	// motivation text layer
	motivation = text_layer_create(GRect(12, 104, 120, 164));
	text_layer_set_font(motivation, fonts_get_system_font(FONT_KEY_GOTHIC_14));
	text_layer_set_text_alignment(motivation, GTextAlignmentLeft);
	text_layer_set_text(motivation, "The quick brown fox jumped over the lazy dog. 63 characters"); // About sixty three characters are able to be dumped into this text layer
	layer_add_child(window_get_root_layer(window), text_layer_get_layer(motivation));
	
	// add a tick timer to this window
	tick_timer_service_subscribe(SECOND_UNIT, tick_handler);
}

/* Tears down the workout_window when the window pops off the stack */
void workout_window_unload(Window *window){
	text_layer_destroy(current_exercise);
	text_layer_destroy(timer);
	
	for(int i = 0; i < 5; i++){
		text_layer_destroy(sets[i]);
	}
	
	inverter_layer_destroy(selected_set_layer);
	
	text_layer_destroy(motivation);
}

/*
 * Click Functions
 */

/* Establishes which functions do what with what clicks */
void workout_window_click_config_provider(Window *window){
	window_single_click_subscribe(BUTTON_ID_UP, workout_window_single_up_click);
	window_single_click_subscribe(BUTTON_ID_DOWN, workout_window_single_down_click);
	window_single_click_subscribe(BUTTON_ID_SELECT, workout_window_single_select_click);
	window_single_click_subscribe(BUTTON_ID_BACK, workout_window_single_back_click);
	window_long_click_subscribe(BUTTON_ID_BACK, 1500, workout_window_long_back_click, NULL);
}

/* Run when a single up click event occurs */
void workout_window_single_up_click(ClickRecognizerRef recognizer, void *context){
	if(current_rep_count < 5)
		current_rep_count++;
	
	update_rep_text();
}

/* Run when a single up click event occurs */
void workout_window_single_down_click(ClickRecognizerRef recognizer, void *context){
	if(current_rep_count > 0)
		current_rep_count--;
	
	update_rep_text();
}

/* Run when a single up click event occurs */
void workout_window_single_select_click(ClickRecognizerRef recognizer, void *context){
	// Ensure that the timer is running and reset the timer
	timer_running = true;
	timer_count = 0;
	
	// Add one to the current working set and check if it needs to change
	if(++current_working_set == 5){
		current_working_set = 0;
		current_rep_count = 0;
		
		// TODO: Store the data for this exercise away
		
		// If we've reached the end of our workout, then we dump everything we have into persistent storage and call it a day
		if(++current_exercise_index == 3){
			// TODO: Store the workout and push it to the persistent storage
			
			// Pop this window off the stack
			window_stack_pop(true);
			return;
		} 
		
		else {
			text_layer_set_text(current_exercise, get_exercise_text());
		}
	}
	
	// Update all the various layers
	update_working_set_inverter_layer(); // change the selected set shown
	update_timer(); // Update the timer to reflect passing a set
	update_rep_text(); // Update the number of reps on the next set
	update_motivation_text(); // Update the motivation text
}

/* Run when a single up click event occurs */
void workout_window_single_back_click(ClickRecognizerRef recognizer, void *context){
	// TODO: Set this up
	
	// When a back click is made, then we are moving back to a previous rep
	if(--current_working_set == -1){
		current_working_set = 4;
		
		// If we are backing completely out of the workout, then we need to pop off this window
		if (--current_exercise_index == -1){
			window_stack_pop(true);
			return;
		} else {
			// Otherwise we need to reset the current exercise text
			text_layer_set_text(current_exercise, get_exercise_text());
		}
	}
	
	update_working_set_inverter_layer();
	update_rep_text();
}

void workout_window_long_back_click(ClickRecognizerRef recognizer, void *context){
	// If the user holds the back button for a long while, then pop this window without saving anything
	window_stack_pop(true);
}

/* Updates the current rep text */
void update_rep_text(){
	// We will use our array of buffers to manage this function
	snprintf(buffers[current_working_set], sizeof(buffers[current_working_set]), "%i", current_rep_count);
	text_layer_set_text(sets[current_working_set], buffers[current_working_set]);
}

/* Returns the proper exercise  */
const char * get_exercise_text(){
	
	// A day
	if(!day_type){
		switch(current_exercise_index){
			case 0:
				return "Squat";
			case 1:
				return "Bench Press";
			default:
				return "Bent Rows";
		}
	} 
	
	// B day
	else {
		switch(current_exercise_index){
			case 0:
				return "Squat";
			case 1:
				return "Overhead Press";
			default:
				return "Deadlift";
		}
	}
}

/* Moves the inverter layer over depending on what the current working set is */
void update_working_set_inverter_layer(){
	// First we will destroy the old layer
	inverter_layer_destroy(selected_set_layer);
	
	// Now we need to figure out where we need to place the layer again
	switch(current_working_set){
		case 0:
			selected_set_layer = inverter_layer_create(GRect(12, 80, 16, 20));
			break;
		case 1:
			selected_set_layer = inverter_layer_create(GRect(40, 80, 16, 20));
			break;
		case 2:
			selected_set_layer = inverter_layer_create(GRect(68, 80, 16, 20));
			break;
		case 3:
			selected_set_layer = inverter_layer_create(GRect(96, 80, 16, 20));
			break;
		default:
			selected_set_layer = inverter_layer_create(GRect(122, 80, 16, 20));
	}
	
	layer_add_child(window_get_root_layer(workout_window), inverter_layer_get_layer(selected_set_layer));
}

/* Updates the motivation text shown to the user */
void update_motivation_text(){
	switch(current_rep_count){
		case 5:
		text_layer_set_text(motivation, "Nice job! If it was easy, wait 90 seconds. Otherwise, wait 180.");
		break;
		default:
		text_layer_set_text(motivation, "Failure is part of the game. Wait five minutes.");
	}
}

/*
 * Tick Handlers
 */

/* Handles the time updates every second */
void tick_handler(struct tm *tick_time, TimeUnits units_changed){
	
	if(timer_running){
		timer_count++;
		update_timer();
	}
	
	// We need to send vibrations for a few different intervals
	// TODO: Update the motivation text layer
	
	if(timer_count == 90)
		vibes_short_pulse();
	else if (timer_count == 180)
		vibes_long_pulse();
	else if (timer_count == 300)
		vibes_long_pulse();
	else if (timer_count == 600)
		vibes_long_pulse();
	else if(timer_count == 999){
		timer_count = 0;
		timer_running = false;
		vibes_double_pulse();
	}
}

/* Updates the timer */
void update_timer(){
	// Create a little buffer
	static char buffer[] = "00:00";
	
	// calculate the number of seconds and minutes
	int minutes = timer_count / 60;
	int seconds = timer_count % 60;
	
	
	// Throw in the elapsed time
	if(seconds < 10)
		snprintf(buffer, sizeof(buffer), "%i:0%i", minutes, seconds);
	else
		snprintf(buffer, sizeof(buffer), "%i:%i", minutes, seconds);
	
	// Set the text up
	text_layer_set_text(timer, buffer);
}













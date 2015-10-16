/**
* main.c
*
* The entry point for the app
*/
#include <pebble.h>
#include "main_menu_window.h"
#include "model.h"

void init(){
	init_model();
	main_menu_window_init();
}

void deinit() {
	window_destroy(main_menu_window);
	
	int set1[5];
	int set2[5];
	int set3[5];
	
	Workout workout = get_workout(get_workout_count() - 1, set1, set2, set3);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "%i-%i-%i: day_type=%i\n%s: %i\n%s: %i\n%s: %i", workout.year, workout.month, workout.day, workout.day_type, 
					workout.exercise1.name, workout.exercise1.weight, workout.exercise2.name, workout.exercise2.weight, workout.exercise3.name, workout.exercise3.weight);
	
	nuke_all_old_workouts(); // TODO: Remove this
}
	
int main(void) {
	init();
	app_event_loop();
	deinit();
}
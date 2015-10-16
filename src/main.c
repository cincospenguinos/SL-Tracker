/**
* main.c
*
* The entry point for the app
*/
#include <pebble.h>
#include "workout_window.h"
#include "model.h"

// TODO: Fix the model so that array pointers are dumped into the convert_int_to_workout() function

void init(){
//	workout_window_init(false);
	// We will use this as a test method for now
	int sets1[5];
	int sets2[5];
	int sets3[5];
	
	for(int i = 0; i < 5; i++){
		sets1[i] = 0;
		sets2[i] = 0;
		sets3[i] = 0;
	}
	
	// Exercises
	Exercise exercise1;
	exercise1.name = "Squat";
	exercise1.weight = 45;
	exercise1.reps = sets1;
	
	Exercise exercise2;
	exercise2.name = "Bench Press";
	exercise2.weight = 45;
	exercise2.reps = sets2;
	
	Exercise exercise3;
	exercise3.name = "Bent Rows";
	exercise3.weight = 45;
	exercise3.reps = sets3;
	
	// Workout
	Workout workout;
	workout.day_type = true; // true means B day; false means A day
	workout.year = 115; // Remember: year is counted from 1900
	workout.month = 10;
	workout.day = 15;
	workout.exercise1 = exercise1;
	workout.exercise2 = exercise2;
	workout.exercise3 = exercise3;
	
	// Now the tests
	Workout other = convert_int_to_workout(convert_workout_to_int_typeA(workout), convert_workout_to_int_typeB(workout), convert_workout_to_int_typeC(workout));
	APP_LOG(APP_LOG_LEVEL_INFO, "Year: %i", other.year);
	APP_LOG(APP_LOG_LEVEL_INFO, "Month: %i", other.month);
	APP_LOG(APP_LOG_LEVEL_INFO, "Day: %i", other.day);
	
	for(int i = 0; i < 5; i++)
		APP_LOG(APP_LOG_LEVEL_INFO, "%s; %i: %i", other.exercise1.name, i, other.exercise1.reps[i]);
	
	for(int i = 0; i < 5; i++)
		APP_LOG(APP_LOG_LEVEL_INFO, "%s; %i: %i", other.exercise2.name, i, other.exercise2.reps[i]);
	
	for(int i = 0; i < 5; i++)
		APP_LOG(APP_LOG_LEVEL_INFO, "%s; %i: %i", other.exercise3.name, i, other.exercise3.reps[i]);
}

void deinit(){
// 	window_destroy(workout_window);
}
	
int main(void){
	init();
	app_event_loop();
	deinit();
}
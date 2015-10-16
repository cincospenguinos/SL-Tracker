/**
* main.c
*
* The entry point for the app
*/
#include <pebble.h>
#include "workout_window.h"
#include "model.h"

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
	workout.day_type = false; // true means B day; false means A day
	workout.year = 115; // Remember: year is counted from 1900
	workout.month = 10;
	workout.day = 15;
	workout.exercise1 = exercise1;
	workout.exercise2 = exercise2;
	workout.exercise3 = exercise3;
	
	// Now the tests
	int workout_intA = convert_workout_to_int_typeA(workout);
	int workout_intB = convert_workout_to_int_typeB(workout);
	int workout_intC = convert_workout_to_int_typeC(workout);
	APP_LOG(APP_LOG_LEVEL_INFO, "workout_intA: %X", workout_intA);
	APP_LOG(APP_LOG_LEVEL_INFO, "workout_intB: %X", workout_intB);
	APP_LOG(APP_LOG_LEVEL_INFO, "workout_intC: %X", workout_intC);
}

void deinit(){
// 	window_destroy(workout_window);
}
	
int main(void){
	init();
	app_event_loop();
	deinit();
}
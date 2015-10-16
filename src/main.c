/**
* main.c
*
* The entry point for the app
*/
#include <pebble.h>
#include "workout_window.h"
#include "model.h"

// TODO: Fix the model so that array pointers are dumped into the convert_int_to_workout() function

void print_all_info(Workout workout){
	APP_LOG(APP_LOG_LEVEL_INFO, "Year: %i", workout.year);
	APP_LOG(APP_LOG_LEVEL_INFO, "Month: %i", workout.month);
	APP_LOG(APP_LOG_LEVEL_INFO, "Day: %i", workout.day);
	
	for(int i = 0; i < 5; i++)
		APP_LOG(APP_LOG_LEVEL_INFO, "%s => %i lbs; %i:%i", workout.exercise1.name, workout.exercise1.weight, i, workout.exercise1.reps[i]);
	
	for(int i = 0; i < 5; i++)
		APP_LOG(APP_LOG_LEVEL_INFO, "%s => %i lbs; %i:%i", workout.exercise2.name, workout.exercise2.weight, i, workout.exercise2.reps[i]);
	
	for(int i = 0; i < 5; i++)
		APP_LOG(APP_LOG_LEVEL_INFO, "%s => %i lbs; %i:%i", workout.exercise3.name, workout.exercise3.weight, i, workout.exercise3.reps[i]);
}

void init(){
//	workout_window_init(false);
	// Ensure we have the model established
	init_model();
	
	// We will use this as a test method for now
	int sets1[5];
	int sets2[5];
	int sets3[5];
	
	for(int i = 0; i < 5; i++){
		sets1[i] = 3;
		sets2[i] = 5;
		sets3[i] = 1;
	}
	
	int other_sets1[5];
	int other_sets2[5];
	int other_sets3[5];
	
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
	workout.year = 2015;
	workout.month = 10;
	workout.day = 15;
	workout.exercise1 = exercise1;
	workout.exercise2 = exercise2;
	workout.exercise3 = exercise3;
	
	// Now the tests
	store_new_workout(workout); // Store a workout
	store_new_workout(workout); // Store another workout
	store_new_workout(workout); // Store yet another workout
	
	// Now delete one
	delete_workout(1);
	
// 	for(int i = 0; i < get_workout_count(); i++){
// 		Workout other = get_workout(i, other_sets1, other_sets2, other_sets3);
// 		print_all_info(other);
// 	}
	
	// Now peek at one
	WorkoutPeek peek = peek_workout(1);
	APP_LOG(APP_LOG_LEVEL_INFO, "%i:%i:%i ----> %i", peek.year, peek.month, peek.day, peek.day_type);
	
	// Destroy everything
	nuke_all_old_workouts();
}

void deinit(){
// 	window_destroy(workout_window);
}
	
int main(void){
	init();
	app_event_loop();
	deinit();
}
/**
 * model.c
 *
 * Represents the data model for the app. Manages persistent IO.
 */
#include "model.h"

/* Various constants */
#define CURRENT_STORAGE_VERSION 1 // What version of storage we are currently using

/*
 * Persistent Keys
 */

#define STORAGE_VERSION_KEY 0 // The key that pulls what version of storage the user has last had it set to
#define WORKOUT_TYPE_KEY 1 // Boolean indicating whether it's an A day (false) or a B day (true)
#define SQUAT_WEIGHT_KEY 2 // Integer containing the current weight of the next squat
#define BENCH_WEIGHT_KEY 3 // Integer containing the current weight of the next bench press
#define BENT_WEIGHT_KEY 4 // Integer containing the current weight of the next bent rows
#define OVERHEAD_WEIGHT_KEY 5 // Integer containing the current weight of the next overhead press
#define DEADLIFT_WEIGHT_KEY 6 // Integer containing the current weight of the next deadlift
#define WORKOUT_LOG_COUNT_KEY 7 // The total number of old workouts is located
#define WORKOUT_LOG_START_KEY 8 // Where the workout log starts

/*
 * Model Management Functions - acts as a controller
 */
	
/* Sets up the persistent data. */
void init_model() {
	// If we don't have any existing persistent data, then we need to set it up
	if(!persist_exists(WORKOUT_LOG_COUNT_KEY)) {
		// We have no logged workouts
		persist_write_int(WORKOUT_LOG_COUNT_KEY, 0);
		
		// The default next day is an A day
		persist_write_bool(WORKOUT_TYPE_KEY, false);
		
		// All weight defaults to 45 lbs
		persist_write_int(SQUAT_WEIGHT_KEY, 45);
		persist_write_int(BENCH_WEIGHT_KEY, 45);
		persist_write_int(BENT_WEIGHT_KEY, 45);
		persist_write_int(OVERHEAD_WEIGHT_KEY, 45);
		persist_write_int(DEADLIFT_WEIGHT_KEY, 45);
		
		// Write the current storage version into persistent storage as well
		persist_write_int(STORAGE_VERSION_KEY, CURRENT_STORAGE_VERSION);
	}
}

/* Toggles the boolean A/B day type */
void toggle_next_workout(){
	bool type_of_day = persist_read_bool(WORKOUT_TYPE_KEY);
	type_of_day = !type_of_day;
	persist_write_bool(WORKOUT_TYPE_KEY, type_of_day);
}

/* Stores into persistent memory the workout structure passed */
void store_workout(Workout workout){
	// First convert the workout into three separate integers
	int workout_intA = convert_workout_to_int_typeA(workout);
	int workout_intB = convert_workout_to_int_typeB(workout);
	int workout_intC = convert_workout_to_int_typeC(workout);
	
	// Write them to their proper place
	int write_index = WORKOUT_LOG_START_KEY + (persist_read_int(WORKOUT_LOG_COUNT_KEY) * 3);
	persist_write_int(write_index, workout_intA);
	persist_write_int(write_index + 1, workout_intB);
	persist_write_int(write_index + 2, workout_intC);
	
	// Add one to the count
	persist_write_int(WORKOUT_LOG_COUNT_KEY, persist_read_int(WORKOUT_LOG_COUNT_KEY) + 1);
}

/* Pulls the workout out from the index passed */
Workout get_workout(int index){
	// TODO: Implement this
	Workout result;
	return result; // a stub
}

/* Deletes the workout at the index passed */
void delete_workout(int index){
	// TODO: Implement this
}

/* Returns an integer containing the day type and date the workout was created at the index passed */
int peek_workout(int index){
	// TODO: Implement this
	
	return 0; // a stub
}

/*
 * Conversion functions - converts ints to structs and back
 */

/* Converts a workout into an integer */
int convert_workout_to_int_typeA(Workout workout){
	// Figure out what the day is
	int result = 0;
	
	if(workout.day_type)
		result = 1;
	
	APP_LOG(APP_LOG_LEVEL_INFO, "%X", result);
	
	// Throw in the year, month and day
	result = (result << 8) | workout.year;
	APP_LOG(APP_LOG_LEVEL_INFO, "%X", result);
	result = (result << 4) + workout.month;
	APP_LOG(APP_LOG_LEVEL_INFO, "%X", result);
	result = (result << 5) + workout.day;
	APP_LOG(APP_LOG_LEVEL_INFO, "%X", result);
	
	// Now the weight of the first exercise
	result = (result << 6) + (workout.exercise1.weight / 5); // We store the weight in lbs divided by 5
	APP_LOG(APP_LOG_LEVEL_INFO, "%X", result);
	APP_LOG(APP_LOG_LEVEL_INFO, "Exercise 1 weight: %X", (workout.exercise1.weight / 5));
	
	// And now two of the sets
	result = (result << 3) + workout.exercise1.reps[0];
	APP_LOG(APP_LOG_LEVEL_INFO, "%X", result);
	result = (result << 3) + workout.exercise1.reps[1];
	APP_LOG(APP_LOG_LEVEL_INFO, "%X", result);
	
	result = result << 2;
	
	return result;
}
int convert_workout_to_int_typeB(Workout workout){
	// TODO: Implement this
	
	return 0; // a stub
}
int convert_workout_to_int_typeC(Workout workout){
	// TODO: Implement this
	
	return 0; // a stub
}

/* Converts an integer to a workout structure */
Workout convert_int_to_workout(int workout){
	// TODO: Implement this
	Workout result;
	
	
	return result; // a stub
}

/* Converts exercise structure to an integer */
int convert_exercise_to_int(Exercise exercise){
	// TODO: Implement this
	return 0; // a stub
}

/* Converts an integer to an exercise structure */
Exercise convert_int_to_exercise(int exercise){
	// TODO: Implement this
	Exercise result;
	
	
	return result; // a stub
}


/*
 * Version Management Functions
 */

/* Updates from the first version of workouts to the current version */
void update_version1_workouts(){
	// NOTE: Implement this if needed
}

/* Migrate all of the old workouts to the new workouts */
void migrate_old_workouts(){
	// NOTE: Implement this if needed
}
















/**
 * model.c
 *
 * Represents the data model for the app. Manages persistent IO.
 */
#include "model.h"

/*
 * Persistent Keys
 */

#define WORKOUT_TYPE_KEY 0 // Boolean indicating whether it's an A day (false) or a B day (true)
#define SQUAT_WEIGHT_KEY 1 // Integer containing the current weight of the next squat
#define BENCH_WEIGHT_KEY 2 // Integer containing the current weight of the next bench press
#define BENT_WEIGHT_KEY 3 // Integer containing the current weight of the next bent rows
#define OVERHEAD_WEIGHT_KEY 4 // Integer containing the current weight of the next overhead press
#define DEADLIFT_WEIGHT_KEY 5 // Integer containing the current weight of the next deadlift
#define WORKOUT_LOG_COUNT_KEY 1 // The total number of old workouts is located
#define WORKOUT_LOG_START_KEY 2 // Where the workout log starts


/* Toggles the boolean A/B day type */
void toggle_next_workout(){
	bool type_of_day = persist_read_bool(WORKOUT_TYPE_KEY);
	type_of_day = !type_of_day;
	persist_write_bool(WORKOUT_TYPE_KEY, type_of_day);
}
	
/* Sets up the persistent data. */
void setup_persistent_data() {
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
	}
}
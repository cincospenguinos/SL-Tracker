/**
 * model.c
 *
 * Represents the data model for the app. Manages persistent IO.
 */
#include <pebble.h>

/* Persistent keys */
#define OLD_WORKOUT_COUNT_KEY 1 // The total number of old workouts is located
#define NEXT_WORKOUT_KEY 0 // Boolean indicating whether it's an A day (false) or a B day (true)
#define WORKOUT_LOG_START_KEY 2 // Where the workout log starts

/* Workout struct definition */
typedef struct {
	bool day_type; // Whether it's an A day or B day
	
	int day;
	int month;
	int year;
	
	int *workout_sets1; // The first workout set
	int *workout_sets2; // The second workout set
	int *workout_sets3; // The third workout set
}Workout;

/* Creates a workout with the given parameters and returns a pointer to that workout */
Workout workout_create(bool day_type, int day, int month, int year, int *workout_sets1, int *workout_sets2, int *workout_sets3) {
	// Create the workout
	Workout workout;
	
	// Populate the information
	workout.day_type = day_type;
	
	workout.day = day;
	workout.month = month;
	workout.year = year;
	
	workout.workout_sets1 = workout_sets1;
	workout.workout_sets2 = workout_sets1;
	workout.workout_sets3 = workout_sets1;
	
	return workout;
}

/* Converts the passed workout into an int */
int workout_convert_to_int(Workout workout){
	int result = 0;
	
	
	
	return result; // a stub
}

/* Stores the workout passed into the data model */
void store_workout(Workout *workout){
	
}
	
/* Sets up the persistent data. */
void setup_persistent_data() {
	// If we don't have any existing persistent data, then we need to set it up
	if(!persist_exists(OLD_WORKOUT_COUNT_KEY)) {
		persist_write_int(OLD_WORKOUT_COUNT_KEY, 0); // No old workouts
		persist_write_bool(NEXT_WORKOUT_KEY, false); // The next day is an A day
	}
}
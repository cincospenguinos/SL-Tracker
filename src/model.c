/**
 * model.c
 *
 * Represents the data model for the app. Manages persistent IO.
 */
#include <pebble.h>

/* Persistent keys */
#define NEXT_WORKOUT_KEY 0 // Boolean indicating whether it's an A day (false) or a B day (true)
#define WORKOUT_LOG_COUNT_KEY 1 // The total number of old workouts is located
#define WORKOUT_LOG_START_KEY 2 // Where the workout log starts

/* Workout struct definition */
typedef struct {
	bool day_type; // Whether it's an A day or B day
	
	int day;
	int month;
	int year;
	
	int sets_sum1; // The first workout set
	int sets_sum2; // The second workout set
	int sets_sum3; // The third workout set
}Workout;

/* Creates a workout with the given parameters and returns a pointer to that workout */
Workout workout_create(bool day_type, int day, int month, int year, int sets1, int sets2, int sets3) {
	// Create the workout
	Workout workout;
	
	// Populate the information
	workout.day_type = day_type;
	
	workout.day = day;
	workout.month = month;
	workout.year = year;
	
	workout.sets_sum1 = sets1;
	workout.sets_sum2 = sets2;
	workout.sets_sum3 = sets3;
	
	return workout;
}

/* Converts the passed workout into an int */
int workout_convert_to_int(Workout workout){
	int result = 0;
	
	// If it's a B day, then we start with one
	if(workout.day_type)
		result = 1;
	
	// add in the date
	result = (result << 7) + workout.year;
	result = (result << 3) + workout.month;
	result = (result << 5) + workout.day;
	
	// add in the various set sums
	result = (result << 5) + workout.sets_sum1;
	result = (result << 5) + workout.sets_sum2;
	result = (result << 5) + workout.sets_sum3;	
	
	return result; // a stub
}

/* Stores the workout passed into the data model */
void workout_store_workout(Workout workout){
	// Convert the workout to an integer
	int workout_int = workout_convert_to_int(workout);
	
	// Get the old workout count
	int old_workout_count = persist_read_int(WORKOUT_LOG_COUNT_KEY);
	
	// Write the workout int to the next location in persistent memory
	persist_write_int(old_workout_count + persist_read_int(WORKOUT_LOG_START_KEY), workout_int);
	
	// Add one to the count and store it in persistent memory
	old_workout_count++;
	persist_write_int(WORKOUT_LOG_COUNT_KEY, old_workout_count);
}

/* Toggles the boolean A/B day type */
void toggle_next_workout(){
	bool day_type = persist_read_bool(NEXT_WORKOUT_KEY);
	day_type = !day_type;
	persist_write_bool(NEXT_WORKOUT_KEY, day_type);
}
	
/* Sets up the persistent data. */
void setup_persistent_data() {
	// If we don't have any existing persistent data, then we need to set it up
	if(!persist_exists(WORKOUT_LOG_COUNT_KEY)) {
		persist_write_int(WORKOUT_LOG_COUNT_KEY, 0); // No old workouts
		persist_write_bool(NEXT_WORKOUT_KEY, false); // The next day is an A day
	}
}
/**
 * model.c
 *
 * Represents the data model for the app. Manages persistent IO.
 *
 * NOTE: When keeping track of the year, we actually store the FULL year! NOT some small chunk of it!
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
#define SQUAT_DELOAD_KEY 7 // Integer indicating whether or not we failed the last set
#define BENCH_DELOAD_KEY 8 // Integer indicating whether or not we failed the last set
#define BENT_DELOAD_KEY 9 // Integer indicating whether or not we failed the last set
#define OVERHEAD_DELOAD_KEY 10 // Integer indicating whether or not we failed the last set
#define DEADLIFTS_DELOAD_KEY 11 // Integer indicating whether or not we failed the last set
#define WORKOUT_LOG_COUNT_KEY 12 // The total number of old workouts is located here
#define WORKOUT_LOG_START_KEY 13 // Where the workout log starts

/*
 * Model Management Functions - acts as a controller
 */
	
/* Sets up the persistent data. */
void init_model() {
	// If we don't have any existing persistent data, then we need to set it up
// 	if(!persist_exists(WORKOUT_LOG_COUNT_KEY)) {		
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
	
		// Setup all the deload stuff
		persist_write_int(SQUAT_DELOAD_KEY, 0);
		persist_write_int(BENCH_DELOAD_KEY, 0);	
		persist_write_int(BENT_DELOAD_KEY, 0);
		persist_write_int(OVERHEAD_DELOAD_KEY, 0);
		persist_write_int(DEADLIFTS_DELOAD_KEY, 0);
		
		// Write the current storage version into persistent storage as well
		persist_write_int(STORAGE_VERSION_KEY, CURRENT_STORAGE_VERSION);
// 	}
}

/* Toggles the boolean A/B day type */
void toggle_next_workout(){
	bool type_of_day = persist_read_bool(WORKOUT_TYPE_KEY);
	
	type_of_day = !type_of_day;
	persist_write_bool(WORKOUT_TYPE_KEY, type_of_day);
}

/* Stores into persistent memory the workout structure passed */
void store_new_workout(Workout workout) {
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
	
	// Toggle the next workout
	toggle_next_workout();
	
	// Manage the weight changes
	// A day
	if(!workout.day_type){
		manage_weights_a(workout);
	} 
	
	// B day
	else {
		manage_weights_b(workout);
	}
}

/* Helper function that manages the weights for an A day */
void manage_weights_a(Workout workout){
	// Get the sums of all the reps
	int squats = 0;
	int bench = 0;
	int bent = 0;
	
	for(int i = 0; i < 5; i++){
		squats += workout.exercise1.reps[i];
		bench += workout.exercise2.reps[i];
		bent += workout.exercise3.reps[i];
	}
	
	// Check to see if we add weight
	if(squats == 25){
		persist_write_int(SQUAT_WEIGHT_KEY, workout.exercise1.weight + 5);
		persist_write_int(SQUAT_DELOAD_KEY, 0);
	} 
	else
		persist_write_int(SQUAT_DELOAD_KEY, persist_read_int(SQUAT_DELOAD_KEY) + 1);
	
	if(bench == 25){
		persist_write_int(BENCH_WEIGHT_KEY, workout.exercise2.weight + 5);
		persist_write_int(BENCH_DELOAD_KEY, 0);
	}
	else
		persist_write_int(BENCH_DELOAD_KEY, persist_read_int(BENCH_DELOAD_KEY) + 1);
	
	if(bent == 25){
		persist_write_int(BENT_WEIGHT_KEY, workout.exercise3.weight + 5);
		persist_write_int(BENT_DELOAD_KEY, 0);
	}
	else
		persist_write_int(BENT_DELOAD_KEY, persist_read_int(BENT_DELOAD_KEY) + 1);
	
	// Check squat deload
	if(persist_read_int(SQUAT_DELOAD_KEY) == 3){
		squats = workout.exercise1.weight * 9 / 10;
		
		while(squats % 5 != 0)
			squats--;
		
		if(squats < 45)
			squats = 45;
		
		persist_write_int(SQUAT_WEIGHT_KEY, squats);
		persist_write_int(SQUAT_DELOAD_KEY, 0);
	}
	
	// Check bench deload
	if(persist_read_int(BENCH_DELOAD_KEY) == 3){
		bench = workout.exercise1.weight * 9 / 10;
		
		while(bench % 5 != 0)
			bench--;
		
		if(bench < 45)
			bench = 45;
		
		persist_write_int(BENCH_WEIGHT_KEY, bench);
		persist_write_int(BENCH_DELOAD_KEY, 0);
	}
	
	// Check bent deload
	if(persist_read_int(BENT_DELOAD_KEY) == 3){
		bent = workout.exercise1.weight * 9 / 10;
		
		while(bent % 5 != 0)
			bent--;
		
		if(bent < 45)
			bent = 45;
		
		persist_write_int(BENT_WEIGHT_KEY, bent);
		persist_write_int(BENT_DELOAD_KEY, 0);
	}
}

/* Helper function that manages the weights for a B day */
void manage_weights_b(Workout workout){
	// Get the sums of all the reps
	int squats = 0;
	int overhead = 0;
	
	for(int i = 0; i < 5; i++){
		squats += workout.exercise1.reps[i];
		overhead += workout.exercise2.reps[i];
	}
	
	// Check to see if we add weight
	if(squats == 25){
		persist_write_int(SQUAT_WEIGHT_KEY, workout.exercise1.weight + 5);
		persist_write_int(SQUAT_DELOAD_KEY, 0);
	}
	else
		persist_write_int(SQUAT_DELOAD_KEY, persist_read_int(SQUAT_DELOAD_KEY) + 1);
	
	if(overhead == 25) {
		persist_write_int(OVERHEAD_WEIGHT_KEY, workout.exercise2.weight + 5);
		persist_write_int(OVERHEAD_DELOAD_KEY, 0);
	}
	else
		persist_write_int(OVERHEAD_DELOAD_KEY, persist_read_int(OVERHEAD_DELOAD_KEY) + 1);
	
	if(workout.exercise3.reps[2] == 5){
		persist_write_int(DEADLIFT_WEIGHT_KEY, workout.exercise3.weight + 10);
		persist_write_int(DEADLIFTS_DELOAD_KEY, 0);
	}
	else
		persist_write_int(DEADLIFTS_DELOAD_KEY, persist_read_int(DEADLIFTS_DELOAD_KEY) + 1);
	
	// Check squat deload
	if(persist_read_int(SQUAT_DELOAD_KEY) == 3){
		squats = workout.exercise1.weight * 9 / 10;
		
		while(squats % 5 != 0)
			squats--;
		
		if(squats < 45)
			squats = 45;
		
		persist_write_int(SQUAT_WEIGHT_KEY, squats);
		persist_write_int(SQUAT_DELOAD_KEY, 0);
	}
	
	// Check overhead press deload
	if(persist_read_int(OVERHEAD_DELOAD_KEY) == 3){
		overhead = workout.exercise1.weight * 9 / 10;
		
		while(overhead % 5 != 0)
			overhead--;
		
		if(overhead < 45)
			overhead = 45;
		
		persist_write_int(OVERHEAD_WEIGHT_KEY, overhead);
		persist_write_int(OVERHEAD_DELOAD_KEY, 0);
	}
	
	// Check deadlift deload
	if(persist_read_int(DEADLIFTS_DELOAD_KEY) == 3){
		int deadlift = workout.exercise1.weight * 9 / 10;
		
		while(deadlift % 10 != 5)
			deadlift--;
		
		if(deadlift < 45)
			deadlift = 45;
		
		persist_write_int(DEADLIFT_WEIGHT_KEY, deadlift);
		persist_write_int(DEADLIFTS_DELOAD_KEY, 0);
	}
}

/* Pulls the workout out from the index passed */
Workout get_workout(int index, int *sets1, int *sets2, int *sets3) {
	Workout result;
	int model_index = WORKOUT_LOG_START_KEY + index * 3;
	
	if(persist_exists(model_index)){
		int storedA = persist_read_int(model_index);
		int storedB = persist_read_int(model_index + 1);
		int storedC = persist_read_int(model_index + 2);
		
		result = convert_int_to_workout(storedA, storedB, storedC, sets1, sets2, sets3);
	} else {
		APP_LOG(APP_LOG_LEVEL_ERROR, "Entry could not be found at %i!", index);
	}
	
	return result;
}

/* Deletes the workout at the index passed */
void delete_workout(int index) {
	int current_index = WORKOUT_LOG_START_KEY + index * 3; // where in the model we are actually deleting
	int next_index = WORKOUT_LOG_START_KEY + (index + 1) * 3; // The next one down
	
	if(persist_exists(current_index)){
		while(persist_exists(current_index)){
		
			// If another workout exists in the next space, copy it over
			if(persist_exists(next_index)){
				persist_write_int(current_index, persist_read_int(next_index));
				persist_write_int(current_index + 1, persist_read_int(next_index + 1));
				persist_write_int(current_index + 2, persist_read_int(next_index + 2));
			} 

			// Otherwise, just delete the current one
			else {
				persist_delete(current_index);
				persist_delete(current_index + 1);
				persist_delete(current_index + 1);
			}

			current_index += 3;
			next_index += 3;
		}
		
		persist_write_int(WORKOUT_LOG_COUNT_KEY, persist_read_int(WORKOUT_LOG_COUNT_KEY) - 1);
	}
}

/* Returns a WorkoutPeek structure */
WorkoutPeek peek_workout(int index){
	WorkoutPeek peek;
	int read_index = WORKOUT_LOG_START_KEY + (index * 3);
	
	// If it exists, let's peek it
	if(persist_exists(read_index)){
		int peek_int = persist_read_int(read_index);
		
		// Pull out the peek information and return it
		peek.day_type = (peek_int < 0);
		peek.year = ((peek_int & 0x7FFFFFFF) >> 23) + 2000;
		peek.month = ((peek_int & 0x007FFFFF) >> 19);
		peek.day = ((peek_int & 0x0007FFFF) >> 14);
	}
	
	// This code should never run
	else {
		APP_LOG(APP_LOG_LEVEL_ERROR, "Attempted to peek at index %i!", index);
	}
	
	return peek;
}

/* Returns the number of workouts in the model */
int get_workout_count(){
	return persist_read_int(WORKOUT_LOG_COUNT_KEY);
}

/* Returns boolean indicating an A day or B day */
bool get_workout_day_type(){
	return persist_read_bool(WORKOUT_TYPE_KEY);
}

void set_workout_day_type(bool day_type){
	persist_write_bool(WORKOUT_TYPE_KEY, day_type);
}

/* Get weight for various exercises; always in pounds */
int get_squat_weight(){
	return persist_read_int(SQUAT_WEIGHT_KEY);
}
int get_bench_weight(){
	return persist_read_int(BENCH_WEIGHT_KEY);
}
int get_bent_rows_weight(){
	return persist_read_int(BENT_WEIGHT_KEY);
}
int get_overhead_weight(){
	return persist_read_int(OVERHEAD_WEIGHT_KEY);
}
int get_deadlifts_weight(){
	return persist_read_int(DEADLIFT_WEIGHT_KEY);
}

void set_squat_weight(int weight){
	persist_write_int(SQUAT_WEIGHT_KEY, weight);
}
void set_bench_weight(int weight){
	persist_write_int(BENCH_WEIGHT_KEY, weight);
}
void set_bent_rows_weight(int weight){
	persist_write_int(BENT_WEIGHT_KEY, weight);
}
void set_overhead_weight(int weight){
	persist_write_int(OVERHEAD_WEIGHT_KEY, weight);
}
void set_deadlifts_weight(int weight){
	persist_write_int(DEADLIFT_WEIGHT_KEY, weight);
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
	
	// Throw in the year, month and day
	result = (result << 8) + (workout.year - 2000); // We use 1900 as the year we gauge everything by
	result = (result << 4) + workout.month;
	result = (result << 5) + workout.day;
	
	// Now the weight of the first exercise
	result = (result << 8) + (workout.exercise1.weight / 5); // We store the weight in lbs divided by 5
	
	// And now two of the sets
	result = (result << 3) + workout.exercise1.reps[0];
	result = (result << 3) + workout.exercise1.reps[1];
	
	return result;
}
int convert_workout_to_int_typeB(Workout workout){
	int result = 0;
	
	// put in the last few sets of the first exercise
	result = workout.exercise1.reps[2];
	result = (result << 3) + workout.exercise1.reps[3];
	result = (result << 3) + workout.exercise1.reps[4];
	
	// put in the weight for the second exercise
	result = (result << 8) + (workout.exercise2.weight / 5);
	
	// put in all of the sets for the second exercise
	for(int i = 0; i < 5; i++)
		result = (result << 3) + workout.exercise2.reps[i];
	
	return result;
}
int convert_workout_to_int_typeC(Workout workout){
	// dump in the weight over five first
	int result = workout.exercise3.weight / 5;
	
	// dump in each of the sets
	for(int i = 0; i < 5; i++)
		result = (result << 3) + workout.exercise3.reps[i];
	
	// move over the necessary bits
	result = (result << 9);
	
	return result;
}

/* Converts an integer to a workout structure */
Workout convert_int_to_workout(int storedA, int storedB, int storedC, int *sets1, int *sets2, int *sets3){
	// Create the workout, sets, and Exercises
	Workout result;
	
	Exercise ex1;
	Exercise ex2;
	Exercise ex3;
	
	for(int i = 0; i < 5; i++){
		ex1.reps[i] = sets1[i];
		ex2.reps[i] = sets2[i];
		ex3.reps[i] = sets3[i];
	}
	
	// Get all the stuff from storedA
	result.day_type = (storedA < 0); // If storedA is negative, then it's a B day
	result.year = ((storedA & 0x7FFFFFFF) >> 23) ;
	result.month = ((storedA & 0x007FFFFF) >> 19);
	result.day = ((storedA & 0x0007FFFF) >> 14);
	
	ex1.weight = ((storedA & 0x00003FFF) >> 6) * 5; // Don't forget to multiply by five!
	ex1.reps[0] = ((storedA & 0x0000003F) >> 3);
	ex1.reps[1] = (storedA & 0x00000007);
	
	// Get all the stuff from storedB
	ex1.reps[2] = (storedB >> 29) & 0x00000007;
	ex1.reps[3] = ((storedB & 0x1FFFFFFF) >> 26);
	ex1.reps[4] = ((storedB & 0x03FFFFFF) >> 23);
	
	ex2.weight = ((storedB & 0x007FFFFF) >> 15) * 5; // Don't forget to multiply by five!
	
	ex2.reps[0] = ((storedB & 0x00007FFF) >> 12);
	ex2.reps[1] = ((storedB & 0x00000FFF) >> 9);
	ex2.reps[2] = ((storedB & 0x000001FF) >> 6);
	ex2.reps[3] = ((storedB & 0x0000003F) >> 3);
	ex2.reps[4] = storedB & 0x00000007;
	
	// Get all the stuff from storedC
	ex3.weight = (storedC >> 24) * 5; // Don't forget to multiply by five!
	
	ex3.reps[0] = ((storedC & 0x00FFFFFF) >> 21);
	ex3.reps[1] = ((storedC & 0x001FFFFF) >> 18);
	ex3.reps[2] = ((storedC & 0x0003FFFF) >> 15);
	ex3.reps[3] = ((storedC & 0x00007FFF) >> 12);
	ex3.reps[4] = ((storedC & 0x00000FFF) >> 9);
	
	// Setup the last few things in each of the exercises
	
	ex1.name = "Squat"; // Exercise 1 is always squats
	
	if(!result.day_type){
		// A day
		ex2.name = "Bench Press";
		ex3.name = "Bent Rows";
	} else {
		// B day
		ex2.name = "Overhead Press";
		ex3.name = "Deadlift";
	}
	
	// Put the exercises into the workout and return it
	result.exercise1 = ex1;
	result.exercise2 = ex2;
	result.exercise3 = ex3;
	
	return result;
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

/* Deletes all the old workouts, NOT any of the weights */
void nuke_all_old_workouts(){	
	// I don't know why on earth we need to do this, but for some reason we do
	bool workout_day_type = get_workout_day_type();
	int squat_weight = get_squat_weight();
	int bench_weight = get_bench_weight();
	int bent_weight = get_bent_rows_weight();
	int overhead_weight = get_overhead_weight();
	int deadlift_weight = get_deadlifts_weight();
	
	// Delete all of the old workouts
	for(int i = 0; i < persist_read_int(WORKOUT_LOG_COUNT_KEY); i++){
		persist_delete(persist_read_int(WORKOUT_LOG_COUNT_KEY) + i);
		persist_delete(persist_read_int(WORKOUT_LOG_COUNT_KEY) + i + 1);
		persist_delete(persist_read_int(WORKOUT_LOG_COUNT_KEY) + i + 2);
	}
	
	// Ensure that the workout log count is equal to zero
	persist_write_int(WORKOUT_LOG_COUNT_KEY, 0);
	
	// Make sure we preserve the day type
	set_workout_day_type(workout_day_type);
	set_squat_weight(squat_weight);
	set_bench_weight(bench_weight);
	set_bent_rows_weight(bent_weight);
	set_overhead_weight(overhead_weight);
	set_deadlifts_weight(deadlift_weight);
}

/* Converts an integer to a three letter code representing the month */
const char * int_to_month(int month){
	switch(month){
		case 0:
			return "JAN";
		case 1:
			return "FEB";
		case 2:
			return "MAR";
		case 3:
			return "APR";
		case 4:
			return "MAY";
		case 5:
			return "JUN";
		case 6:
			return "JUL";
		case 7:
			return "AUG";
		case 8:
			return "SEP";
		case 9:
			return "OCT";
		case 10:
			return "NOV";
		default:
			return "DEC";
	}
}


/*
 * Debugging functions
 */

void print_out_workout(int index){
	int sets1[5];
	int sets2[5];
	int sets3[5];
	
	Workout workout = get_workout(index, sets1, sets2, sets3);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Printing workout at index %i\n", index);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "%s @ %i lbs => %i %i %i %i %i", workout.exercise1.name, workout.exercise1.weight, workout.exercise1.reps[0],
				 workout.exercise1.reps[1], workout.exercise1.reps[2], workout.exercise1.reps[3], workout.exercise1.reps[4]);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "%s @ %i lbs => %i %i %i %i %i", workout.exercise2.name, workout.exercise2.weight, workout.exercise2.reps[0],
				 workout.exercise2.reps[1], workout.exercise2.reps[2], workout.exercise2.reps[3], workout.exercise2.reps[4]);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "%s @ %i lbs => %i %i %i %i %i", workout.exercise3.name, workout.exercise3.weight, workout.exercise3.reps[0],
				 workout.exercise3.reps[1], workout.exercise3.reps[2], workout.exercise3.reps[3], workout.exercise3.reps[4]);
}

void print_out_workout_struct(Workout workout){
	APP_LOG(APP_LOG_LEVEL_DEBUG, "Printing workout");
	APP_LOG(APP_LOG_LEVEL_DEBUG, "%s @ %i lbs => %i %i %i %i %i", workout.exercise1.name, workout.exercise1.weight, workout.exercise1.reps[0],
				 workout.exercise1.reps[1], workout.exercise1.reps[2], workout.exercise1.reps[3], workout.exercise1.reps[4]);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "%s @ %i lbs => %i %i %i %i %i", workout.exercise2.name, workout.exercise2.weight, workout.exercise2.reps[0],
				 workout.exercise2.reps[1], workout.exercise2.reps[2], workout.exercise2.reps[3], workout.exercise2.reps[4]);
	APP_LOG(APP_LOG_LEVEL_DEBUG, "%s @ %i lbs => %i %i %i %i %i", workout.exercise3.name, workout.exercise3.weight, workout.exercise3.reps[0],
				 workout.exercise3.reps[1], workout.exercise3.reps[2], workout.exercise3.reps[3], workout.exercise3.reps[4]);
}









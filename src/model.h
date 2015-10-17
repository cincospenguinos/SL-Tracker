/**
 * model.h
 *
 * Represents the data model for the entire application. This collection of methods
 * though acts less like the model and more like a controller. It manages the model
 * which is all in persistent storage. It pulls out and puts in the various parts
 * requested and stuff.
 *
 * Think of the model as a dynamic array of workouts which this collection of methods
 * permits you to access as if it were an array. You can get at index, set at index,
 * delete from an index, etcetera.
 */
#pragma once
#include <pebble.h>

/*
 * Struct definitions
 */

/* Exercise struct definition */
typedef struct {
	char *name; // Name of the exercise
	int *reps; // Pointer to an array of the sets containing the reps of each
	int weight; // The weight of the exercise - always in pounds
}Exercise;

/* Workout struct definition */
typedef struct {
	bool day_type; // False means A; true means B
	
	int day; // Day of the month the workout was created
	int month; // Month the workout was created
	int year; // The year the workout was created
	
	Exercise exercise1; // The first exercise - ALWAYS SQUATS
	Exercise exercise2; // The second exercise - Bench or Overhead
	Exercise exercise3; // The third exercise - Bent Rows or Deadlift
}Workout;

/* WorkoutPeek struct definition */
typedef struct {
	bool day_type; // false for A, true for B
	
	// Date the workout was created
	int year;
	int month;
	int day;
}WorkoutPeek;

/*
 * Model Management Functions - acts as a controller
 */

/* Sets up the persistent data. */
void init_model();

/* Toggles whether the next workout is an A day or B day */
void toggle_next_workout();

/* Stores into persistent memory the workout structure passed */
void store_new_workout(Workout workout);

/* Stores into persistent memory a workout using the data passed */
void store_new_workout_raw(bool day_type, int *sets1, int *sets2, int *sets3, int weight1, int weight2, int weight3);

/* Pulls the workout out from the index passed */
Workout get_workout(int index, int *sets1, int *sets2, int *sets3);

/* Deletes the workout at the index passed */
void delete_workout(int index);

/* Returns a small structure containing the day type and date the workout was created at the index passed */
WorkoutPeek peek_workout(int index);

/* Returns the number of workouts in the model */
int get_workout_count();

/* Returns boolean indicating an A day or B day */
bool get_workout_day_type();

/* Get weight for various exercises; always in pounds */
int get_squat_weight();
int get_bench_weight();
int get_bent_rows_weight();
int get_overhead_weight();
int get_deadlifts_weight();


/*
 * Conversion functions - converts ints to structs and back
 */

/* Converts a workout into an integer */
int convert_workout_to_int_typeA(Workout workout);
int convert_workout_to_int_typeB(Workout workout);
int convert_workout_to_int_typeC(Workout workout);

/* Converts an integer to a workout structure */
Workout convert_int_to_workout(int storedA, int storedB, int storedC, int *sets1, int *sets2, int *sets3);

/*
 * Version Management Functions - in case I change my mind on how things should be stored
 */

/* Updates from the first version of workouts to the current version */
void update_version1_workouts();

/* Migrate all of the old workouts to the new workouts */
void migrate_old_workouts();

/* Deletes all persistant data with this application */
void nuke_all_old_workouts();

/* Converts an integer to a three letter code representing the month */
const char * int_to_month(int month);
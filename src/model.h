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
	bool day_type; // Whether it's an A day or B day
	
	int day; // Day of the month the workout was created
	int month; // Month the workout was created
	int year; // The year the workout was created
	
	Exercise exercise1; // The first exercise - ALWAYS SQUATS
	Exercise exercise2; // The second exercise - Bench or Overhead
	Exercise exercise3; // The third exercise - Bent Rows or Deadlift
}Workout;

/*
 * Various functions to manage the model
 */

/* Toggles whether the next workout is an A day or B day */
void toggle_next_workout();

/* Sets up the persistent data. */
void setup_persistent_data();
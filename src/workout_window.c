#include <pebble.h>
#include "model.h"
#include "workout_window.h"

/* Variables for the view */
static TextLayer *current_exercise; // the workout that is currently being done
static TextLayer *weight_text; // the amount of weight for the current exercise
static TextLayer *timer; // the timer managing the various sets and reps
static TextLayer *sets[5]; // the collection of sets that are currently being managed
static InverterLayer *selected_set_layer; // the collection of layers that indicate what is the currently selected set
static TextLayer *motivation; // a text layer to provide motivation to the user

/* Variables for the model */
Workout current_workout;

/* Variables to manage the view */
bool timer_running; // dictates whether or not the timer is running

int timer_count; // the current number of seconds on the timer
int current_rep_count; // the current number of reps the user has listed
int current_working_set; // the current set that the user is working on
int current_exercise_index; // manages the current exercise

char set_buffers[5][2]; // An array of buffers to manage the sets text layers
char weight_buffer[8]; // Buffer to hold the weight of the various exercises

bool finished_workout; // Indicates whether we need to save the workout or not

bool on_deadlifts; // True if the user is currently working on deadlifts
bool failed_set; // True if the user failed a set
/* This show resulation of time in nano second *
/* in future we should get time from ntp server */

/* maybe */
# include <time.h>
# include <unistd>

int resulation = -9

int Qtime_get_current_date_time();

int Qtime_set_current_date_time();

int Qtime_get_current_time();

int Qtime_set_current_time();

int Qtime_set_current_date();

int Qtime_get_current_date();

char* Qtime_get_current_date_time();

/* convert timestamp to string of time */
char* Qtime_to_char();

/* convert string to timestamp of time */
char* Qtime_to_int();

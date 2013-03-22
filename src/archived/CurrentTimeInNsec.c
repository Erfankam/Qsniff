#include <stdio.h>
#include <time.h>
#include <stdlib.h>
//For Compile: gcc CurrentTimeInNsec.c -lrt

int main(void)
    {
    time_t now;
    struct timespec tp;
    struct tm ts;
    char buffer[200];
    // Get current time

    // Format time, "Date(yyyy-mm-dd) 
    //hh:mm:ss:nnnnnnnn zzzz"
    //Current time is based on local area !
    
    //struct tm {ts.tm_sec , ts.tm_min , ts.tm_hour , ...}
    //struct timespec tp {tp.tv_sec , tp.tv_nsec}
    long int i=0;
    time(&now);
    for (i=0; i<1; i++) {
        time(&now);
        ts = *localtime(&now);
        clock_gettime(CLOCK_REALTIME, &tp);
        //char* tm = system("date +%s%N");
    
        //strftime(buffer, sizeof(buffer), "Date: (%a %Y-%m-%d)\n%H Hour(s):%M Minute(s):%S Second(s)", &ts);
        //printf("%s", buffer);
        printf("%ld.%ld NanoSecond(s) Local Area:IRST \n",tp.tv_sec, tp.tv_nsec);
    }
    long double last ;
    printf("size of last is: %d\n", sizeof(last));
    printf("nano sec of the time is %ld\n", tp.tv_nsec);
    last = tp.tv_sec + (long double)tp.tv_nsec/1000000000;
    double lastTiny = (double)last;
    printf("last tine in float is: %.9llf\n", last);
    printf("last tine in float tiny is: %.9llf\n", last);
    printf("size of tiny last is: %d\n", sizeof(lastTiny));
    //int sec = atoi(tp.tv_sec);
    //int nsec = atoi(tp.tv_sec);
    return 0;
        }

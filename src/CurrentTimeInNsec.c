#include <stdio.h>
#include <time.h>
//For Compile: gcc CurrentTimeInNsec.c -lrt

int main(void)
    {
    time_t now;
    struct timespec tp;
    struct tm ts;
    char buffer[200];
    // Get current time
    time(&now);

    // Format time, "Date(yyyy-mm-dd) 
    //hh:mm:ss:nnnnnnnn zzzz"
    //Current time is based on local area !
    
    //struct tm {ts.tm_sec , ts.tm_min , ts.tm_hour , ...}
    ts = *localtime(&now);
    //struct timespec tp {tp.tv_sec , tp.tv_nsec}
    clock_gettime(CLOCK_REALTIME, &tp);
    
    strftime(buffer, sizeof(buffer), "Date: (%a %Y-%m-%d)\n%H Hour(s):%M Minute(s):%S Second(s)", &ts);
    printf("%s", buffer);
    printf(":%ld NanoSecond(s) Local Area:IRST \n",tp.tv_nsec);
    return 0;
    }

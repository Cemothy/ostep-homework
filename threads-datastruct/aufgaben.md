# Aufgaben #

1. We’ll start by redoing the measurements within this chapter. Use
the call gettimeofday() to measure time within your program.
How accurate is this timer? What is the smallest interval it can measure? Gain confidence in its workings, as we will need it in all subsequent questions. You can also look into other timers, such as the
cycle counter available on x86 via the rdtsc instruction.

    int gettimeofday(struct timeval *tv, struct timezone *tz); 
        Parameters:
            tv: A pointer to a struct timeval, which will be filled with the current time.
            tz: Historically used for time zone information. Nowadays, it's often set to NULL, as it's obsolete.
    
    struct timeval {
        time_t      tv_sec;  // seconds
        suseconds_t tv_usec; // microseconds
    };
        Time saved in second and microseconds



2. Now, build a simple concurrent counter and measure how long it
takes to increment the counter many times as the number of threads
increases. How many CPUs are available on the system you are
using? Does this number impact your measurements at all?

    Number of threads: 1, Time taken: 196248 microseconds, Counter: 10000000
    Number of threads: 2, Time taken: 1481944 microseconds, Counter: 20000000
    Number of threads: 4, Time taken: 2954277 microseconds, Counter: 40000000
    Number of threads: 8, Time taken: 6283868 microseconds, Counter: 80000000
    Number of threads: 16, Time taken: 11234554 microseconds, Counter: 160000000

    Von 1 auf 2 Threads: 655.14% länger. --> Overhead too big ?? 
    Von 2 auf 4 Threads: 99.35% länger.
    Von 4 auf 8 Threads: 112.70% länger.
    Von 8 auf 16 Threads: 78.78% länger.



3. Next, build a version of the approximate counter. Once again, measure its performance as the number of threads varies, as well as the
threshold. Do the numbers match what you see in the chapter?


    Threshold: 10
    Number of threads: 1, Time taken: 316158 microseconds, Counter: 10000000
    Number of threads: 2, Time taken: 1188502 microseconds, Counter: 20000000
    Number of threads: 4, Time taken: 2345458 microseconds, Counter: 40000000
    Number of threads: 8, Time taken: 2899781 microseconds, Counter: 80000000
    Number of threads: 16, Time taken: 5748981 microseconds, Counter: 160000000

    Von 1 auf 2 Threads: etwa 275.92% länger.
    Von 2 auf 4 Threads: etwa 97.35% länger.
    Von 4 auf 8 Threads: etwa 23.63% länger.
    Von 8 auf 16 Threads: etwa 98.26% länger.

    Threshold: 1000
    Number of threads: 1, Time taken: 255267 microseconds, Counter: 10000000
    Number of threads: 2, Time taken: 788847 microseconds, Counter: 20000000
    Number of threads: 4, Time taken: 2108212 microseconds, Counter: 40000000
    Number of threads: 8, Time taken: 3136651 microseconds, Counter: 80000000
    Number of threads: 16, Time taken: 4920191 microseconds, Counter: 160000000

    Von 1 auf 2 Threads: etwa 209.03% länger.
    Von 2 auf 4 Threads: etwa 167.25% länger.
    Von 4 auf 8 Threads: etwa 48.78% länger.
    Von 8 auf 16 Threads: etwa 56.86% länger.
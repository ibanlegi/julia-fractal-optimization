#pragma once
#include <ctime>

struct Timer {
    timespec startTime;
    timespec endTime;

    void start() { 
        clock_gettime(CLOCK_MONOTONIC, &startTime); 
    }

    void stop() { 
        clock_gettime(CLOCK_MONOTONIC, &endTime); 
    }

    double elapsed() const {
        return (endTime.tv_sec - startTime.tv_sec) + (endTime.tv_nsec - startTime.tv_nsec) / 1e9;
    }
};

#ifndef _TIMER_H_
#define _TIMER_H_

#ifdef _WIN32
#include <sys/timeb.h>
#include <windows.h>
#include <time.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif


/**
 *  A very basic timer class, suitable for FPS counters etc.
 */
class Timer {

public:
	Timer() : startTime_(getCurrentTime()) {};

	/**
	 * Report the elapsed time in seconds (it will return a double,
	 * so the fractional part is subsecond part).
	 */
	inline double elapsed() const {
		return getCurrentTime() - startTime_;
	};

	/** 
	 * Report the elapsed time in seconds, and reset the timer.
	 */
	inline double elapsedAndRestart() {
		double now = getCurrentTime();
		double elapsed = now - startTime_;
		startTime_ = now;
		return elapsed;
	};

	/**
	 * Restart the timer.
	 */
	inline void restart() {
		startTime_ = getCurrentTime();
	};

	/** 
	 * Return the current time as number of elapsed seconds of this day.
	 */
	double static getCurrentTime() {
#ifdef WIN32
#if 1
    LARGE_INTEGER f;
    LARGE_INTEGER t;
    QueryPerformanceFrequency(&f);
    QueryPerformanceCounter(&t);
    return t.QuadPart/(double) f.QuadPart;
#else
    struct _timeb ts;
		_ftime(&ts);
		return (ts.time + 0.001 * ts.millitm);
#endif
#else
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return tv.tv_sec+tv.tv_usec*1e-6;
#endif
  };


private:
	double startTime_;
};
#endif // _TIMER_H_

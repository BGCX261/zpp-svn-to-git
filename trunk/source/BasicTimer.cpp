/*
This file is part of the ZScript Preprocessor library,
(BSD License) and can be redistributed as such.
All rights reserved.
*/

#include <time.h>
#include <math.h>
#include "BasicTimer.h"



BasicTimer::BasicTimer()
	: 
	minutes(0),
	seconds(0.f),
	start_time(0),
	finish_time(0)
	{
	}


BasicTimer::BasicTimer( const BasicTimer& t )
	: 
	minutes(t.minutes), 
	seconds(t.seconds), 
	start_time(t.start_time), 
	finish_time(t.finish_time) 
	{
	}


BasicTimer::~BasicTimer()
	{
	}


BasicTimer &BasicTimer::operator = ( const BasicTimer& t )
	{
		minutes = t.minutes;
		seconds = t.seconds;
		start_time = t.start_time;
		finish_time = t.finish_time;

		return *this;
	}


void BasicTimer::StartTimer()
	{
		start_time = clock();
	}


void BasicTimer::StopTimer()
	{
		finish_time = clock();
		ComputeTime();
	}


void BasicTimer::ComputeTime()
	{
		double elapsed_seconds = (finish_time - start_time) / (double)CLOCKS_PER_SEC;
		minutes = int( elapsed_seconds / 60.0 );
		seconds = float( fmod(elapsed_seconds, 60.0) );
	}


void BasicTimer::ResetTimer()
	{
		*this = BasicTimer();
	}


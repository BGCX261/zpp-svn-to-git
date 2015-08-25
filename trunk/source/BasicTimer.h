/*
This file is part of the ZScript Preprocessor library,
(BSD License) and can be redistributed as such.
All rights reserved.
*/

#pragma once



class BasicTimer
{
public:
	BasicTimer();
	BasicTimer( const BasicTimer& t );
	~BasicTimer();

	BasicTimer &operator = ( const BasicTimer& t );

	int GetMinutes() const
		{
			return minutes;
		}

	float GetSeconds() const
		{
			return seconds;
		}

	long GetStartTime()
		{
			return start_time;
		}

	long GetFinishTime()
		{
			return finish_time;
		}

	void StartTimer();
	void StopTimer();
	void ComputeTime();
	void ResetTimer();

protected:
	int		minutes;
	float	seconds;
	long	start_time;
	long	finish_time;
};

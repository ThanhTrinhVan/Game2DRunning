#pragma once
#ifndef IMP_TIMER_H
#define IMP_TIMER_H

#include "CommonFunc.h"

class ImpTimer
{
public:
	ImpTimer();
	~ImpTimer();

	void start();
	void stop();
	void paused();
	void continue_();

	int get_ticks();
	bool is_start() { return is_started; };
	bool is_pause() { return is_paused; };
private:
	int start_tick;
	int paused_tick;
	bool is_paused;
	bool is_started;
};
#endif // !IMP_TIMER_H



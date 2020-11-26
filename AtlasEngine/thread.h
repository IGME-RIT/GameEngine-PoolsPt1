#pragma once
#include "pch.h"

enum ThreadTypes
{
	TH_PLAYER,
	TH_COIN,
	TH_NUM_TYPES
};

struct Thread
{
	// used for Pool
	Thread* prev;
	Thread* next;

	Thread* parentThread;
	Thread* siblingThread;
	Thread* childThread;

	void (*funcOnDestroy)();
	void (*funcOnCollide)();
	void (*funcOnTick)(Thread* th);

	// pointer to any data used by the thread
	void* object;

	// Thread and Instance are linked together
	Instance* inst;
};

Thread* Thread_Init(int type, void (*onTick)(Thread* th));
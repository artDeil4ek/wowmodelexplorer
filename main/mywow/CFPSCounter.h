#pragma once

#include "core.h"

class CFPSCounter
{
public:
	CFPSCounter()
	{
		FPS = 60;
		StartTime = 0;
		FrameCounted = 0;
	}

	//
	f32 getFPS() const { return FPS; }
	void registerFrame(u32 now);
	

private:
	f32 FPS;
	u32 StartTime;
	u32 FrameCounted;
};

inline void CFPSCounter::registerFrame(u32 now)
{
	++FrameCounted;

	const u32 milliseconds = now - StartTime;

	if ( milliseconds >= 1500 )
	{
		const f32 invMilli = reciprocal_((f32)milliseconds);
		FPS = (1000*FrameCounted) * invMilli ;
		FrameCounted = 0;
		StartTime = now;
	}
}

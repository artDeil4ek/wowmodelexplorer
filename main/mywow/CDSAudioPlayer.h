#pragma once

#include "IAudioPlayer.h"
#include <dsound.h>
#include <list>

class CDSSound;

class CDSAudioPlayer : public IAudioPlayer
{
private:
	DISALLOW_COPY_AND_ASSIGN(CDSAudioPlayer);

public:
	CDSAudioPlayer();
	~CDSAudioPlayer();

public:
	bool initDevice(HWND hwnd, u32 primaryChannels, u32 primarySampleRate, u32 primayBitsPerSample);

	virtual ISound* getEmptyWavSound() const;
	virtual ISound* getEmptyOggSound() const;
	virtual ISound* getEmptyMp3Sound() const;

	virtual ISound* getWavSound(u32 index) const;
	virtual ISound* getOggSound(u32 index) const;
	virtual ISound* getMp3Sound(u32 index) const;

	virtual void setAudioSetting(const SAudioSetting& setting);
	virtual const SAudioSetting& getAudioSetting() const { return Setting; }

	virtual void fadeoutSound(ISound* sound, u32 fadetime);
	virtual void tickFadeOutSounds(u32 timeSinceLastFrame);

private:
	bool setPrimaryBufferFormat(u32 primaryChannels, u32 primarySampleRate, u32 primayBitsPerSample);
	static DWORD WINAPI NotificationProc( LPVOID lpParameter );

private:
	LPDIRECTSOUND		pIDSound;

	CDSSound*	WavSounds[NUM_WAV_SOUNDS];
	CDSSound*	OggSounds[NUM_OGG_SOUNDS];
	CDSSound*	Mp3Sounds[NUM_MP3_SOUNDS];

	HANDLE	NotifyThread;
	DWORD	ThreadID;

	SAudioSetting	Setting;

	struct SEntry
	{
		ISound*	sound;
		u32 fadetime;
	};

	typedef std::list<SEntry, qzone_allocator<SEntry>>	T_FadeoutList;
	T_FadeoutList		FadeoutList;
};
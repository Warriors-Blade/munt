#ifndef PORT_AUDIO_DRIVER_H
#define PORT_AUDIO_DRIVER_H

#include <QtCore>

#include <portaudio.h>

#include <mt32emu/mt32emu.h>

#include "AudioDriver.h"
#include "../ClockSync.h"

class QSynth;
class Master;
class PortAudioDriver;

class PortAudioStream : public AudioStream {
private:
	QSynth *synth;
	unsigned int sampleRate;
	PaStream *stream;

	ClockSync clockSync;
	// The total latency of audio stream buffers
	// Special value of 0 indicates PortAudio to use its own recommended latency value
	qint64 audioLatency;
	// The number of nanos by which to delay (MIDI) events to help ensure accurate relative timing.
	qint64 latency;
	qint64 sampleCount;

	static int paCallback(const void *inputBuffer, void *outputBuffer, unsigned long frameCount, const PaStreamCallbackTimeInfo *timeInfo, PaStreamCallbackFlags statusFlags, void *userData);

public:
	PortAudioStream(QSynth *useSynth, unsigned int useSampleRate);
	~PortAudioStream();
	bool start(PaDeviceIndex deviceIndex);
	void close();
};

class PortAudioDevice : public AudioDevice {
friend class PortAudioDriver;
private:
	PaDeviceIndex deviceIndex;
	PortAudioDevice(PortAudioDriver *driver, int useDeviceIndex, QString useDeviceName);

public:
	PortAudioStream *startAudioStream(QSynth *synth, unsigned int sampleRate);
};

class PortAudioDriver : public AudioDriver {
public:
	PortAudioDriver(Master *useMaster);
	~PortAudioDriver();
	QList<AudioDevice *> getDeviceList();
};

#endif
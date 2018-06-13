#pragma once

/**
@file		FPSCounter.h
@author		Kimmo Riihiaho
@date		6/13/2018
@version	0.1

@brief Simple fps counter on header file only.

@section DESCRIPTION

Call tic with your GLFW time
every time you render a frame. Use getters to
get FPS and time per frame.
*/
class FPSCounter
{
public:
	FPSCounter() {}
	~FPSCounter() {}

	/*
	Call tick once per rendered frame.
	*/
	void tic(double time) {
		currentTime = time;
		update();
	}

	/* FPS as double. */
	double getFpsD() {
		return fps;
	}

	/* FPS as int. */
	int getFpsI() {
		return fps;
	}

	/* Time per frame in seconds. */
	double getTimePerFrame() {
		return timePerFrame;
	}

	/* Time per frame in milliseconds. */
	double getTimePerFrameMS() {
		return timePerFrame * 1000;
	}

private:

	double currentTime = 0.0;
	double timeOfLastFPSUpdate = 0.0;
	// Time of last render.
	double timeOfLastRender = 0.0;
	// Time per frame in seconds.
	double timePerFrame = 0.0;
	unsigned nbFrames = 0;
	double fps = 0.0;

	/*
	Update counters
	*/
	void update() {

		double delta = currentTime - timeOfLastFPSUpdate;
		nbFrames++;
		// If last cout was more than 1 sec ago
		if(delta >= 1.0) {
			// Update fps once per second and reset frame count.
			fps = (double)nbFrames / delta;
			nbFrames = 0;
			timeOfLastFPSUpdate = currentTime;
		}

		// time it took to render last frame
		timePerFrame = currentTime - timeOfLastRender;

		timeOfLastRender = currentTime;
	}
};


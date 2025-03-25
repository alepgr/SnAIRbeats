#include <gpiod.h>
#include <iostream>
#include <thread>
#include <chrono>
#include <cstdlib>


class GPIOPin{
private:

	gpiod_chip* chip;
	gpiod_line* SensorLine;
	gpiod_line* LEDLine;

	int InterruptPin = 17;
	int LEDPin = 27;

	bool running = true;

public:
	GPIOPin(const char* chipName, int InterruptPin, int LEDPin): chip(nullptr), SensorLine(nullptr), LEDLine(nullptr), InterruptPin(InterruptPin), LEDPin(LEDPin){
		chip = gpiod_chip_open_by_name(chipName);
		if (!chip){
			std::cerr << "[ERROR] Could not open chip - womp womp" << std::endl;
			
		}

		SensorLine = gpiod_chip_get_line(chip,InterruptPin);
		if (!SensorLine){
			std::cerr << "[ERROR] Could not get sensor line" << std::endl;

		}

		if (!gpiod_line_request_both_edges_events(SensorLine, "sensor") < 0){
			std::cerr << "[ERROR] Could not request events for sensor line - womp womp" << std::endl;
		}



	}

	void Worker(){
        while(running) {
            // Block indefinitely until an event occurs.
            //std::cout << "Start of Loop" << std::endl;
            int ret = gpiod_line_event_wait(SensorLine, nullptr);
            if(ret < 0) {
                std::cerr << "Error waiting for event" << std::endl;
                continue;
            }
            if(ret > 0) {
                struct gpiod_line_event event;
                if(gpiod_line_event_read(SensorLine, &event) < 0) {
                    std::cerr << "Error reading event" << std::endl;
                    continue;
                }
                // Process only falling edge events (i.e. when the pin goes LOW)
                if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
                    //Check WOM clears interrupt latch, by reading INT_STATUS
                    //if(check_wom_interrupt()) {
                        //replace with Callbacks
                        //std::cout << "Output" << std::endl;
                    std::cout << "Hello" << std::endl;

                    }
                }
            }
        }

};
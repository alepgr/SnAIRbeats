#include "include/gpioevent.h"
#include <gpiod.h>
#include <iostream>
#include <thread>
#include <chrono>

namespace GPIOName {

    GPIOClass::GPIOClass(const char* chipName, int InterruptPin, int LEDPin)
        : chip(nullptr), SensorLine(nullptr), LEDLine(nullptr),
          InterruptPin(InterruptPin), LEDPin(LEDPin),
          running(true), Counter(0)
    {
        chip = gpiod_chip_open_by_name(chipName);
        if (!chip) {
            std::cerr << "[ERROR] Could not open chip - womp womp" << std::endl;
        }
        SensorLine = gpiod_chip_get_line(chip, InterruptPin);
        if (!SensorLine) {
            std::cerr << "[ERROR] Could not get sensor line" << std::endl;
        }
        if (gpiod_line_request_both_edges_events(SensorLine, "sensor") < 0) {
            std::cerr << "[ERROR] Could not request events for sensor line - womp womp" << std::endl;
        }
    }

    void GPIOClass::Worker() {
        while (running) {
            int ret = gpiod_line_event_wait(SensorLine, nullptr);
            if (ret < 0) {
                std::cerr << "Error waiting for event" << std::endl;
                continue;
            }
            if (ret > 0) {
                struct gpiod_line_event event;
                if (gpiod_line_event_read(SensorLine, &event) < 0) {
                    std::cerr << "Error reading event" << std::endl;
                    continue;
                }
                if (event.event_type == GPIOD_LINE_EVENT_FALLING_EDGE) {
                    Counter++;
                    std::cout << Counter << " Hits have been detected" << std::endl;
                    //std::this_thread::sleep_for(std::chrono::milliseconds(4));
                }
            }
        }
    }

}

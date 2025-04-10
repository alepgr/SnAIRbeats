#include "include/CameraMaths.hpp"
#include "cam2opencv/libcam2opencv.h"
#include "cam2opencv/qtviewer/window.h"

#include <iostream>



namespace CameraMathsName{
    CameraMaths::CameraMaths(){

    }
    
    // struct Screenshot : public Libcam2OpenCV::Callback{
    //     Window* window = nullptr;
    //     virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &) {
    //         if (nullptr != window) {
    //             window->updateImage(frame);
    //         }
    //     }
    // };

    
struct Screenshot : Libcam2OpenCV::Callback {
    Window* window = nullptr;
    virtual void hasFrame(const cv::Mat &frame, const libcamera::ControlList &) {
        if (nullptr != window) {
            window->updateImage(frame);
        }

        if (frame.empty()) {
            std::cerr << "Warning: Received an empty frame!" << std::endl;
            return;
        }
        //debugging stuff
        std::cout << "First row pixel values: ";
        int numPrint = std::min(frame.cols, 10); // Print at most 10 pixels for brevity.
        for (int j = 0; j < numPrint; j++) {
            cv::Vec3b pixel = frame.at<cv::Vec3b>(0, j);
            std::cout << "[" << (int)pixel[0] << ", " 
                      << (int)pixel[1] << ", " 
                      << (int)pixel[2] << "] ";
        }

        cv::imshow("Camera Frame", frame);
        cv::waitKey(1);

    }
};
}
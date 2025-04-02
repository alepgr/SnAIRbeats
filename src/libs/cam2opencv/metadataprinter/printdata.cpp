#include "../libcam2opencv.h"
#include <libcamera/libcamera.h>

#include <unistd.h>

struct MyCallback : Libcam2OpenCV::Callback {
    virtual void hasFrame(const cv::Mat &, const libcamera::ControlList &metadata) {
	for (const auto &ctrl : metadata) {
	    const libcamera::ControlId *id = libcamera::controls::controls.at(ctrl.first);
	    const libcamera::ControlValue &value = ctrl.second;
	    
	    std::cout << "\t" << id->name() << " = " << value.toString()
		      << std::endl;
	}
	std::cout << std::endl;
    }
};

// Main program
int main(int argc, char *argv[]) {
    std::cout << "Press any key to stop" << std::endl;
    
    // create an instance of the camera class
    Libcam2OpenCV camera;

    // create an instance of the callback
    MyCallback myCallback;

    // register the callback
    camera.registerCallback(&myCallback);

    // create an instance of the settings
    Libcam2OpenCVSettings settings;

    // set the framerate (default is variable framerate)
    settings.framerate = 30;

    // start the camera with these settings
    camera.start(settings);

    // do nothing till the user presses any key
    getchar();

    // stop the camera
    camera.stop();

    // that's it!
    printf("\n");
}

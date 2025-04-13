# SnAIRbeats
SnAirBeats is a next generation methods to practice the drum kit, while reducing noise and space required to do so. The SnAirBeat set uses intertial measurement units (IMU) within the sticks to track their movement and play a corresponding drum, not requiring any physical hitting like modern electric drum sets need.

# Building
SnAIRBeats requires the following components to work:

* 1x  [Raspberry Pi 5](https://uk.rs-online.com/web/p/raspberry-pi/0219255)
* 2x  [SEN15335 Breakout IMU](https://uk.rs-online.com/web/p/sensor-development-tools/2836590)
* 1x  [External USB Speaker](https://thepihut.com/products/mini-external-usb-stereo-speaker?variant=31955934801&country=GB&currency=GBP&utm_medium=product_sync&utm_source=google&utm_content=sag_organic&utm_campaign=sag_organic&gad_source=1&gbraid=0AAAAADfQ4GEFlapDAiWBmf2emlHnIUtFT&gclid=CjwKCAjwwe2_BhBEEiwAM1I7sZX7Qgw-18XC8Lfv5pq3lkJwZNLx-_Mg28yoPVhgCK76qUmuxv67WhoC8xIQAvD_BwE)


The circuit's wires should be at least 1m long to ensure comfortable movement while playing to avoid risk of damaging the project.  
A wiring guide can be seen below:

![Wiring Guide](FIGURES/WiringGuide.png)

The drumsticks for the project need to be 3D printed via the [STLs](/STLs/) provided within this repository.

# Prerequisites
Firstly it should be noted that SnAIRBeats can only run on a Linux system. It is recommened to use a Raspberry Pi operating system such as [Raspebian](https://www.raspberrypi.com/software/) as the packages will not work on Windows systems.  

Before installing any of the prerequisites, please update your package list with:
```
sudo apt update
```

There are 4 main libraries that need to be installed for this project:
* Libgpiod - for general purpose input/output
* mraa - IoT and hardware interface library (required for IMU driver)
* YAML - Support for YAML (required for IMU driver)
* ALSA - To process and play sound files

These packages can be installed by running the following commands through the terminal of the Raspberry Pi.
```
sudo apt install -y libgpiod-dev
sudo apt install -y libmraa-dev
sudo apt install -ylibyaml-dev
sudo apt install -y libasound2-dev
```


# Compliation from source

The project is built using a series of CMakeLists.txt which locate and link the required internal and external libraries for the project. By running the code below, the CMake will generate the respective make files within each of files. Running make will build the project and return an executable.


```
cmake .
make
```

It may take a few seconds for everything to build properly, but once everything has been successfully created you can use the code below to run SnAIRBeats.  

```
./SnairBeats
```

# Usage
SnAIRBeats works by reading the direction of acceleration within the IMUs. Holding the sticks with the X-direction representing the vertical axis:
* Hitting a stick down will play a snare drum
* Hitting a stick to either side will play a high tom
* Lunging the stick forward will play a crash cymbal

If desired, the sounds played by each direction can be changed by swapping files in the ALSAPlayer library found either [here](src/libs/ALSAPlayer/include) or through the command directory:
```
cd src/libs/ALSAPlayer/include
ls
```


# Media
* [Instagram](https://www.instagram.com/snairbeats/)
* [TikTok](https://www.tiktok.com/@snairbeats?_t=ZN-8uF2Rv9Fbuw&_r=1)

# Authors and contributions
* Calum Robertson
* Alejandra Paja Garcia
* Aleksander Zahariev
* Mohammed Alqabandi
* Renata Cia Sanches Loberto
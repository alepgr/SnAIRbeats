#Top level of CMake. Establishing 
cmake_minimum_required(VERSION 3.10)

project(SnairBeat LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Place built executables at top-level
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

message(STATUS 
"                                                                                                         
                                                                       @@@                               
                                                                         @                               
                                                                         @@ @@@                          
                                                                          @ @@                           
                                                                          @@@                            
                                                                           @@                            
         @@@@@   @@@@  @@@   @@@@@@   @ @  @ @  @@@   @@@@@@    @@@@@@@    -@     @@@@@@@@@  *@@@@       
        @@@@@@@@ @@@@@ @@@   @    @   @    @  @@   @  @@@@@@@@  @@@@@@@    @@@    @@@@@@@@@ @@@@@@@@     
        @@   @@@ @@@@@  @@   @ @@ @   @ @  @  @ @  @  @@   @@@  @@         @ @       @@@    @@@  @@@     
        @@@      @@@@@@ @@   @ @@  @  @ @  @ @@ @  @  @@   @@-  @@         @ @@      @@@    @@@@         
         @@@@    @@@ @@@@@  @  @@  @  @ @  @      @   @@@@@@    @@@@@@    @   @      @@@     @@@@@       
           @@@@  @@@ @@@@@  @ @  - @  @ @  @  @@  @@  @@   @@@  @@@       @@@@@@     @@@       @@@@      
        @@   @@@ @@@  @@@@  @ @@@   @ @ @  @ *@ @  @  @@   @@@  @@       @@    @@    @@@    @@   @@@     
        @@   @@@ @@@  @@@@ @  -  -  @ @ @  @  @ @  @  @@   @@@  @@       @@    @@    @@@    @@@  @@@     
        @@@@@@@  @@@   @@@ @  @  @  @ @ @  @  @ @  @ @@@@@@@@   @@@@@@@  @      @@   @@@     @@@@@@      
                           @@@    @@@ @@@  @@@   @@@                    @@       @                       
                                                                        @@                               
                                                                       @@                                
                                                                                                         
")

message(STATUS "Welcome - Gathering and building required libraries")


#Links the source directory and the tests directory the the project
add_subdirectory(src)
add_subdirectory(tests)

enable_testing()
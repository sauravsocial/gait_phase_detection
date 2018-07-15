# gait_phase_detection
Detection of two phases (swing and stance) phase from the IMU datasets.


# Mount two IMU sensors , one above the  knee , the other below the knee.
The subject should walk with a variable speed on the treadmill for atleast 2 minutes, so that every data of the each gait cycle is recorded.


# Save the file as .csv
First file is the thigh data and the second file is the shank data.


Use the Thresholds for each phases and then apply knn to classify the phases. 
I got correctly 2 correct phases from this approach ie, Swing phase and Stance phase.


Compile and run the main.cpp file.
The labelled data is saved in the data.dat file after closing the console window.
I converted the .dat file to .txt file and uploaded it for your convinience.


# the sampling frequency of the IMU sensor is 200Hz

#Source code for the Hope and Bike project

# HnB-Controller
Hope &amp; Bike projet Fablab CAEN FRANCE

This project contains the source code of the 2 processors used in the Hope & Bike power management board.

The board controls a brushless motor of 250W. It comunicates with the Brain of the system (an Arduino board) by the means of the BioNet network. 

Bionet stands for Bike Open Network

This simple network as been developted for the Hope & Bike projet
Base on a producer / consumer model and on master salve topology it manages information dedicated to the bike :

* Power in motor; Current in Motor; Battery voltage.....

The board is compose of 2 processor from Microship

* Motor Drive --> PIC16F1509

* Measurement --> PIC12F1822


To compile or download the software in the power management card, you need to install the microship IDE environement MPLAB avalaible on the microship web site : http://www.microchip.com/mplab/mplab-x-ide. Make sure that the XC8 compiler is installed. 

Note that the .hex file is avalaible for the PIC16F and only the sources for the PIC12F.





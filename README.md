# NUWC-SDP17

This is the repository for SDP Team #17 for the NUWC's Integrated Chip Configurable Testbed Project

## Serial Link Test Setup

Setup is done through emulation here. Before running the program, you must open a terminal and run the following command: (socat library required -- Mac, Linux, WSL) socat -d -d pty,raw,echo=0 pty,raw,echo=0

This command creates two virtualized serial ports that have a TX port connected to RX and vice versa on either port. First two lines of the output contain the testing port names, however future implementation will auto populate the serial ports from the STM32.
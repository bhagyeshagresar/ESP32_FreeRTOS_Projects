# Preemptive Scheduling Challenge
Using FreeRTOS, create two separate tasks. One listens for an integer over UART (from the Serial Monitor) and sets a variable when it sees an integer. The other task blinks the onboard LED (or other connected LED) at a rate specified by that integer. In effect, you want to create a multi-threaded system that allows for the user interface to run concurrently with the control task (the blinking LED).


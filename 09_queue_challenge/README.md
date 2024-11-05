# Queue Challenge

Use FreeRTOS to create two tasks and two queues. 

Task A should print any new messages it receives from Queue 2. Additionally, it should read any Serial input from the user and echo back this input to the serial input. If the user enters *delay* followed by a space and a number, it should send that number to Queue 1.

Task B should read any messages from Queue 1. If it contains a number, it should update its delay rate to that number (milliseconds). It should also blink an LED at a rate specified by that delay. Additionally, every time the LED blinks 100 times, it should send the string *Blinked* to Queue 2. You can also optionally send the number of times the LED blinked (e.g. 100) as part of struct that encapsulates the string and this number.
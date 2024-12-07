# Mutex Challenge

Starting with the code given below, modify it to protect the task parameter (delay_arg) with a mutex. With the mutex in place, the task should be able to read the parameter (parameters) into the local variable (num) before the calling function’s stack memory goes out of scope (the value given by delay_arg).
# Semaphore Challenge

use a mutex and counting semaphores to protect the shared buffer so that each number (0 throguh 4) is printed exactly 3 times to the Serial monitor (in any order).
Since the program uses a circular buffer we need to make sure that the new values do not overwrite the old values and values are not read if there is no element present in the buffer


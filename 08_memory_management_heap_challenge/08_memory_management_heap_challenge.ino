//Memory Management Challenge: using Heap

//Include
#include <string.h>



// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


//Define the max string input size
#define BUFFER_LENGTH 20

//Define the global variables
char *ptr = NULL;
static char buff[BUFFER_LENGTH] = {0};
static uint8_t index = 0;

//create the two tasks

//Task1 : Listens to an input over Serial and store the characters in a string in heap
void allocateHeap(void *parameter){

  while(1){

    if(Serial.available() > 0){

      char c = Serial.read();
      buff[index] = c;
      index++;
      

      Serial.print(c);

      if(c == '\n'){

         ptr = (char*)pvPortMalloc(BUFFER_LENGTH*sizeof(char));



      }




    

    }


  }



}




//Task2 : Print message stored in heap memory and frees the heap memory
void printMessage(void *parameter){

  while(1){

  }

}

void setup() {

  Serial.begin(115200);

  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Heap Demo---");


  //Start the tasks
  xTaskCreatePinnedToCore(allocateHeap,
                         "Allocate Heap memory task",
                         1024,
                         NULL,
                         1,
                         NULL,
                         app_cpu
                         );



  
  xTaskCreatePinnedToCore(printMessage,
                        "print message task",
                        1024,
                        NULL,
                        1,
                        NULL,
                        app_cpu
                        );

  
  //Delete Setup and Loop
  vTaskDelete(NULL);


}

void loop() {

}

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
static volatile uint8_t isMessageReady = 0;

//create the two tasks
//Task1 : Listens to an input over Serial and store the characters in a string in heap
void allocateHeap(void *parameter){

  static char buff[BUFFER_LENGTH];
  static uint8_t index = 0;

  memset(buff, 0, BUFFER_LENGTH);
  
  while(1){
    
    if(Serial.available() > 0){

      char c = Serial.read();
      //Serial.print(c);


      if(index < BUFFER_LENGTH-1){
        buff[index] = c;
        index++;
        

      }

      uint8_t actual_buffer_size = strlen(buff);
      Serial.println(actual_buffer_size);



      if(c == '\n'){
        
         //ptr = (char*)pvPortMalloc(BUFFER_LENGTH*sizeof(char));
         uint8_t actual_buffer_size = strlen(buff);
         Serial.println(actual_buffer_size);

        

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
  Serial.println("Enter string");

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

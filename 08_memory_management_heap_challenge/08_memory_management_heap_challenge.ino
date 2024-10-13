//Memory Management Challenge: using Heap

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif





//create the two tasks

//Task1 : Listens to an input over Serial and store the characters in a string in heap
void allocateHeap(void *parameter){

  while(1){

    while(Serial.available() > 0){

      char incomingByte = Serial.read();
      Serial.print(incomingByte);

      char *ptr = (char*)pvPortMalloc(400*sizeof(char));

      if(ptr == NULL){
        Serial.print("unable to allocate memory buffer");
      }
      else{

        if(incomingByte == '\n'){

        }
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

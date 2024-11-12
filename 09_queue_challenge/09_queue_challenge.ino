//FreeRTOS Queue Challenge

#include <string.h>
#include <stdlib.h>

// Use only 1 core
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


//Define the buffer length
#define BUFFER_LEN 100

//Configure the Pin
static const int led_pin = LED_BUILTIN;

//Set the length of the queues
static const uint8_t msg_queue_len = 10;

//Global led delay variable
static volatile int led_delay = 300;


//Global Handles for the two queues
static QueueHandle_t msg_queue1;
static QueueHandle_t msg_queue2;


void sendDelay(void *parameter){

  //Copy the message stored in Queue2 in this buffer
  char from_queue2[BUFFER_LEN];

  //Copy the message sent over serial if it contains "delay xx"
  char to_terminal[BUFFER_LEN];
  uint8_t index = 0;
  char c;
  char* p;

  //initial an empty buffer of chars
  memset(from_queue2, 0, BUFFER_LEN);

  while(1){

    //See if there is any "blinked" message in queue2. If so, print to the terminal
    if(xQueueReceive(msg_queue2, (void*)&from_queue2, 0) == pdTRUE){

      int n = strlen(from_queue2);
      for(int i = 0; i < n; i++){
        Serial.print(from_queue2[i]);
      }
    }

    //Read data from serial and echo back. Check if the serial input says "delay x". 
    //If so, copy the delay time to the to_queue1 buffer and send to to queue1
    if(Serial.available() > 0){

      c = Serial.read();

    
      if(c == '\n' || c == '\r'){
        //we have reached the end of the string. Check for "delay x" usting strstr function
        p = strstr(to_terminal, "delay is ");

        //this is a string literal. if you do p + n, you are pointing to unknown memory location
        //p = "delay is ";

        uint8_t n = strlen(p);
        
        //char* sp = p;

        //Get the number from the substring
        char* str = to_terminal + n;
        led_delay = atoi(str);


       

        memset(to_terminal, 0, BUFFER_LEN);
        index = 0;
      
      }

      else{

        if(index < BUFFER_LEN-1){
          to_terminal[index] = c;
          index++;
        }

      }

      

    }

      //wait for 1secdelay 500
      //vTaskDelay(1000/portTICK_PERIOD_MS);
     


    

   

  }
}




// void blinkLED(void *parameter){

//   char buffer2[] = "blinked";

//   while(1){

//     //Lets block the task for 10 ticks just to make sure there is space in the queue
//     // if(xQueueSend(msg_queue2, (void*)&buffer2, 10) != pdTRUE){
//     //   Serial.println("Queue2 is full");

//     // }

//     // vTaskDelay(1000/portTICK_PERIOD_MS);

//   }

// }



void setup() {

  //Configure the serial
  Serial.begin(115200);

  //Wait for a moment so that we do not miss serial output
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Queue Challenge");

  //Create two queues of specific length
  msg_queue1 = xQueueCreate(msg_queue_len, BUFFER_LEN);
  msg_queue2 = xQueueCreate(msg_queue_len, BUFFER_LEN);

  //start the two tasks
  xTaskCreatePinnedToCore(sendDelay,
                          "Send Delay",
                          2048,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  
  // xTaskCreatePinnedToCore(blinkLED,
  //                         "Blink LED",
  //                         1024,
  //                         NULL,
  //                         1,
  //                         NULL,
  //                         app_cpu);

  vTaskDelete(NULL);




}

void loop() {
  // put your main code here, to run repeatedly:

}

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

static char buffer2[] = "blinked";

//Configure the Pin
static const int led_pin = LED_BUILTIN;

//Set the length of the queues
static const uint8_t msg_queue_len = 10;

//Global led delay variable
static volatile int led_delay = 300;


//Global Handles for the two queues
static QueueHandle_t delay_queue;
static QueueHandle_t blinked_queue;

//Prints any new message from the blinked_queue
//Reads serial input from the user and echoes back
//If there is a xxx delay in microseconds, sends it to delay_queue
void sendDelay(void *parameter){

  //Copy the message stored in blinked queue in this buffer
  char from_blinked_queue[BUFFER_LEN];

  //Copy the message sent over serial if it contains "delay xx"
  char from_serial[BUFFER_LEN];
  uint8_t index = 0;
  char c;
  char* p;

  //initial an empty buffer of chars
  memset(from_blinked_queue, 0, BUFFER_LEN);
  memset(from_serial, 0, BUFFER_LEN);

  while(1){

    //See if there is any "blinked" message in blinked queue. If so, print to the terminal
    if(xQueueReceive(blinked_queue, (void*)&from_blinked_queue, 0) == pdTRUE){

      int n = strlen(from_blinked_queue);
      for(int i = 0; i < n; i++){
        Serial.print(from_blinked_queue[i]);
      }
      Serial.println();
    }

    //Read data from serial and echo back. Check if the serial input says "delay x". 
    //If so, copy the delay time to the to_queue1 buffer and send to to queue1
    if(Serial.available() > 0){
      c = Serial.read();

      if(c == '\n' || c == '\r'){
        //we have reached the end of the string. Check for "delay x" usting strstr function
        p = strstr(from_serial, "delay is ");

        //this is a string literal. if you do p + n, you are pointing to unknown memory location
        //p = "delay is ";

        uint8_t n = strlen(p);
        
        //Get the number from the substring
        char* str = from_serial + n;
        led_delay = atoi(str);

        //Echo the user input back to serial terminal
        Serial.println(from_serial);

        //send the number to delay_queue
        if(xQueueSend(delay_queue, (void*)&led_delay, 10) != pdTRUE){

          Serial.println("failed to send message to queue 1");
        }

        memset(from_serial, 0, BUFFER_LEN);
        index = 0;
      
      }

      else{

        if(index < BUFFER_LEN-1){
          from_serial[index] = c;
          index++;
        }

      }

    }

  }
}



//check to see if there is a new delay in the delay_queue
//blink the LED at that rate
//Send "blinked" everytime the LED blinks 100 times
void blinkLED(void *parameter){

  
  int read_delay = 0;
  int counter = 0;

  //Setup the pin
  pinMode(LED_BUILTIN, OUTPUT);

  while(1){

    //check to see if there is led delay number in delay_queue
    if(xQueueReceive(delay_queue, (void*)&read_delay, 0) == pdTRUE){

      //update the delay 
      led_delay = read_delay;

      //Serial.println("Updated the LED rate");

    }

    //blink the led at the current rate
    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_delay/portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_delay/portTICK_PERIOD_MS);

    counter++;

    if(counter == 20){

      //Serial.println("Counter reached 100");
      
      //Lets block the task for 10 ticks just to make sure there is space in the queue
      xQueueSend(blinked_queue, (void*)&buffer2, 10);
      counter = 0;

    }

    //vTaskDelay(1000/portTICK_PERIOD_MS);

  }

}



void setup() {

  //Configure the serial
  Serial.begin(300);

  //Wait for a moment so that we do not miss serial output
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Queue Challenge");

  //Create two queues of specific length
  delay_queue = xQueueCreate(msg_queue_len, sizeof(int));
  blinked_queue = xQueueCreate(msg_queue_len, sizeof(buffer2));

  //start the two tasks
  xTaskCreatePinnedToCore(sendDelay,
                          "Send Delay",
                          2048,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  
  xTaskCreatePinnedToCore(blinkLED,
                          "Blink LED",
                          2048,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  //Delete setup and loop tasks
  vTaskDelete(NULL);


}

void loop() {
  // put your main code here, to run repeatedly:

}

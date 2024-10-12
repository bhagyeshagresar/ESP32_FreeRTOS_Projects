//This program creates two tasks, one to read integer over serial and the other task to use the integer rate to blink the LED onboard ESP32
//The program runs as expected when both the tasks are running at the same priority. Why?
//The program does not blink the LED when the readSerial task is at higher priority. Why?
//The program blinks LED when the readSerial task is of lower priority than the blinkLED task. Why?
//If we change the priority of blinkLED to 1, the task does not even execute. Why?

// A multithreaded program to demonstrate preemptive scheduling
#include <stdlib.h>


#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


//Assign Global variables


// Settings

//Assign the rate for the LED
static int led_rate = 500;

//Assign variable for the LED pin
static const int led_pin = LED_BUILTIN;


//Define the Task Handles
static TaskHandle_t task1 = NULL;
static TaskHandle_t task2 = NULL;
//static TaskHandle_t task2 = NULL;


//Define the tasks

//Task 1: This tasks reads an integer over UART. The onobard LED blinks at the specified integer rate
void readSerial(void *parameters) {

  while(1){

    if(Serial.available() > 0){
      int value = Serial.parseInt();
      led_rate = value;
      Serial.print("Entered Number:");
      Serial.println(led_rate);



    }

  }  
  
}





//Task 2 : blink the onboard LED at the specified rate
void blinkLED(void *parameter){

  Serial.print("Blinking LED");

  while(1){

    digitalWrite(led_pin, HIGH);
    vTaskDelay(led_rate/portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(led_rate/portTICK_PERIOD_MS);

  }
  


}





void setup() {

  // Configure pin
  pinMode(led_pin, OUTPUT);

  // Configure serial and wait a second
  Serial.begin(300);
  
  //wait for serial connection
  //while(!Serial){

  //}


  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Multi-task LED Demo");
  Serial.println("Enter a number in milliseconds to change the LED delay.");

  
            
  // Start serial read task
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
            blinkLED,     // Function to be called
            "Blink LED",  // Name of task
            1024,           // Stack size (bytes in ESP32, words in FreeRTOS)
            NULL,           // Parameter to pass
            2,              // Task priority (must be same to prevent lockup)
            &task1,           // Task handle
            app_cpu);       // Run on one core for demo purposes (ESP32 only)



   // Start serial read task
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
            readSerial,     // Function to be called
            "Read Serial",  // Name of task
            1024,           // Stack size (bytes in ESP32, words in FreeRTOS)
            NULL,           // Parameter to pass
            1,              // Task priority (must be same to prevent lockup)
            &task2,           // Task handle
            app_cpu);       // Run on one core for demo purposes (ESP32 only)


  // Delete "setup and loop" task
  //vTaskDelete(NULL);

 


}

void loop() {
  // put your main code here, to run repeatedly:

 

 
    

}

//A Basic challenge to demonstrate how to blink an LED at two different rates using two separate threads




#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

//Configure the Pin
static const int led_pin = LED_BUILTIN;


//Both the tasks are set at the same priority
// Note that both setup() and loop() functions run in their own task for the Arduino version of ESP-IDF. 
//Because all three tasks (Toggle 1, Toggle 2, and “Setup and Loop”) are priority 1, the processor will split its time equally among them.

// Task : Blink an LED at 2000ms
void toggleLED_task1(void *parameter){
  while(1){

    digitalWrite(led_pin, HIGH);
    vTaskDelay(500/portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500/portTICK_PERIOD_MS); //FreeRTOS defines portTICK_PERIOD_MS to 1ms

  }

}


// Task : Blink an LED at 200ms
void toggleLED_task2(void *parameter){
  while(1){

    digitalWrite(led_pin, HIGH);
    vTaskDelay(323/portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(323/portTICK_PERIOD_MS); //FreeRTOS defines portTICK_PERIOD_MS to 1ms

  }

}

void setup() {
  
  //configure pin
  pinMode(led_pin, OUTPUT);

   //Task to run forever
   xTaskCreatePinnedToCore( //use xTaskCreate() in vanilla FreeRTOS
    toggleLED_task1,             //Function to be called
    "Toggle LED at 2000ms",          //Name of task
    1024,                  //Stack size (bytes in ESP32, words in Vanilla FreeRTOS)
    NULL,                  //Parameter to pass to function
    1,                     //Task Priority
    NULL,                  //Task Handle
    app_cpu);              //Run on one core


  //Task to run forever
   xTaskCreatePinnedToCore( //use xTaskCreate() in vanilla FreeRTOS
    toggleLED_task2,             //Function to be called
    "Toggle LED at 200ms",          //Name of task
    1024,                  //Stack size (bytes in ESP32, words in Vanilla FreeRTOS)
    NULL,                  //Parameter to pass to function
    1,                     //Task Priority
    NULL,                  //Task Handle
    app_cpu);              //Run on one core





}

void loop() {
  // Do nothing
  // setup() and loop() run in their own task with priority 1 in core 1
  // on ESP32

}

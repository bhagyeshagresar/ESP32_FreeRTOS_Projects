// Use only core 1
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif


// Pins
static const int led_pin = LED_BUILTIN;

// Task : Blink an LED
void toggleLED(void *parameter){
  while(1){

    digitalWrite(led_pin, HIGH);
    vTaskDelay(500/portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(500/portTICK_PERIOD_MS); //FreeRTOS defines portTICK_PERIOD_MS to 1ms

  }
}



void setup() {
  
  //configure pin
  pinMode(led_pin, OUTPUT);

  //Task to run forever
  xTaskCreatePinnedToCore( //use xTaskCreate() in vanilla FreeRTOS
    toggleLED,             //Function to be called
    "Toggle LED",          //Name of task
    1024,                  //Stack size (bytes in ESP32, words in Vanilla FreeRTOS)
    NULL,                  //Parameter to pass to function
    1,                     //Task Priority
    NULL,                  //Task Handle
    app_cpu);              //Run on one core


  //If this was vanilla FreeRTOS, we need to call vTaskStartScheduler() in
  //main after setting up your tasks




  

}

void loop() {
  // put your main code here, to run repeatedly:

}

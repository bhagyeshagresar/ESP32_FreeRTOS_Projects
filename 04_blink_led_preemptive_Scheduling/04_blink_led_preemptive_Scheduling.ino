// A multithreaded program to demonstrate preemptive scheduling

#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


//Assign Global variables
char inputByte = 0;
int inputLEDRate = 0;



//Assign the rate for the LED
static int led_rate = 0;

//Assign variable for the LED pin
static const int led_pin = LED_BUILTIN;


//Define the Task Handles
static TaskHandle_t task1 = NULL;
static TaskHandle_t task2 = NULL;


//Define the tasks

//Task 1: This tasks reads an integer over UART. The onobard LED blinks at the specified integer rate
void readInteger(void *parameter){

  //if(Serial.available() > 0){

    //inputByte = Serial.read();
    //inputLEDRate = Serial.parseInt();
    //Serial.println(inputLEDRate);
    
  //}

}





//Task 2 : blink the onboard LED at the specified rate
void blinkLED(void *parameter){

}





void setup() {

  //configure the LED pin as output
  pinMode(led_pin, OUTPUT);

  //Start the serial monitor
  Serial.begin(9600);

  xTaskCreatePinnedToCore(readInteger,
                           "Task 1",
                           1024,
                           NULL,
                           1,
                           &task1,
                           app_cpu);


   xTaskCreatePinnedToCore(blinkLED,
                           "Task 2",
                           1024,
                           NULL,
                           1,
                           &task2,
                           app_cpu);

  


}

void loop() {
  // put your main code here, to run repeatedly:

  

}

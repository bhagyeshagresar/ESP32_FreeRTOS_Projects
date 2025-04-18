/**
 * FreeRTOS Binary Semaphore Example
 * 
 * Pass a parameter to a task using a binary semaphore
 */


// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Define the LED Pin
static const int led_pin = LED_BUILTIN;
static SemaphoreHandle_t bin_sem; 

// Tasks

// Blink LED based on rate passed by parameter
void blinkLED(void *parameters) {

  // Copy the parameter into a local variable
  int num = *(int *)parameters;

  //Release the binary semaphore so that the creating function can finish
  xSemaphoreGive(bin_sem);

  // Print the parameter
  Serial.print("Received: ");
  Serial.println(num);

  // Configure the LED pin
  pinMode(led_pin, OUTPUT);

  // Blink forever and ever
  while (1) {
    digitalWrite(led_pin, HIGH);
    vTaskDelay(num / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(num / portTICK_PERIOD_MS);
  }
}

//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup() {

  long int delay_arg;

  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Mutex Challenge---");
  Serial.println("Enter a number for delay (milliseconds)");

  

  // Wait for input from Serial
  while (Serial.available() <= 0);

 
  // Read integer value
  delay_arg = Serial.parseInt();
  Serial.print("Sending: ");
  Serial.println(delay_arg);


  //create a binary semaphore before starting tasks
  bin_sem = xSemaphoreCreateBinary();


     // Start task 1
  xTaskCreatePinnedToCore(blinkLED,
                          "Blink LED",
                          1024,
                          (void *)&delay_arg,
                          1,
                          NULL,
                          app_cpu);

  
   //Do nothing until binary semaphore has been returned
   xSemaphoreTake(bin_sem, portMAX_DELAY);    


  // Show that we accomplished our task of passing the stack-based argument
  Serial.println("Done!");
}

void loop() {
  
  // Do nothing but allow yielding to lower-priority tasks
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
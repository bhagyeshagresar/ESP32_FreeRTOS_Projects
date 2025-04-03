//Deadlock prevention using heirarchy (also solves livelock) - Credits Shawyn Hymel

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


// Globals
static SemaphoreHandle_t mutex_1;
static SemaphoreHandle_t mutex_2;


void doTaskA(void* parameters){

  while(1){
     //Take mutex1
    xSemaphoreTake(mutex_1, portMAX_DELAY); // Indefinitely wait until semaphore is released
    Serial.println("Task A took mutex 1");
    vTaskDelay(1/portTICK_PERIOD_MS); //Force a deadlock

    //Take mutex2
    xSemaphoreTake(mutex_2, portMAX_DELAY);
    Serial.println("Task A took mutex 2");

    // Critical section protected by 2 mutexes
    Serial.println("Task A doing some work");
    vTaskDelay(500 / portTICK_PERIOD_MS);

     // Give back mutexes (in reverse order that we took them) - Its a good practice
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);

    // Wait to let the other task execute
    Serial.println("Task A going to sleep");
    vTaskDelay(500 / portTICK_PERIOD_MS);


  }
  
}

void doTaskB(void* parameters){

  while(1){
     //Take mutex1
    xSemaphoreTake(mutex_1, portMAX_DELAY); // Indefinitely wait until semaphore is released
    Serial.println("Task B took mutex 1");
    vTaskDelay(1/portTICK_PERIOD_MS); //Force a deadlock

    //Take mutex2
    xSemaphoreTake(mutex_2, portMAX_DELAY);
    Serial.println("Task B took mutex 2");

    // Critical section protected by 2 mutexes
    Serial.println("Task B doing some work");
    vTaskDelay(500 / portTICK_PERIOD_MS);

     // Give back mutexes (in reverse order that we took them) - Its a good practice
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);

    // Wait to let the other task execute
    Serial.println("Task B going to sleep");
    vTaskDelay(500 / portTICK_PERIOD_MS);


  }
  
}

void setup() {
   // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Deadlock Heirarchy---");

  // Create mutexes before starting tasks
  mutex_1 = xSemaphoreCreateMutex();
  mutex_2 = xSemaphoreCreateMutex();

  // Start Task A (high priority)
  xTaskCreatePinnedToCore(doTaskA,
                          "Task A",
                          1024,
                          NULL,
                          2,
                          NULL,
                          app_cpu);

  // Start Task B (low priority)
  xTaskCreatePinnedToCore(doTaskB,
                          "Task B",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}

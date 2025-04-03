//Starvation happens when high priority tasks keep executing.
//This program tried to prevent starvation by keeping a task delay so that lower priority tasks can run.
//Both tasks run with different priorities but even with same priority deadlock still occurs
//Credits: Shawyn Hymel RTOS series

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

// Globals
static SemaphoreHandle_t mutex_1;
static SemaphoreHandle_t mutex_2;

// Task A (high priority)
void doTaskA(void *parameters) {

  // Loop forever
  while (1) {

    // Take mutex 1 (introduce wait to force deadlock)
    xSemaphoreTake(mutex_1, portMAX_DELAY); // Indefintiely wait until the semaphore is released
    Serial.println("Task A took mutex 1");
    //Serial.println()
    vTaskDelay(1 / portTICK_PERIOD_MS); //Force a deadlock

    // Take mutex 2
    xSemaphoreTake(mutex_2, portMAX_DELAY);
    Serial.print("Task A took mutex 2");

    // Critical section protected by 2 mutexes
    Serial.print("Task A doing some work");
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // Give back mutexes
    xSemaphoreGive(mutex_2);
    xSemaphoreGive(mutex_1);

    // Wait to let the other task execute
    Serial.print("Task A going to sleep");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}


// Task B (low priority)
void doTaskB(void *parameters) {

  // Loop forever
  while (1) {

    // Take mutex 2 (introduce wait to force deadlock)
    xSemaphoreTake(mutex_2, portMAX_DELAY);
    Serial.println("Task B took mutex 2");
    //Serial.println()
    vTaskDelay(1 / portTICK_PERIOD_MS);

    // Take mutex 1
    xSemaphoreTake(mutex_1, portMAX_DELAY);
    Serial.print("Task B took mutex 1");

    // Critical section protected by 2 mutexes
    Serial.print("Task B doing some work");
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // Give back mutexes
    xSemaphoreGive(mutex_1);
    xSemaphoreGive(mutex_2);

    // Wait to let the other task execute
    Serial.print("Task B going to sleep");
    vTaskDelay(500 / portTICK_PERIOD_MS);
  }
}



void setup() {
   // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Deadlock Demo---");

  // Create mutexes before starting tasks
  mutex_1 = xSemaphoreCreateMutex();
  mutex_2 = xSemaphoreCreateMutex();

  // Start Task A (high priority)
  xTaskCreatePinnedToCore(doTaskA,
                          "Task A",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Start Task B (low priority)
  xTaskCreatePinnedToCore(doTaskB,
                          "Task B",
                          1024,
                          NULL,
                          2,
                          NULL,
                          app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);

}

void loop() {
 //Execution should never get here

}

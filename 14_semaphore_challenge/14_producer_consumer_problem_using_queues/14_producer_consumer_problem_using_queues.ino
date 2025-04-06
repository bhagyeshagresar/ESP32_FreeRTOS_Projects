//Same Producer Consumer Problem solved using queues. Credits: Shawyn Hymel

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


// Settings
static const int msg_queue_len = 5;
static const int num_prod_tasks = 5;  // Number of producer tasks
static const int num_cons_tasks = 2;  // Number of consumer tasks
static const int num_writes = 3;      // Num times each producer writes to buf
static QueueHandle_t msg_queue;       //Create a queue handle
static SemaphoreHandle_t bin_sem;     //create a binary semaphore handle

// Producer: write a given number of times to shared buffer
void producer(void *parameters) {
  
  int num = *(int*)parameters;

  //release the binary semaphore
  xSemaphoreGive(bin_sem);

  for(int i = 0; i < num_writes; i++){
    //wait until there is space in the queue. portMAX_DELAY is the max time
    xQueueSend(msg_queue, (void *)&num, portMAX_DELAY);
  }

  // Delete self task
  vTaskDelete(NULL);
}


void consumer(void* parameters){
  int val;

  while(1){

    //read from the queue. Wait for max time if queue is empty
    xQueueReceive(msg_queue, (void*)&val, portMAX_DELAY);
    // Lock Serial resource with a mutex
    xSemaphoreTake(mutex, portMAX_DELAY);
    Serial.println(val);
    xSemaphoreGive(mutex);

  }
  
}

void setup() {
  char task_name[12];
  
  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Semaphore Producer Consumer Challenge using Queues");

  //create a queue
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  //create binary semaphore to safely read parameters in producer task
  bin_sem = xSemaphoreCreateBinary();

  // Start producer tasks (wait for each to read argument)
  for (int i = 0; i < num_prod_tasks; i++) {
    sprintf(task_name, "Producer %i", i);
    xTaskCreatePinnedToCore(producer,
                            task_name,
                            1024,
                            (void *)&i,
                            1,
                            NULL,
                            app_cpu);
    xSemaphoreTake(bin_sem, portMAX_DELAY);
  }

  // Start consumer tasks
  for (int i = 0; i < num_cons_tasks; i++) {
    sprintf(task_name, "Consumer %i", i);
    xTaskCreatePinnedToCore(consumer,
                            task_name,
                            1024,
                            NULL,
                            1,
                            NULL,
                            app_cpu);
  }


}

void loop() {

   // Do nothing but allow yielding to lower-priority tasks
  vTaskDelay(1000 / portTICK_PERIOD_MS);

}

//Starvation happens when high priority tasks keep executing.
//This program tried to prevent starvation by keeping a task delay so that lower priority tasks can run
//Credits: Shawyn Hymel RTOS series

// Use only 1 core
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif

char msg[] = "Hello World";

// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

void startTask1(void *parameter){

  //count the number of characters in a string
  int msg_len = strlen(msg);

  //Print string to the terminal
  while(1){
    Serial.println();
    for(int i = 0; i < msg_len; i++){
      Serial.print(msg[i]);
    }
    Serial.println();
    vTaskDelay(1000/portTICK_PERIOD_MS);

  }

}


//Print to serial terminal with high priority
void startTask2(void *parameter){

  while(1){
    Serial.print('*');
    vTaskDelay(100/portTICK_PERIOD_MS);
  }


}



void setup() {
  // Configure Serial
  Serial.begin(300);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Deadlock Demo---");

  

  // Start Task A (low priority)
  xTaskCreatePinnedToCore(startTask1,
                          "Task A",
                          1024,
                          NULL,
                          1,
                          &task_1,
                          app_cpu);


  // Start Task B (High priority)
  xTaskCreatePinnedToCore(startTask2,
                          "Task B",
                          1024,
                          NULL,
                          2,
                          &task_2,
                          app_cpu);



}

void loop() {
 //Execution should never get here

}

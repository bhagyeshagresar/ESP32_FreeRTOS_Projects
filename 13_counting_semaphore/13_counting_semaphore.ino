



// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


//create the number of tasks
static const int num_tasks = 5;

//create a struct
typedef struct Message{
  char body[20];
  uint8_t len;
}Message;


//create the semaphore handle
static SemaphoreHandle_t sem_params; //counts down when parameters are read


//create the task
void myTask(void* parameters){

  // Copy the message struct from the parameter to a local variable
  Message msg = *(Message *)parameters;

  // Increment semaphore to indicate that the parameter has been read
  xSemaphoreGive(sem_params);

  // Print out message contents
  Serial.print("Received: ");
  Serial.print(msg.body);
  Serial.print(" | len: ");
  Serial.println(msg.len);

  // Wait for a while and delete self
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  vTaskDelete(NULL);


}



void setup() {

   char task_name[12];
  Message msg;
  char text[20] = "All your base";
  
  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Counting Semaphore Demo---");

  // Create semaphores (initialize at 0)
  sem_params = xSemaphoreCreateCounting(num_tasks, 0);

  // Create message to use as argument common to all tasks
  strcpy(msg.body, text);
  msg.len = strlen(text);

  // Start tasks
  for (int i = 0; i < num_tasks; i++) {

    // Generate unique name string for task
    sprintf(task_name, "Task %i", i);

    // Start task and pass argument (common Message struct)
    xTaskCreatePinnedToCore(myTask,
                            task_name,
                            1024,
                            (void *)&msg,
                            1,
                            NULL,
                            app_cpu);
  }

  // Wait for all tasks to read shared memory
  for (int i = 0; i < num_tasks; i++) {
    xSemaphoreTake(sem_params, portMAX_DELAY);
  }

  // Notify that all tasks have been created
  Serial.println("All tasks created");




}

void loop() {
  // put your main code here, to run repeatedly:

}

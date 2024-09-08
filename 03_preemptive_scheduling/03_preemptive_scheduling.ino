//Use only 1 core
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


//some string to print
const char msg[] = "Introduction to RTOS";


//Task Handles
static TaskHandle_t task1 = NULL;
static TaskHandle_t task2 = NULL;

//Define the Tasks

//Task 1 : print to serial terminal with lower priority
void startTask1(void *parameter){

  //count the number of characters in a string
  int msg_len = strlen(msg);

  //print the string to terminal
  while(1){

    Serial.println();
    for(int i = 0; i < msg_len; i++){
      Serial.print(msg[i]);
    }
    //Serial.print(msg);
    Serial.println();
    vTaskDelay(300/portTICK_PERIOD_MS);

  }


}

//Task 2 : print to serial terminal with higher priority
void startTask2(void *parameter){
  while(1){
    Serial.print('*');
    vTaskDelay(100/portTICK_PERIOD_MS);
  }
}



// Main (runs as its own task with priority 1 on core 1)



void setup() {
  //Configure Serial to go slow
  Serial.begin(300);


  //Wait for some delay so that we do not miss the serial output
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Task Demo---");

  //Print self priority
  Serial.print("setup and loop task running on core ");
  Serial.print(xPortGetCoreID());
  Serial.print(" with priority ");
  Serial.println(uxTaskPriorityGet(NULL));

  //Task to run forever
  xTaskCreatePinnedToCore(startTask1,
                           "Task 1",
                           1024,
                           NULL,
                           1,
                           &task1,
                           app_cpu);


  //Task to run with higher priority
  xTaskCreatePinnedToCore(startTask2,
                           "Task 2",
                           1024,
                           NULL,
                           2,
                           &task2,
                           app_cpu);





}

void loop() {




}

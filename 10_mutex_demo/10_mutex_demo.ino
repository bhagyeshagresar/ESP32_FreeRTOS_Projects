
// Use only 1 core
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


//Global
static int shared_var = 0;
static SemaphoreHandle_t mutex;

//increment shared variable (the wrong way)
void incTask(void* parameter){

  int local_var;

  while(1){

    //Take mutex prior to critical section
    if(xSemaphoreTake(mutex, 0) == pdTRUE){

        local_var = shared_var;
        local_var++;
        vTaskDelay(random(100,500)/portTICK_PERIOD_MS);
        shared_var = local_var;

        //Give mutex after critical section
        xSemaphoreGive(mutex);

        //print the shared variable
        Serial.println(shared_var);

    }
    else{
      //do something if you cannot obtain the mutex
    }



  
  }



}


void setup() {

  randomSeed(analogRead(0));

  Serial.begin(115200);

  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS mutex Demo");

  //create mutex before starting tasks
  mutex = xSemaphoreCreateMutex();


  //start task 1
  xTaskCreatePinnedToCore(incTask,
                          "Increment task 1",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  //start task 2
  xTaskCreatePinnedToCore(incTask,
                          "Increment Task 2",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  vTaskDelete(NULL);


}




void loop() {

}

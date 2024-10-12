//This program demonstrates stack overflow when using FreeRTOS


// Use only one core
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


// Task : Perform some mundane task
void testTask(void *parameter){

  while(1){
    int a = 1;
    int b[100]; // int is of 4 byte, so the array is of 400 bytes, this will result in stack overflow

    //Do something with the array so that it is not optimized out by the compiler
    for(int i = 0; i < 100; i++){
      b[i] = a + 1;
    }

    Serial.println(b[0]);
  }


}




void setup() {
  // put your setup code here, to run once:

  //Configure the serial
  Serial.begin(115200);

  //Wait a moment to start so we don't miss serial output
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Memory Demo---");

  //start the task
  //Around 700 KB of stack is overhead. 
  xTaskCreatePinnedToCore(testTask,
                          "Test Task",
                           1500,
                           NULL,
                           1,
                           NULL,
                           app_cpu);


  // Delete Setup and Loop
  vTaskDelete(NULL);

}

void loop() {
  // put your main code here, to run repeatedly:

}

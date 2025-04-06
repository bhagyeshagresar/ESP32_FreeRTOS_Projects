
//Demo program to understand the use of Queues in RTOS
//The program drops items if the queue is full
//The variable that is read from the queue does not change if the queue is being read faster than the rate at which data is sent




// Use only 1 core
#if CONFIG_FREERTOS_UNICORE
  static const BaseType_t app_cpu = 0;
#else
  static const BaseType_t app_cpu = 1;
#endif


//Assign the length of the queue
static const uint8_t msg_queue_len = 5;

//Global Handle for queue
static QueueHandle_t msg_queue;


//Tasks
//This task reads values from the FreeRTOS queue and prints on serial monitor
void printMessages(void *parameter){

  int item;
  while(1){

    //See if there is a message in the queue(do not block)
    if (xQueueReceive(msg_queue, (void *)&item, 0) == pdTRUE){

      //print only if there is message in the queue
      Serial.println(item);
    }

    //Serial.println(item);

    //Wait for 1 sec
    vTaskDelay(300/portTICK_PERIOD_MS);


  }


}


//Setup runs its own task with priority 1 on core 1

void setup() {

  //Configure the serial
  Serial.begin(115200);

  //Wait for a moment so that we do not miss serial output
  vTaskDelay(1000/portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Queue Demo");

  //Create a queue of specific length and of particular datatype
  msg_queue = xQueueCreate(msg_queue_len, sizeof(int));

  //Start a print task
  xTaskCreatePinnedToCore(printMessages,
                          "Print Messages",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);



}

void loop() {

  static int num = 0;

  //Block the task for 10 ticks so that there is space available on the queue
  if(xQueueSend(msg_queue, (void*)&num, 10) != pdTRUE){
    Serial.println("Queue Full");
  }
  num++;

  vTaskDelay(1000/portTICK_PERIOD_MS);
}

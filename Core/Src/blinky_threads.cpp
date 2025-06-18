#include "page.h"
#include <stdio.h>
#include <stdint.h>


#include "PFunction.h"
#include "PTools.h"
#include "PWriter.h"
//#include "rtos/MemoryPoolRTOS.h"
#include "PStack.h"
#include "json/Node.h"
#include "somePages.h"
#include "PMutex.h"

extern "C" {
#include "main.h" // Für C-Funktionen wie HAL_Delay
void http_write (struct netconn * conn, const char * data, size_t len);
}


using namespace ptools;
using namespace ptools::json;

static volatile uint32_t msDelayYellow = 500;
static volatile uint32_t msDelayRed = 500;

uint32_t get_blinky_yellow_delay()
{
    uint32_t val;
    taskENTER_CRITICAL();
    val = msDelayYellow;
    taskEXIT_CRITICAL();
    return val;
}

void set_blinky_yellow_delay(uint32_t msDelay)
{
    taskENTER_CRITICAL();
    msDelayYellow = msDelay;
    taskEXIT_CRITICAL();
}
uint32_t get_blinky_red_delay()
{
    uint32_t val;
    taskENTER_CRITICAL();
    val = msDelayRed;
    taskEXIT_CRITICAL();
    return val;

}
void set_blinky_red_delay(uint32_t msDelay)
{
    taskENTER_CRITICAL();
    msDelayRed = msDelay;
    taskEXIT_CRITICAL();

}


void cpp_th_blink_yellow (void * arg)
{
    while (1)
    {
//        vTaskDelay(pdMS_TO_TICKS(get_blinky_yellow_delay()));
        sleep_milliseconds(get_blinky_yellow_delay());
        BSP_LED_Toggle(LED_YELLOW);
    }
}

void cpp_th_blink_red (void * arg)
{
    while (1)
    {
//        vTaskDelay(pdMS_TO_TICKS(get_blinky_red_delay()));
        sleep_milliseconds(get_blinky_red_delay());
        BSP_LED_Toggle(LED_RED);
    }
}



extern "C" {
void th_blink_yellow (void * arg)
{
    cpp_th_blink_yellow(arg);
}

void th_blink_red (void * arg)
{
    cpp_th_blink_red(arg);
}
}


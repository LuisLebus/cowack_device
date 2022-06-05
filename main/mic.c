/**
 * @file mic.c
 * @author
 * @date
 * @brief
 */

//=============================================================================
// [Inclusions] ===============================================================
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"

#include "driver/gpio.h"

#include "mic.h"
#include "gps.h"
#include "bat.h"
#include "adxl345.h"
#include "leds.h"
#include "lora.h"

//=============================================================================

//=============================================================================
// [Private Defines] ==========================================================
#define MIC_INT_BIT     BIT0

#define MIC_INT_GPIO    GPIO_NUM_2

//=============================================================================

//=============================================================================
// [Local Typedef] ============================================================

//=============================================================================

//=============================================================================
// [External Data Definitions] ================================================

// Const ---------------------------------------------
//----------------------------------------------------

// Vars ----------------------------------------------
//----------------------------------------------------

// Task Handlers -------------------------------------
//----------------------------------------------------

// Queue Handlers ------------------------------------
//----------------------------------------------------

// Event Group Handlers ------------------------------
//----------------------------------------------------

// Semaphore Handlers --------------------------------
//----------------------------------------------------

//=============================================================================

//=============================================================================
// [Local Data Declarations] ==================================================

// Const ---------------------------------------------
//----------------------------------------------------

// Vars ----------------------------------------------
//----------------------------------------------------

// Task Handlers -------------------------------------
//----------------------------------------------------

// Queue Handlers ------------------------------------
//----------------------------------------------------

// Event Group Handlers ------------------------------
static EventGroupHandle_t mic_ev_group = NULL;

//----------------------------------------------------

// Semaphore Handlers --------------------------------
//----------------------------------------------------

//=============================================================================

//=============================================================================
// [Local Function Declarations] ==============================================

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
static void mic_task(void * pvParameter);

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
static void IRAM_ATTR mic_int_gpio_isr_handler(void * arg);

//=============================================================================

//=============================================================================
// [FreeRTOS Task Definitions] ================================================

static void mic_task(void * pvParameter)
{
    lora_packet_t lora_packet = {0};

    vTaskDelay(pdMS_TO_TICKS(5000));

    while (1)
    {
        xEventGroupWaitBits(mic_ev_group, MIC_INT_BIT, pdTRUE, pdTRUE, portMAX_DELAY);

        lora_packet.msg_type = LORA_MSG_EVENT_MIC;

        lora_packet.latitude = gps_get_latitude();
        lora_packet.longitude = gps_get_longitude();
        lora_packet.speed = (uint8_t)gps_get_speed();
        lora_packet.course = (uint16_t)gps_get_course();
        lora_packet.sats = gps_get_sats();

        adxl345_fsample_t sample = {0};
        adxl345_get_fsample(&sample);
        lora_packet.x = (int16_t)(sample.x * 100.0);
        lora_packet.y = (int16_t)(sample.y * 100.0);
        lora_packet.z = (int16_t)(sample.z * 100.0);

        lora_packet.bat = (uint16_t)(bat_get_voltage() * 100.0);

        lora_write(&lora_packet);


        for (uint8_t i = 0; i < 1; i++)
		{
			leds_flash();
			vTaskDelay(pdMS_TO_TICKS(5000));
		}
    }
}
// End

//=============================================================================

//=============================================================================
// [Local Function Definitions] ===============================================

static void IRAM_ATTR mic_int_gpio_isr_handler(void * arg)
{
    xEventGroupSetBitsFromISR(mic_ev_group, MIC_INT_BIT, NULL);
}
// End

//=============================================================================

//=============================================================================
// [External Function Definition] =============================================

void mic_init(void)
{
    mic_ev_group = xEventGroupCreate();

    gpio_pad_select_gpio(MIC_INT_GPIO);
    gpio_set_direction(MIC_INT_GPIO, GPIO_MODE_INPUT);
    gpio_set_intr_type(MIC_INT_GPIO, GPIO_INTR_POSEDGE);

    gpio_install_isr_service(0);
    gpio_isr_handler_add(MIC_INT_GPIO, mic_int_gpio_isr_handler, NULL);

    xTaskCreate(mic_task, "mic_rx_task", configMINIMAL_STACK_SIZE * 3, NULL, tskIDLE_PRIORITY + 1, NULL);
}
// End

//=============================================================================

//====================== [End Document] =======================================

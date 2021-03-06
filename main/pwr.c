/**
 * @file pwr.c
 * @author
 * @date
 * @brief
 */

//=============================================================================
// [Inclusions] ===============================================================
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "pwr.h"
#include "gps.h"
#include "bat.h"
#include "adxl345.h"

#include "esp_log.h"
#include "lora.h"

//=============================================================================

//=============================================================================
// [Private Defines] ==========================================================
#define PWR_TAG				"[PWR]"

#define PWR_DELAY_TASK   	1000 * 2 * 1     //15 sec

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
//----------------------------------------------------

// Semaphore Handlers --------------------------------
//----------------------------------------------------

//=============================================================================

//=============================================================================
// [Local Function Declarations] ==============================================

//=============================================================================

//=============================================================================
// [FreeRTOS Task Definitions] ================================================

static void pwr_process_task(void * pvParameter)
{
    lora_packet_t lora_packet = {0};

    vTaskDelay(pdMS_TO_TICKS(5000));

    while (1)
    {
        lora_packet.msg_type = LORA_MSG_DATA;

        lora_packet.latitude = gps_get_latitude();
        lora_packet.longitude = gps_get_longitude();
        lora_packet.speed = (uint8_t)gps_get_speed();
        lora_packet.course = (uint16_t)gps_get_course();
        lora_packet.sats = gps_get_sats();

        adxl345_fsample_t sample = {0};
        adxl345_get_fsample(&sample);
        lora_packet.x = (int16_t)(sample.x * 10.0);
        lora_packet.y = (int16_t)(sample.y * 10.0);
        lora_packet.z = (int16_t)(sample.z * 10.0);

        lora_packet.bat = (uint16_t)(bat_get_voltage() * 100.0);

        lora_write(&lora_packet);

        vTaskDelay(pdMS_TO_TICKS(PWR_DELAY_TASK));
    }
}
// End static void pwr_process_task(void * pvParameter)

//=============================================================================

//=============================================================================
// [Local Function Definitions] ===============================================

//=============================================================================

//=============================================================================
// [External Function Definition] =============================================

void pwr_init(void)
{
    xTaskCreate(pwr_process_task, "pwr_proc_task", configMINIMAL_STACK_SIZE * 3, NULL, tskIDLE_PRIORITY + 1, NULL);
}
// End void pwr_init(void)


//=============================================================================

//====================== [End Document] =======================================

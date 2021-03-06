/**
 * @file gps.h
 * @author
 * @date
 * @brief
 */

#ifndef _GPS_H_
#define _GPS_H_

//=============================================================================
// [Inclusions] ===============================================================
#include <math.h>
#include <stdint.h>

//=============================================================================

//=============================================================================
// [External Defines] =========================================================

//=============================================================================

//=============================================================================
// [External Typedef] =========================================================

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
// [External Function Declarations] ===========================================

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void gps_init(void);

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
float_t gps_get_latitude(void);

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
float_t gps_get_longitude(void);

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
bool gps_get_valid(void);

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
float_t gps_get_speed(void);

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
float_t gps_get_course(void);

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
uint8_t gps_get_sats(void);

/**
 * @details
 * @param[in]
 * @param[in]
 * @return
 */
void gps_sleep(bool sleep);

//=============================================================================

#endif /* _GPS_H_ */

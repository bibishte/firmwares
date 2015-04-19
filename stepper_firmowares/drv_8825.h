/************************************************************************
 Title:    Driver for stepper motor controller based on DRV8825 by Texas Instruments
 Author:   Bilyana Borisova <bibishte@gmail.com>
 File:     $Id: drv_8825.h 2015/04/19
 Software: AVR-GCC 4.9.2, AVR Libc 1.8.0
 Hardware: DRV8825 & ATmega8
 License:  GNU General Public License v2
 Usage:    see Doxygen manual

 LICENSE:
 Copyright (C) 2015 Bilyana Borisova

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License v2 as published by
 the Free Software Foundation; either version 2 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License v2 for more details.

 ************************************************************************/



#ifndef _DRV_8825
#define _DRV_8825
/** 
 *  @defgroup biba_drv DRV8825 Library
 *  @code #include <drv_8825.h> @endcode
 * 
 *  @brief Driver for stepper motor controller based on DRV8825 by Texas Instruments 
 *
 *  This modul contains basic functions for controlling stepper motor with the Texas Instruments DRV8825
 *
 *  @note Typical application based on DRV8825 
 *  @author Bilyana Borisova bibishte@gmail.com
 */

/**@{*/



#include <stdint.h>

/**
 * @brief Type for decay status.
 * 
 * Defines the status of the decay modes.
 */
typedef enum {SLOW_DECAY,       /**< Status for setting slow decay. */
        FAST_DECAY,             /**< Status for setting fast decay. */
        AUTO_DECAY              /**< Status for setting auto decay. */
        } decay_type;
        
/**
 * @brief Type for mode status.
 * 
 * Defines the status of the modes.
 */  
typedef enum {MODE_FULL_STEP,   /**< Status for setting the full step mode. */
        MODE_HALF_STEP,         /**< Status for setting the half step mode. */
        MODE_QUATER_STEP,       /**< Status for setting the quater step mode. */
        MODE_8_MICROSTEP,       /**< Status for setting the 8 microstep mode. */
        MODE_16_MICROSTEP,      /**< Status for setting the 16 microstep mode. */
        MODE_32_MICROSTEP       /**< Status for setting the 32 microstep mode. */
        } mode_type;
        
/**
 * @brief Type for the direction status.
 * 
 * Defines the direction:clockwise or counter clock wise.
 */        
typedef enum {STEP_COUNTER_CLOCKWISE,   /**< Status for setting the clockwise direction. */
        STEP_CLOCKWISE                  /**< Status for setting the counter clockwise direction. */
        } step_type;

void SetMaxSpeed(uint16_t spd);
void SetMinSpeed(uint16_t spd);
void SetAcceleration(uint16_t accel);
void Enable_Stepper(void);
void Disabled_Stepper(void);
void Decay(decay_type decay);
void Mode(mode_type mode);
void Step(step_type step);
void Count_Step(step_type step, uint64_t step_count);
void Way_Speed(step_type step);
void Stop_Motion_normal(void);
void Stop_Motion_fast(void);
uint8_t Home(void);
decay_type Get_Decay(void);
mode_type Get_Mode(void);
uint8_t Get_Start(void);
uint16_t GetMaxSpeed(void);
uint16_t GetMinSpeed(void);
uint16_t GetAcceleration(void);
uint16_t GetCurrentSpeed(void);
void SetCurrentSpeed(uint16_t spd);
void Set_Steps_revol(uint16_t step_rev);
void SetRealMaxSpeed(uint16_t maxspd);
void SetRealMinSpeed(uint16_t minspd);
void SetRealAcc(uint16_t realacc);
uint16_t Get_Steps_revol(void);
uint16_t GetRealMaxSpeed(void);
uint16_t GetRealMinSpeed(void);
uint16_t GetRealAcc(void);
void RealToTime(void);
void TimeToReal(void);
void CalWorkParam(void);
void InitStepper(void);
void store(void);
uint64_t HelperRtoT(uint64_t);
uint64_t HelperTtoR(uint64_t);


#endif

/************************************************************************
 Title:    Driver for stepper motor controller based on DRV8825 by Texas Instruments
 Author:   Bilyana Borisova <bibishte@gmail.com>
 File:     $Id: drv_8825.c 2015/04/19
 Software: AVR-GCC 4.9.2, AVR Libc 1.8.0
 Hardware: DRV8825 & ATmega8
 License:  GNU General Public License v2
 Usage:    see Doxygen manual

 DESCRIPTION:
 This modul contains basic functions for controlling stepper motor with the Texas Instruments DRV8825

 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License v2 as published by
 the Free Software Foundation; either version 2 of the License, or
 any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License v2 for more details.

 ************************************************************************/



#include "drv_8825.h"
#include "config.h"
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "utils.h"
#include "messages.h"
#include <avr/pgmspace.h>
#include <avr/eeprom.h>

#define PERTMR 723				/**<Defines the Timer*/
#define TIMERCONST 82987552	/**<Calculate the Timer Constant*/

/** @struct Motor_Parameters
 *  @brief This structure defines types for motor parameters
 *  This type is used for declaration of three structures that are for the
 *  motor paramenters used for timmer, the real parameters of the motor and the
 *  working parameters that have been already calculated according to the microstep mode.
 *  @var Motor_Parameters::MaxSpeed
 *  Member 'MaxSpeed' defines the maximum allowed speed
 *  @var Motor_Parameters::MinSpeed
 *  Member 'MinSpeed' defines the minimum allowed speed
 *  @var Motor_Parameters::Acceleration
 *  Member 'Acceleration' defines the step of acceleration
 */
typedef struct Motor_Parameters
{
	uint16_t MaxSpeed;
	uint16_t MinSpeed;
	uint16_t Acceleration;
} Motor_Parameters;


/** @struct Motion
 *  @brief This structure defines the current step execution status.
 *  This structure is used for the timmer interrupting and in the comments
 *  for initializing the spin.
 *  @var Motion::StepsAccel
 *  Member 'StepsAccel' defines the remaining steps for acceleration process
 *  @var Motion::StepsConstSpeed
 *  Member 'StepsConstSpeed' defines remaining steps for constant speed
 *  @var Motion::StepsDeaccel
 *  Member 'StepsDeaccel' defines the remaining steps for deacceleration
 *  @var Motion::CurrentSpeed
 *  Member 'CurrentSpeed' defines current speed saved in the timmer
 */
volatile static struct Motion
{
	uint16_t StepsAccel;
	uint64_t StepsConstSpeed;
	uint16_t StepsDeaccel;
	uint16_t CurrentSpeed;
} Motion;


/**
 * This is used for calculating the timer values for full step mode.
 */
volatile static Motor_Parameters TimeParam;
/**
 * This is used for the timmer and it is calculated based on the mode.
 */
volatile static Motor_Parameters WorkingParam;
/**
 * This is used for calculating the timer values. This is the real speed of the motor.
 */
volatile static Motor_Parameters RealSpeed;
/**
 * Flag used for constant speed mode.
 */
volatile static uint8_t ConstSpd;

/**
 * Store the current decay mode.
 */
volatile static decay_type CurrentDecay;
/**
 * Store the current step mode.
 */
volatile static mode_type CurrentMode;
/**
 * Flog for enable and disable stepper motor
 */
volatile static uint8_t ModeStart;
/**
 * Flag for stepper direction.
 */
volatile static step_type StepDir;

/**
 * Motor parameter - Steps per revolution.
 */
volatile static uint16_t StepsPerRev;


/**
       * Function that sets the maximum speed.
       * @see TimeParam.MaxSpeed
       * @param spd the first argument.
		 * @return void 
       */
void SetMaxSpeed( uint16_t spd )
{
	TimeParam.MaxSpeed = spd;
	TimeToReal( );
}


/**
       * Function that sets the minimum speed.
       * @see TimeParam.MinSpeed
       * @param spd the first argument.
		 * @return void 
       */
void SetMinSpeed( uint16_t spd )
{
	TimeParam.MinSpeed = spd;
	TimeToReal( );
}

/**
       * Function that sets the acceleration.
       * @see TimeParam.Acceleration
       * @param accel the first argument.
		 * @return void 
       */
void SetAcceleration( uint16_t accel )
{
	TimeParam.Acceleration = accel;
	TimeToReal( );
}

/**
       * Function that enables the stepper motor.
		 * @return void 
       */
void Enable_Stepper( void )
{
	clr_bit( PENABLE_STEPPER, ENABLE_STEPPER );
	ModeStart = 1;
}

/**
       * Function that disables the stepper motor.
		 * @return void 
       */
void Disabled_Stepper( void )
{
	set_bit( PENABLE_STEPPER, ENABLE_STEPPER );
	ModeStart = 0;
}

/**
       * Function that sets the decay type.
       * @see decay_type
       * @param decay the first argument.
		 * @return void 
       */
void Decay( decay_type decay )
{
	CurrentDecay = decay;
	store( );
	if ( decay == FAST_DECAY )
	{
		set_as_output( DDECAY, DECAY );
		set_bit( PDECAY, DECAY );
		return;
	}

	if ( decay == SLOW_DECAY )
	{
		set_as_output( DDECAY, DECAY );
		clr_bit( PDECAY, DECAY );
		return;
	}

	set_as_input( DDECAY, DECAY );
	clr_bit( PDECAY, DECAY );
}



/**
       * Function that sets the mode.
       * @see mode_type
       * @param mode the first argument.
		 * @return void 
       */
void Mode( mode_type mode )
{
	CurrentMode = mode;
	CalWorkParam( );
	store( );

	if ( mode == MODE_FULL_STEP )
	{
		clr_bit( PMODE0, MODE0 );
		clr_bit( PMODE1, MODE1 );
		clr_bit( PMODE2, MODE2 );
	}

	if ( mode == MODE_HALF_STEP )
	{
		set_bit( PMODE0, MODE0 );
		clr_bit( PMODE1, MODE1 );
		clr_bit( PMODE2, MODE2 );
	}

	if ( mode == MODE_QUATER_STEP )
	{
		clr_bit( PMODE0, MODE0 );
		set_bit( PMODE1, MODE1 );
		clr_bit( PMODE2, MODE2 );
	}

	if ( mode == MODE_8_MICROSTEP )
	{
		set_bit( PMODE0, MODE0 );
		set_bit( PMODE1, MODE1 );
		clr_bit( PMODE2, MODE2 );
	}

	if ( mode == MODE_16_MICROSTEP )
	{
		clr_bit( PMODE0, MODE0 );
		clr_bit( PMODE1, MODE1 );
		set_bit( PMODE2, MODE2 );
	}

	if ( mode == MODE_32_MICROSTEP )
	{
		set_bit( PMODE0, MODE0 );
		clr_bit( PMODE1, MODE1 );
		set_bit( PMODE2, MODE2 );
	}

}


/**
       * Performs single step.
       * @see step_type
       * @param step the first argument.
		 * @return void 
       */
void Step( step_type step )
{
	if ( step == STEP_CLOCKWISE )
	{
		set_bit( PDIRECTION, DIRECTION );

	} else
	{
		clr_bit( PDIRECTION, DIRECTION );
	}
	set_bit( PSTEP, STEP );
	_delay_us( 2 );
	clr_bit( PSTEP, STEP );
	_delay_us( 2 );
}


/**
       * Initializes the presented steps and calculates the steps for acceleration,
		 * deacceleration and the steps for constant speed
       * @see step_type
       * @param step the first argument.
		 * @param step_count the second argument.
		 * @return void 
       */
void Count_Step( step_type step, uint64_t step_count )
{

	uint16_t y;
	StepDir = step;


	y = ( WorkingParam.MinSpeed - WorkingParam.MaxSpeed ) / WorkingParam.Acceleration;
	y = y << 1;
	if ( y < step_count )
	{
		Motion.StepsAccel = y >> 1;
		Motion.StepsDeaccel = Motion.StepsAccel;
		Motion.StepsConstSpeed = step_count - ( Motion.StepsAccel + Motion.StepsDeaccel );

	} else
	{
		Motion.StepsConstSpeed = 0;
		Motion.StepsAccel = step_count >> 1;
		Motion.StepsDeaccel = Motion.StepsAccel;
		if ( ( step_count & 1 ) )
		{
			Motion.StepsAccel++;
		}

	}
	clr_bit( TCCR1B, CS10 );
	set_bit( TCCR1B, CS11 );
	clr_bit( TCCR1B, CS12 );
	TCNT1 = 65535 - WorkingParam.MinSpeed;
	Motion.CurrentSpeed = WorkingParam.MinSpeed;
	set_bit( TIMSK, TOIE1 );

}


/**
       * Movement with constant speed and constant rPM
       * @see step_type
       * @param step the first argument.
		 * @return void 
       */
void Way_Speed( step_type step )
{
	ConstSpd = 1;
	Count_Step( step, 60000 );
}

/**
       * Stops the movement with deacceleration at the end.
		 * @return void 
       */
void Stop_Motion_normal( void )
{
	ConstSpd = 0;
	Motion.StepsAccel = 0;
	Motion.StepsConstSpeed = 0;
}


/**
       * Stops the movement without deacceleration at the end.
		 * @return void 
       */
void Stop_Motion_fast( void )
{
	ConstSpd = 0;
	Motion.StepsAccel = 0;
	Motion.StepsConstSpeed = 0;
	Motion.StepsDeaccel = 0;
}


/**
       * The home of the motor.
		 * @todo it doesnt work as espected 
       * @see step_type
		 * @return uint8_t 
       */
uint8_t Home( void )
{
	int16_t i;

	for ( i = 0; i < 128; i++ )
	{
		Step(STEP_COUNTER_CLOCKWISE);
		if ( !(check_pin( IHOME, HOME )) )
			return true;
	}
	return false;
}


/**
       * Reads the setted decay. 
       * @see decay_type
		 * @return decay_type 
       */
decay_type Get_Decay( void )
{
	return CurrentDecay;
}

/**
       * Reads the setted mode. 
       * @see mode_type
		 * @return mode_type 
       */
mode_type Get_Mode( void )
{
	return CurrentMode;
}

/**
       * Returns the mode. 
		 * @return uint8_t
       */
uint8_t Get_Start( void )
{
	return ModeStart;
}

/**
       * Reads the setted maximum speed.. 
       * @see TimeParam.MaxSpeed
		 * @return uint16_t 
       */
uint16_t GetMaxSpeed( void )
{
	return TimeParam.MaxSpeed;
}


/**
       * Reads the setted minimum speed. 
       * @see TimeParam.MinSpeed
		 * @return uint16_t 
       */
uint16_t GetMinSpeed( void )
{
	return TimeParam.MinSpeed;
}

/**
       * Reads the setted acceleration. 
       * @see TimeParam.Acceleration
		 * @return uint16_t 
       */
uint16_t GetAcceleration( void )
{
	return TimeParam.Acceleration;
}


/**
       * Reads the current speed. 
       * @see TimeParam.CurrentSpeed
		 * @return uint16_t 
       */
uint16_t GetCurrentSpeed( void )
{
	return Motion.CurrentSpeed;
}


/**
       * Sets the current speed. 
       * @see Motion.CurrentSpeed
       * @param spd the first argument.
		 * @return void 
       */
void SetCurrentSpeed( uint16_t spd )
{
	Motion.CurrentSpeed = spd;
}


/**
       * Interrupt sours routine.Makes a step and then loads the next value of the timmer  
       */
ISR( TIMER1_OVF_vect )
{
	if ( ConstSpd == 1 )
	{
		Motion.StepsConstSpeed++;
	}
	if ( Motion.StepsAccel > 0 )
	{
		if ( Motion.CurrentSpeed > WorkingParam.Acceleration )
		{
			Motion.CurrentSpeed = Motion.CurrentSpeed - WorkingParam.Acceleration;
		} else
		{
			Motion.CurrentSpeed = WorkingParam.MaxSpeed;
		}

		if ( Motion.CurrentSpeed < WorkingParam.MaxSpeed )
		{
			Motion.CurrentSpeed = WorkingParam.MaxSpeed;
		}
		TCNT1 = 0 - Motion.CurrentSpeed;
		Step( StepDir );
		Motion.StepsAccel--;

		return;
	}

	if ( Motion.StepsConstSpeed > 0 )
	{
		Motion.StepsConstSpeed--;
		TCNT1 = 0 - WorkingParam.MaxSpeed;
		Step( StepDir );

		return;
	}


	if ( Motion.StepsDeaccel > 0 )
	{
		Motion.StepsDeaccel--;
		Motion.CurrentSpeed = Motion.CurrentSpeed + WorkingParam.Acceleration;
		TCNT1 = 0 - Motion.CurrentSpeed;
		Step( StepDir );

		return;
	}

	clr_bit( TIMSK, TOIE1 );
	TCNT1 = 65535 - WorkingParam.MinSpeed;
}


/**
       * Sets the steps per revolution.
		 * @see  StepsPerRev
       * @param step_rev the first argument.
		 * @return void 
       */
void Set_Steps_revol( uint16_t step_rev )
{
	StepsPerRev = step_rev;
}

/**
       * Gets the steps per revolution.
		 * @see  StepsPerRev  
		 * @return uint16_t 
       */
uint16_t Get_Steps_revol( void )
{
	return StepsPerRev;
}


/**
       * Sets the real maximum speed.
		 * @see  RealSpeed.MaxSpeed  
       * @param maxspd the first argument.
		 * @return void 
       */
void SetRealMaxSpeed( uint16_t maxspd )
{
	RealSpeed.MaxSpeed = maxspd;
	RealToTime( );
}

/**
       * Sets the real minimum speed.
		 * @see RealSpeed.MinSpeed 
       * @param minspd the first argument.
		 * @return void 
       */
void SetRealMinSpeed( uint16_t minspd )
{
	RealSpeed.MinSpeed = minspd;
	RealToTime( );
}

/**
       * Sets the real acceleration.
		 * @see RealSpeed.Acceleration 
       * @param realacc the first argument.
		 * @return void 
       */
void SetRealAcc( uint16_t realacc )
{
	RealSpeed.Acceleration = realacc;
	RealToTime( );
}

/**
       * Gets the real maximum speed.
		 * @see RealSpeed.MaxSpeed 
		 * @return uint16_t 
       */
uint16_t GetRealMaxSpeed( void )
{
	return RealSpeed.MaxSpeed;
}


/**
       * Gets the real minimum speed.
		 * @see RealSpeed.MinSpeed 
		 * @return uint16_t 
       */
uint16_t GetRealMinSpeed( void )
{
	return RealSpeed.MinSpeed;
}

/**
       * Gets the real acceleration.
		 * @see RealSpeed.Acceleration 
		 * @return uint16_t 
       */
uint16_t GetRealAcc( void )
{
	return RealSpeed.Acceleration;
}

/**
       * Calculates the real speed to the timmer speed.
		 * @return void 
       */
void RealToTime( void )
{
	
	uint16_t revpermin;
	TimeParam.MaxSpeed=HelperRtoT(RealSpeed.MaxSpeed);
	TimeParam.MinSpeed=HelperRtoT(RealSpeed.MinSpeed);
	
	revpermin = ( TimeParam.MinSpeed - TimeParam.MaxSpeed ) / RealSpeed.Acceleration;

	//putstring_P( PSTR( "acc=" ) );
	//putstring( int_to_string( ( uint64_t ) ( revpermin ) ) );
	//putstring_P( PSTR( "\r" ) );

	if ( revpermin < 1 )
	{
		TimeParam.Acceleration = 1;
		//	putstring_P( PSTR( "Acc  low\r" ) );
	} else
	{
		TimeParam.Acceleration = revpermin;
	}

	CalWorkParam( );
	store( );
}

/**
       * Calculates the timmer speed to the real speed.
		 * @return void 
       */
void TimeToReal( void )
{
	uint16_t r;
	RealSpeed.MaxSpeed=HelperTtoR(TimeParam.MaxSpeed);
	RealSpeed.MinSpeed=HelperTtoR(TimeParam.MinSpeed);

/*
	

	//const uint64_t l=1000000000*60;

	r = TimeParam.MaxSpeed;

	r = r * PERTMR;
	r = r*StepsPerRev;
	r = 10000000000 / r;
	r = r * 6;




	if ( r > 1 )
	{
		RealSpeed.MaxSpeed = r;
	} else
	{
		RealSpeed.MaxSpeed = 1;
		//putstring_P( PSTR( "Target MaxSpeed\r" ) );
	}


	r = TimeParam.MinSpeed;
	r = r * PERTMR;
	r = r*StepsPerRev;
	r = 60000000000 / r;
	//r=r*6;


	if ( r > 1 )
	{
		RealSpeed.MinSpeed = r;
	} else
	{
		RealSpeed.MinSpeed = 1;
		//putstring_P( PSTR( "Target Minspeed\r" ) );
	}

*/

	r = ( TimeParam.MinSpeed - TimeParam.MaxSpeed ) / TimeParam.Acceleration;

	if ( r < 1 )
	{
		RealSpeed.Acceleration = 1;
		//putstring_P( PSTR( "Acc low\r" ) );
	} else
	{
		RealSpeed.Acceleration = r;
	}

	CalWorkParam( );
	store( );
}


/**
       * Calculates the maximum, minimum and the acceleration for 
		 * the different step modes.

		 * @return void 
       */
void CalWorkParam( void )
{
	switch ( CurrentMode )
	{
		case MODE_FULL_STEP:
			WorkingParam.MaxSpeed = TimeParam.MaxSpeed;
			WorkingParam.MinSpeed = TimeParam.MinSpeed;
			WorkingParam.Acceleration = TimeParam.Acceleration;
			break;

		case MODE_HALF_STEP:
			WorkingParam.MaxSpeed = TimeParam.MaxSpeed >> 1;
			WorkingParam.MinSpeed = TimeParam.MinSpeed >> 1;
			WorkingParam.Acceleration = TimeParam.Acceleration >> 1;
			break;


		case MODE_QUATER_STEP:
			WorkingParam.MaxSpeed = TimeParam.MaxSpeed >> 2;
			WorkingParam.MinSpeed = TimeParam.MinSpeed >> 2;
			WorkingParam.Acceleration = TimeParam.Acceleration >> 2;
			break;

		case MODE_8_MICROSTEP:
			WorkingParam.MaxSpeed = TimeParam.MaxSpeed >> 3;
			WorkingParam.MinSpeed = TimeParam.MinSpeed >> 3;
			WorkingParam.Acceleration = TimeParam.Acceleration >> 3;
			break;


		case MODE_16_MICROSTEP:
			WorkingParam.MaxSpeed = TimeParam.MaxSpeed >> 4;
			WorkingParam.MinSpeed = TimeParam.MinSpeed >> 4;
			WorkingParam.Acceleration = TimeParam.Acceleration >> 4;
			break;


		case MODE_32_MICROSTEP:
			WorkingParam.MaxSpeed = TimeParam.MaxSpeed >> 5;
			WorkingParam.MinSpeed = TimeParam.MinSpeed >> 5;
			WorkingParam.Acceleration = TimeParam.Acceleration >> 5;
			break;

		default:
			Errormssg( );

			break;
	}
}


/**
       * Initialize the DRV232.
		 * @return void 
       */
void InitStepper( void )
{
	RealSpeed.MaxSpeed = eeprom_read_word( ( uint16_t * ) ( 0 ) );
	RealSpeed.MinSpeed = eeprom_read_word( ( uint16_t * ) ( 2 ) );
	RealSpeed.Acceleration = eeprom_read_word( ( uint16_t * ) ( 4 ) );
	StepsPerRev = eeprom_read_word( ( uint16_t * ) ( 6 ) );
	CurrentMode = eeprom_read_byte( ( uint8_t * ) ( 8 ) );
	CurrentDecay = eeprom_read_byte( ( uint8_t * ) ( 9 ) );
	if ( ( RealSpeed.MaxSpeed == 0 ) || ( RealSpeed.MaxSpeed == 0xffff ) )
	{
		RealSpeed.MaxSpeed = 300;
		Decay( AUTO_DECAY );
		Mode( MODE_FULL_STEP );
	}

	if ( ( RealSpeed.MinSpeed == 0 ) || ( RealSpeed.MinSpeed == 0xffff ) )
	{
		RealSpeed.MinSpeed = 10;
	}

	if ( ( RealSpeed.Acceleration == 0 ) || ( RealSpeed.Acceleration == 0xffff ) )
	{
		RealSpeed.Acceleration = 100;
	}

	if ( ( StepsPerRev == 0 ) || ( StepsPerRev == 0xffff ) )
	{
		StepsPerRev = 400;
	}

	Disabled_Stepper( );
	Decay( CurrentDecay );
	Mode( CurrentMode );
	RealToTime( );
	CalWorkParam( );
}


/**
       * Calculates the real maximum speed,real minimum speed, real acceleration, steps per revolution, current mode and the current decay.
		 * @return void 
       */
void store( void )
{
	eeprom_write_word( ( uint16_t * ) ( 0 ), RealSpeed.MaxSpeed );
	eeprom_write_word( ( uint16_t * ) ( 2 ), RealSpeed.MinSpeed );
	eeprom_write_word( ( uint16_t * ) ( 4 ), RealSpeed.Acceleration );
	eeprom_write_word( ( uint16_t * ) ( 6 ), StepsPerRev );
	eeprom_write_byte( ( uint8_t * ) ( 8 ), CurrentMode );
	eeprom_write_byte( ( uint8_t * ) ( 9 ), CurrentDecay );


}

/**
       * Helper of the function RealToTime
		 * @see RealToTime
       * @param in the first argument.
		 * @return uint64_t 
       */
uint64_t HelperRtoT(uint64_t in)
{
	uint64_t revpermin;

/*
	revpermin = in;
	revpermin = revpermin*StepsPerRev;
	revpermin = revpermin / 60;
	revpermin = 1000000000 / revpermin;
	revpermin = revpermin / PERTMR;
*/
revpermin=TIMERCONST/(in*StepsPerRev);
	if ( revpermin < 65500 )
	{	
		return revpermin;
	}
	else
	{
		return  65500;
	}
}


/**
       * Helper of the function TimeToReal
		 * @see TimeToReal
       * @param in the first argument.
		 * @return uint64_t 
       */
uint64_t HelperTtoR(uint64_t in)
{
	uint64_t r;

	//const uint64_t l=1000000000*60;
	
	r=TIMERCONST/(in*StepsPerRev);
	
/*
	r = in;

	
	
	r = r * PERTMR;
	r = r*StepsPerRev;
	r = 10000000000 / r;
	r = r * 6;

*/



	if ( r > 1 )
	{
		return r;
	} else
	{
		return 1;
		//putstring_P( PSTR( "Target MaxSpeed\r" ) );
	}

}






/*
 * interpreter.c
 *
 *  Created on: 28.07.2010
 *      Author: ovp  Ognian Petrov
 */

//TODO Konstante Strings ins Flasch.
//http://www.rn-wissen.de/index.php/Avr-gcc#Optimierungen.2C_Tipps_.26_Tricks
#include <string.h>
#include "interpreter.h"

struct cmd_struct
{
	signed char id;
	char const *name;
} cmd_tbl[] =
{
{ VERSION, "ver?" },
{ HELP, "hlp?" },
{ TEST, "tst?" },
{ SET_DEC_SLOW, "sdec" },
{ SET_DEC_FAST, "fdec" },
{ SET_DEC_AUTO, "adec" },
{ SET_MODE_FULL, "fullm" },
{ SET_MODE_HALF, "1/2m" },
{ SET_MODE_QUATER, "1/4m" },
{ SET_MODE_8, "8m" },
{ SET_MODE_16, "16m" },
{ SET_MODE_32, "32m" },
{ SET_HOME, "home" },
{ SET_ACC, "acc=" },
{ SET_MIN_SPEED, "minspd=" },
{ SET_MAX_SPEED, "maxspd=" },
{ SET_STP_REV, "stp_rev="},
{ SET_R_MIN_SPD, "rminspd=" },
{ SET_R_MAX_SPD, "rmaxspd=" },
{ SET_R_ACC, "racc=" },
{ GET_ACC, "acc?" },
{ GET_MIN_SPEED, "minspd?" },
{ GET_MAX_SPEED, "maxspd?" },
{ GET_STP_REV, "stp_rev?"},
{ GET_R_MIN_SPD, "rminspd?" },
{ GET_R_MAX_SPD, "rmaxspd?" },
{ GET_R_ACC, "racc?" },
{ ENABLE, "enbl" },
{ DISABLE, "disbl" },
{ GOTO, "g=" },
{ STATUS, "stat?" },
{ STOPN, "stopn"},
{ STOPF, "stopf"},
{ CONSTSPD, "conspd="},
{ -1, NULL } };

/**
 *
 * @param name
 * @return
 */
char get_cmd_id(char *name)
{
	unsigned char i = 0;

	if (name[0] == 0)
		return -2;

	for (i = 0; cmd_tbl[i].id != -1; i++)
		if (!strncmp(name, cmd_tbl[i].name,strlen(cmd_tbl[i].name)))
			return cmd_tbl[i].id;

	return -1;
}

/**
 *
 * @param id
 * @return command name or NULL if not found
 */
char const * get_cmd_name(char id)
{
	unsigned char i;

	for (i = 0; cmd_tbl[i].id != -1; i++)
		if (id == cmd_tbl[i].id)
			return cmd_tbl[i].name;

	return NULL;
}



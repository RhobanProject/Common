/*************************************************
 * Publicly released by Rhoban System, August 2012
 * www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/

#ifndef MSG_PROT_H
#define MSG_PROT_H

#define RHOBAN_PROTOCOL_VERSION "2"

#define RHOBAN_MESSAGE_DESTINATIONS_NB 8
static const char * RHOBAN_MESSAGE_DESTINATIONS[RHOBAN_MESSAGE_DESTINATIONS_NB] =
{
		"Error",\
		"Server",\
		"System",\
		"LowLevel",\
		"Scheduler",\
		"Vision",\
		"Localisation",\
		"StateMachine"
};

#define MSG_TYPE_ERROR 0

// This is the error command available for all destinations
#define MSG_ERROR_COMMAND 1

#define MSG_TYPE_SERVER 1
#define MSG_SERVER_GET_VERSION 0
#define MSG_SERVER_ECHO 2
#define MSG_SERVER_XML_TO_FILE 3
#define MSG_SERVER_CONNECT_TO_LOW_LEVEL 4
#define MSG_SERVER_REGISTER_COMPONENT 5

#define MSG_TYPE_SYSTEM 2
#define MSG_SYSTEM_BASH 4
#define MSG_SYSTEM_STAT 5

#define MSG_TYPE_LOW_LEVEL 3

#define MSG_LL_ERROR MSG_ERROR_COMMAND
#define MSG_LL_DYNAMIXEL_ATTACH 7
#define MSG_LL_DYNAMIXEL_DETACH 8
#define MSG_LL_DYNAMIXEL_SCAN 9
#define MSG_LL_DYNAMIXEL_LIST_MOTORS 10
#define MSG_LL_DYNAMIXEL_SET_VAL_GOAL 11
#define MSG_LL_DYNAMIXEL_SET_TORQUE 12
#define MSG_LL_DYNAMIXEL_SET_SPEED 13
#define MSG_LL_DYNAMIXEL_GET_VAL_REAL 14
#define MSG_LL_DYNAMIXEL_GET_SPEED 15
#define MSG_LL_DYNAMIXEL_GET_LOAD 16
#define MSG_LL_DYNAMIXEL_SET_REGISTER 17
#define MSG_LL_DYNAMIXEL_GET_REGISTER 18
#define MSG_LL_DYNAMIXEL_SYNCHRONIZE 19
#define MSG_LL_DYNAMIXEL_GET_VALUES 20
#define MSG_LL_DYNAMIXEL_SET_VALUES 22
#define MSG_LL_DYNAMIXEL_STATUS 23
#define MSG_LL_DYNAMIXEL_INIT 24
#define MSG_LL_DYNAMIXEL_SYNC 25
#define MSG_LL_DYNAMIXEL_SET_COMPLIANT 26
#define MSG_LL_DYNAMIXEL_SET_ALL_COMPLIANT 27
#define MSG_LL_DYNAMIXEL_OUTOFTHEBOX 80
#define MSG_LL_DYNAMIXEL_CHANGE_SERVO_ID 81
#define MSG_LL_DYNAMIXEL_DIAGNOSE 82
#define MSG_LL_DYNAMIXEL_STOP_OVERLOAD_COMPLIANCE 83
#define MSG_LL_DYNAMIXEL_SET_EEPROM_REGISTER 41
#define MSG_LL_DYNAMIXEL_DETECT_OUTOFTHEBOX 84
#define MSG_LL_DYNAMIXEL_RECONFIGURE_ALL 85

#define MSG_LL_PWM_ENABLE 29
#define MSG_LL_PWM_DISABLE 30
#define MSG_LL_PWM_SET_FREQUENCY 31
#define MSG_LL_PWM_SET_CYCLIC_RATE 32

#define MSG_LL_I2C_INIT 37
#define MSG_LL_I2C_CLOSE 38
#define MSG_LL_I2C_SEND 39
#define MSG_LL_I2C_RESET 40

#define MSG_LL_GET_ANALOGIC_SENSORS 42
#define MSG_LL_GET_PWMS 43
#define MSG_LL_LOAD_CONFIG 44
#define MSG_LL_GET_SENSOR_VALUES 45
#define MSG_LL_SET_PWMS_RATES 48
#define MSG_LL_GET_ANALOGIC_NORMALIZED 49

#define MSG_LL_JOYPAD_GET_VALUES 51
#define MSG_LL_JOYPAD_SET_VALUES 52

#define MSG_LL_DYNAMIXEL_GET_ERROR 54

#define MSG_LL_KINECT_GET_VALUES 56
#define MSG_LL_KINECT_SET_VALUES 57

#define MSG_LL_DYNAMIXEL_READ_MOTORS 60

//#define MSG_LL_SAVE_CONFIG 70
//#define MSG_LL_LOAD_LOCAL_CONFIG 71
#define MSG_LL_GET_CONFIG 70
#define MSG_LL_CONFIG_IS_LOADED 71
/**************************************
 * MOVE SCHEDULER
 */

#define MSG_TYPE_MOVE_SCHEDULER 4
#define SCHEDULER_LOAD_CONFIG 40
#define SCHEDULER_ERROR MSG_ERROR_COMMAND
#define SCHEDULER_START_MOVE 42
#define SCHEDULER_STOP_MOVE 43
#define SCHEDULER_PAUSE_RESUME_MOVE 44
#define SCHEDULER_UPDATE_BLOCK 45
#define SCHEDULER_GET_LOADED_MOVES 46
#define SCHEDULER_GET_AVAILABLE_MOVES 47
#define SCHEDULER_KILL_MOVE 48
#define SCHEDULER_LOAD_XML_MOVE 49
#define SCHEDULER_STORE_XML_MOVE 51
#define SCHEDULER_GET_MOVE_INFO 50
#define SCHEDULER_LOAD_MOVE_FILE 52
#define SCHEDULER_READ_BLOCK_VALUES 54
#define SCHEDULER_READ_ALL_BLOCKS_VALUES 55
#define SCHEDULER_CROSSFADE 56
#define SCHEDULER_GET_RECORDED_SPLINE_FILE 57
#define SCHEDULER_SAVE_RECORDED_SPLINE 58
#define SCHEDULER_RECORD_SPLINE 59
#define SCHEDULER_STOP_SPLINE 60
#define SCHEDULER_PLAY_SPLINE 61
#define SCHEDULER_GET_COMPRESSED_RECORDED_SPLINE 63
#define SCHEDULER_PAUSE_RESUME_SPLINE 64
#define SCHEDULER_TEST_SPLINE 65
#define SCHEDULER_GET_SPLINE_STATE 66
#define SCHEDULER_SET_COMPRESSED_SPLINE 67
#define SCHEDULER_EMERGENCY_STOP 68
#define SERVOS_GET_CONFIG 69
#define SCHEDULER_UPDATE_CONSTANT 70
#define SCHEDULER_GET_CONFIG 71
#define SCHEDULER_CONFIG_IS_LOADED 72
#define SCHEDULER_TAKE_REF_POSITION 73

#define MSG_TYPE_VISION 5
#define VISION_ERROR MSG_ERROR_COMMAND
#define VISION_GET_PARAMETER 4
#define VISION_SET_PARAMETER 5
#define VISION_GET_IPL_FRAME 6
#define VISION_GET_JPEG_FRAME 7
#define VISION_GET_JPEG_FRAMES 17
#define VISION_SAVE_PARAMETERS 20
#define VISION_GET_PARAMETERS 18
#define VISION_SET_PARAMETERS 19
#define VISION_RESET_CAMERA 16
#define VISION_GET_BALL_INFO 21
#define VISION_GET_GOAL_INFO 22
#define VISION_SET_TASKS_FREQUENCY 23
#define VISION_START_TASK 24
#define VISION_STOP_TASK 25
#define VISION_HELLO 45

#define MSG_TYPE_LOCALISATION 6
#define LOCALISATION_ERROR MSG_ERROR_COMMAND
#define LOCALISATION_GET_STATE 2
#define LOCALISATION_ADD_SAMPLE 3
#define LOCALISATION_GET_DATABASE 4
#define LOCALISATION_GET_FIELD 5
#define LOCALISATION_SET_DATABASE 6
#define LOCALISATION_SAVE_DATABASE 7
#define LOCALISATION_CLEAR_DATABASE 8
#define LOCALISATION_AUTO_CALIBRATE 9
#define LOCALISATION_AREA_CALIBRATE 10
#define LOCALISATION_PATTERN_CALIBRATE 11

#define MSG_TYPE_STM 7
#define STM_LOAD_XML_MACHINE 2
#define STM_START_MACHINE 3
#define STM_STOP_MACHINE 4
#define STM_KILL_MACHINE 5
#define STM_GET_STATES 6
#define STM_GET_STATE 7
#define STM_GET_MACHINES_INFO 8
#define STM_SET_MACHINE_ATTRIBUTES 9
#define STM_GET_MACHINE_ATTRIBUTES 10
#define STM_EVALUATE_EXPRESSION 11

#define MSG_TYPE_MAX_NB 16

#endif // MSG_PROT_H

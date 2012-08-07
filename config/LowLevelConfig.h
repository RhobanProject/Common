/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
/*
 * low_level_config.h
 *
 *  Created on: 4 oct. 2011
 *      Author: Hugo Gimbert and Olivier Ly
 */

#ifndef LOW_LEVEL_CONFIG_H_
#define LOW_LEVEL_CONFIG_H_

#include "names.h"
#include "Config.h"
#include "AnalogicSensorConfig.h"
#include "DynamixelConfig.h"
#include "PwmConfig.h"
#include "PinsConfig.h"
#include "JoypadConfig.h"
#include "KinectConfig.h"

using namespace std;

class LowLevelConfig : public Config
{
    public:
        AnalogicSensorConfig *sensor_config;
        DynamixelConfig *dynamixel_config;
        PwmConfig *pwm_config;
        PinsConfig *pins_config;
        JoypadConfig *joypad_config;
        KinectConfig *kinect_config;

        bool initialized;

        LowLevelConfig();
		~LowLevelConfig();
		
		virtual void instantiate();

        //load configuration
        void from_xml(TiXmlNode *node);
        string to_xml() const;

        string class_name() const;

};

#endif /* LOW_LEVEL_CONFIG_H_ */

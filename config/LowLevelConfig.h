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
        AnalogicSensorConfig sensor_config;
        DynamixelConfig dynamixel_config;
        PwmConfig pwm_config;
        PinsConfig pins_config;
        JoypadConfig joypad_config;
        KinectConfig kinect_config;

        bool initialized;

        LowLevelConfig();

        //load configuration
        void from_xml(TiXmlNode *node);
        string to_xml() const;

        string class_name() const;

};

#endif /* LOW_LEVEL_CONFIG_H_ */

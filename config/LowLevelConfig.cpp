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
 * low_level_config.cpp
 *
 *  Created on: 4 oct. 2011
 *      Author: Hugo Gimbert and Olivier Ly
 */

#include "XMLTools.h"
#include "LowLevelConfig.h"

LowLevelConfig::LowLevelConfig()
{
    initialized = false;
}

void LowLevelConfig::from_xml(TiXmlNode * node)
{
    TiXmlNode * subnode = 0;

    subnode = node->FirstChild( "sensors_config" );
    if(subnode) sensor_config.from_xml(subnode);

    subnode = node->FirstChild( "dynamixel_config" );
    if(subnode) dynamixel_config.from_xml(subnode);

    subnode = node->FirstChild( "pwm_config" );
    if(subnode) pwm_config.from_xml(subnode);

    subnode = node->FirstChild( "pins_config" );
    if(subnode) pins_config.from_xml(subnode);

    subnode = node->FirstChild( "joypad_config" );
    if(subnode) joypad_config.from_xml(subnode);

    subnode = node->FirstChild( "kinect_config" );
    if(subnode) kinect_config.from_xml(subnode);

}


string LowLevelConfig::to_xml() const
{
    string result;
    result += "<sensors_config>"+ sensor_config.to_xml() + "</sensors_config>";
    result += "<dynamixel_config>"+ dynamixel_config.to_xml() + "</dynamixel_config>";
    result += "<pwm_config>"+ pwm_config.to_xml() + "</pwm_config>";
    result += "<pins_config>"+ pins_config.to_xml() + "</pins_config>";
    result += "<joypad_config>"+ joypad_config.to_xml() + "</joypad_config>";
    result += "<kinect_config>"+ kinect_config.to_xml() + "</kinect_config>";
    return result;
}


string LowLevelConfig::class_name() const
{
    return LOWLEVEL_CONFIG_FILENAME;
}

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

#include <string>
#include <xml/XMLTools.h>
#include <file_mngt.h>
#include "LowLevelConfig.h"

LowLevelConfig::LowLevelConfig()
{
  initialized = false;
  instantiate();
}

LowLevelConfig::LowLevelConfig(string configfile)
{
  initialized = false;
  instantiate();

  load_file(configfile);
}

void LowLevelConfig::instantiate()
{
  sensor_config = new AnalogicSensorConfig();
  dynamixel_config = new DynamixelConfig();
  pwm_config = new PwmConfig();
  pins_config = new PinsConfig();
  joypad_config = new JoypadConfig();
  kinect_config = new KinectConfig();
}

LowLevelConfig::~LowLevelConfig()
{
  delete sensor_config;
  delete dynamixel_config;
  delete pwm_config;
  delete pins_config;
  delete joypad_config;
  delete kinect_config;
}

void LowLevelConfig::from_xml(TiXmlNode * node)
{
  TiXmlNode * subnode = 0;

  subnode = node->FirstChild( "sensors_config" );
  if(subnode) sensor_config->from_xml(subnode);

  subnode = node->FirstChild( "dynamixel_config" );
  if(subnode) dynamixel_config->from_xml(subnode);

  subnode = node->FirstChild( "pwm_config" );
  if(subnode) pwm_config->from_xml(subnode);

  subnode = node->FirstChild( "pins_config" );
  if(subnode) pins_config->from_xml(subnode);

  subnode = node->FirstChild( "joypad_config" );
  if(subnode) joypad_config->from_xml(subnode);

  subnode = node->FirstChild( "kinect_config" );
  if(subnode) kinect_config->from_xml(subnode);

}


string LowLevelConfig::to_xml() const
{
  string result;
  result += "<sensors_config>"+ sensor_config->to_xml() + "</sensors_config>";
  result += "<dynamixel_config>"+ dynamixel_config->to_xml() + "</dynamixel_config>";
  result += "<pwm_config>"+ pwm_config->to_xml() + "</pwm_config>";
  result += "<pins_config>"+ pins_config->to_xml() + "</pins_config>";
  result += "<joypad_config>"+ joypad_config->to_xml() + "</joypad_config>";
  result += "<kinect_config>"+ kinect_config->to_xml() + "</kinect_config>";
  return result;
}


string LowLevelConfig::class_name() const
{
  return LOWLEVEL_CONFIG_FILENAME;
}

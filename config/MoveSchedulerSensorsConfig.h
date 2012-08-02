/*
 * SensorsConfig.hpp
 *
 *  Created on: 10 mars 2012
 *      Author: Hugo
 */

#ifndef SENSORSCONFIG_HPP_
#define SENSORSCONFIG_HPP_

#include "Config.h"
#include "XMLTools.h"
#include "util.h"

class MoveSchedulerSensorsConfig : public Config
{
public:
	MoveSchedulerSensorsConfig();

	//compass sensor name, no compass if void
	string compass_sensor;

	//Accelerometer sensor name, no accelerometer if void
	string accx_sensor;
	string accy_sensor;
	string accz_sensor;

	string gyrox_sensor;
	string gyroy_sensor;

	string joypad_sensor;

	string kinect_sensor;

	void from_xml(TiXmlNode *node);
	string to_xml() const;
	string class_name() const;

};

#endif /* SENSORSCONFIG_HPP_ */

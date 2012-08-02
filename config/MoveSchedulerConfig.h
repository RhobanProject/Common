/*
 * RobotConfig.h
 *
 *  Created on: 4 oct. 2011
 *      Author: hugo
 */

#ifndef ROBOTCONFIG_H_
#define ROBOTCONFIG_H_

#include "Config.h"
#include "util.h"

#include "LowLevelConfig.h"
#include "ServosConfig.h"
#include "MoveSchedulerSensorsConfig.h"

/*!
 *
 * this static class is used to store and centralize
 * the lowlevel config
 * used by the control layer,
 * and maps sensor devices to module devices
 */

class MoveSchedulerConfig : public Config
{
public:

	ServosConfig servos_config;
	MoveSchedulerSensorsConfig sensors_config;

	MoveSchedulerConfig();
	virtual ~MoveSchedulerConfig(){};

	void from_xml(TiXmlNode *node);
	string to_xml() const;

	string class_name() const;

	bool config_loaded;
};


extern MoveSchedulerConfig mpconfig;

#endif /* ROBOTCONFIG_H_ */

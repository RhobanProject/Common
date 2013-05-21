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
 * RobotConfig.h
 *
 *  Created on: 4 oct. 2011
 *      Author: hugo
 */

#ifndef ROBOTCONFIG_H_
#define ROBOTCONFIG_H_

#include <string>

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

  /* moves tha should be automatically loaded at startup */
  vector<string> autoload_moves;

  MoveSchedulerConfig();
  MoveSchedulerConfig(string config);
  virtual ~MoveSchedulerConfig(){};

  void from_xml(TiXmlNode *node);
  string to_xml() const;

  string class_name() const;

  static string MovesDirectoryName() { return "Moves"; };

  bool config_loaded;
};


extern MoveSchedulerConfig mpconfig;

#endif /* ROBOTCONFIG_H_ */

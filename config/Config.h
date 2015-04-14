/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef CONFIG_H_
#define CONFIG_H_

#include <cstdlib>
#include <iostream>
#include <string>
#include <xml/XMLTools.h>
#include <logging/log.h>

class LowLevel;

/**
 * Log level
 * 1: Caution
 * 2: Message
 * 3: Debug
 */
#define CONFIG_LOG_LEVEL 1

#define CONFIG_CAUTION(...)     LOG_CPP(1, CONFIG_LOG_LEVEL, "config:caution", __VA_ARGS__)
#define CONFIG_MSG(...)         LOG_CPP(2, CONFIG_LOG_LEVEL, "config", __VA_ARGS__)
#define CONFIG_DEBUG(...)       LOG_CPP(3, CONFIG_LOG_LEVEL, "config:debug", __VA_ARGS__)

typedef unsigned int uint;

using namespace std;

class Config : public Serializable
{
public:
  /**
   * Loads the configuration
   */
  void from_xml(TiXmlNode *node);

  /**
   * Saves the configuration
   */
  string to_xml() const;

  /**
   * Class name
   */
  virtual string class_name() const = 0;

  /**
   * Loads the configuration
   */
  virtual void load_config(LowLevel *);
};

#endif // CONFIG_H_

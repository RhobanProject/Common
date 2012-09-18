/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <string>
#include <xml/XMLTools.h>
#include "Config.h"

void Config::from_xmlfile(string filecontents)
{
  TiXmlDocument *doc = XMLTools::file_to_node(filecontents);
  from_xml(doc);
}


void Config::from_xml(TiXmlNode * node)
{
	cout << "TROLOLO" << endl;
  throw string("Config: from_xml() is not implemented");
}

string Config::to_xml() const
{
  throw string("Config: to_xml() is not implemented");
}

void Config::load_config(AdapterLowLevel *adapter)
{
  CONFIG_CAUTION("Config: load_config() is not implemented");
}

void Config::setConfig(string config)
{
  this->config = config;
}

string Config::getConfig()
{
  return config;
}

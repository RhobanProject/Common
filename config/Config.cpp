/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include "Config.h"

void Config::from_xml(TiXmlNode * node)
{
    throw string("Config: from_xml() is not implemented");
}

string Config::to_xml() const
{
    throw string("Config: to_xml() is not implemented");
}

void Config::load_config()
{
    cout << "Config: load_config() is not implemented" << endl;
	__asm__("int3");
	exit(1);
}

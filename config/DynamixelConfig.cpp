/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#include <sstream>

#include "DynamixelConfig.h"

void DynamixelConfig::from_xml(TiXmlNode * node)
{
    device_name = XMLTools::get_string_element(node,"device_name");
    frequency = XMLTools::get_int_element(node,"frequency");
    write_frequency = XMLTools::get_int_element(node,"write_frequency");
    usart_id = XMLTools::get_int_element(node,"usart_id");
}


string DynamixelConfig::to_xml() const
{
    ostringstream result;
    result << "<device_name>" << device_name << "</device_name>";
    result << "<frequency>" << frequency << "</frequency>";
    result << "<write_frequency>" << write_frequency << "</write_frequency>";
    result << "<usart_id>" << usart_id << "</usart_id>";
    return result.str();
}

string DynamixelConfig::class_name() const
{
    return DYNAMIXEL_CONFIG_FILENAME;
}

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

#include "PwmConfig.h"

string PwmConfig::to_xml() const
{
    string result;
    for(uint i =0; i< pwms.size(); i++)
    {
        const PwmDeviceParameter pdp = pwms[i];
        result += "<" + pdp.class_name()+ ">" + pdp.to_xml() + "</" + pdp.class_name()+ ">";
    }
    return result;
}

void PwmConfig::from_xml(TiXmlNode * node)
{
    pwms.clear();
    TiXmlNode * pwms_node = node->FirstChild( "pwms" );
    if(pwms_node)
        for ( TiXmlNode* child = pwms_node->FirstChild(); child != 0; child = child->NextSibling())
        {
            pwms.push_back(create_parameter(child));
        }
}

PwmDeviceParameter PwmConfig::create_parameter(TiXmlNode *node)
{ 
    PwmDeviceParameter pdp;
    pdp.from_xml(node);
    return pdp;
}

string PwmConfig::class_name() const
{
    return PWM_CONFIG_FILENAME;
}

void PwmDeviceParameter::from_xml(TiXmlNode * node)
{
	frequency = (uint) XMLTools::get_int_element(node,"frequency");
	pin = XMLTools::get_string_element(node,"pin");
	name = XMLTools::get_string_element(node,"name");
	cyclic_rate = XMLTools::get_double_element(node,"cyclic_rate");
	max = XMLTools::get_double_element(node,"max");
	min = XMLTools::get_double_element(node,"min");
}

string PwmDeviceParameter::to_xml() const
{
	ostringstream result;
	result << "<frequency>" << frequency << "</frequency>";
	result << "<pin>" << pin << "</pin>";
	result << "<name>" << name << "</name>";
	result << "<cyclic_rate>" << cyclic_rate << "</cyclic_rate>";
	result << "<max>" << max << "</max>";
	result << "<min>" << min << "</min>";
	return result.str();
}

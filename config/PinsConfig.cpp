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

#include "PinsConfig.h"

string PinsConfig::class_name() const
{
    return PINS_CONFIG_FILENAME;
}

string PinsConfig::to_xml() const
{
    string result;
    result += "<pins>";
    for(uint i =0; i< pins.size(); i++)
    {
        const PinDeviceParameter *pdp = pins[i];
        result += "<" + pdp->class_name()+ ">" + pdp->to_xml() + "</" + pdp->class_name()+ ">";
    }
    result += "</pins>";
    return result;
}

void PinsConfig::from_xml(TiXmlNode * node)
{
    pins.clear();
    TiXmlNode * pins_node = node->FirstChild( "pins" );
    if(pins_node)
        for ( TiXmlNode* child = pins_node->FirstChild(); child != 0; child = child->NextSibling())
        {
            pins.push_back(create_parameter(child));
        }
}

PinDeviceParameter *PinsConfig::create_parameter(TiXmlNode *node)
{
    PinDeviceParameter *pdp = new PinDeviceParameter();
    pdp->from_xml(node);
    return pdp;
}

PinDeviceParameter * PinsConfig::find_pindevice_parameter(string pin_name)
{
    for(uint i = 0; i< pins.size(); i++)
    {
        PinDeviceParameter * asp = pins[i];
        if(asp->name == pin_name)
            return asp;
    }
    return NULL;
}

void PinDeviceParameter::from_xml(TiXmlNode * node)
{
    frequency = (uint) XMLTools::get_int_element(node,"frequency");
    pin = XMLTools::get_string_element(node,"pin");
    name = XMLTools::get_string_element(node,"name");
    type = (XMLTools::get_int_element(node,"pin_type") == 0) ? reader : writer;
    on = XMLTools::get_bool_element(node,"On");
}

string PinDeviceParameter::to_xml() const
{
    ostringstream result;
    result << "<frequency>" << frequency << "</frequency>";
    result << "<pin>" << pin << "</pin>";
    result << "<name>" << name << "</name>";
    result << "<pin_type>" << ((type == reader) ? 0 : 1) << "</pin_type>";
    result << "<On>" << (on ? "true" : "false") << "</On>";
    return result.str();
}

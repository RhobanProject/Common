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
    result += "<pwms>";
    for(uint i =0; i< pwms.size(); i++)
    {
        const PwmDeviceParameter *pdp = pwms[i];
        result += "<" + pdp->class_name()+ ">" + pdp->to_xml() + "</" + pdp->class_name()+ ">";
    }
    result += "</pwms>";
    return result;
}

void PwmConfig::from_xml(TiXmlNode * node)
{
    pwms.clear();
    TiXmlNode * pwms_node = node->FirstChild( "pwms" );
    if(pwms_node) {
        for ( TiXmlNode* child = pwms_node->FirstChild(); child != 0; child = child->NextSibling())
        {
            pwms.push_back(create_parameter(child));
        }
    }
}

PwmDeviceParameter *PwmConfig::create_parameter(TiXmlNode *node)
{ 
    PwmDeviceParameter *pdp = new PwmDeviceParameter();
    pdp->from_xml(node);
    return pdp;
}

string PwmConfig::class_name() const
{
    return PWM_CONFIG_FILENAME;
}

void PwmDeviceParameter::from_xml(TiXmlNode * node)
{
    name = XMLTools::get_string_element(node, "name");
    pin = XMLTools::get_string_element(node, "pin");
    stepsPerDegree = XMLTools::get_float_element(node, "stepsPerDegree");
}

string PwmDeviceParameter::to_xml() const
{
    ostringstream result;
    result << "<name>" << name << "</name>";
    result << "<pin>" << pin << "</pin>";
    result << "<stepsPerDegree>" << stepsPerDegree << "</stepsPerDegree>";
    result << "<cyclicRate>" << cyclicRate << "</cyclicRate>";
    return result.str();
}

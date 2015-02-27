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
#include <cmath>

#include "AnalogicSensorConfig.h"

void AnalogicSensorConfig::from_xml(TiXmlNode * node)
{
    sensors.clear();
    TiXmlNode * sensors_node = node->FirstChild( "sensors" );
    if(sensors_node)
        for ( TiXmlNode* child = sensors_node->FirstChild(); child != 0; child = child->NextSibling())
        {
            sensors.push_back(create_parameter(child));
        }
}
        
AnalogicSensorParameter *AnalogicSensorConfig::create_parameter(TiXmlNode *node)
{
    AnalogicSensorParameter *asp = new AnalogicSensorParameter();
    asp->from_xml(node);
    return asp;
}

string AnalogicSensorConfig::to_xml() const
{
    string result;
    result += "<sensors>";

    for (uint i = 0; i <sensors.size(); i++)
    {
        const AnalogicSensorParameter * asp = sensors[i];
        result += "<" + asp->class_name()+ ">" + asp->to_xml() + "</" + asp->class_name()+ ">";
    }

    result += "</sensors>";
    return result;
}

string AnalogicSensorConfig::class_name() const
{
    return ANALOGIC_SENSOR_CONFIG_FILENAME;
}

AnalogicSensorParameter * AnalogicSensorConfig::find_sensor_parameter(string sensor_name)
{
    for(uint i = 0; i< sensors.size(); i++)
    {
        AnalogicSensorParameter * asp = sensors[i];
        if(asp->name == sensor_name)
            return asp;
    }
    return NULL;
}

AnalogicSensorParameter::AnalogicSensorParameter():frequency(1), can(0), min(100000), max(0), zero(0), norm_coef(1)
{
    name="";
}

void AnalogicSensorParameter::from_xml(TiXmlNode * node)
{
    frequency = (uint) XMLTools::get_int_element(node,"frequency");
    can = XMLTools::get_int_element(node,"can");
    name = XMLTools::get_string_element(node,"name");
    min = XMLTools::get_double_element(node,"min");
    max = XMLTools::get_double_element(node,"max");
    zero = XMLTools::get_double_element(node,"zero");
    norm_coef = 1.0 / ((int) max - (int) min);
}

string AnalogicSensorParameter::to_xml() const
{
    ostringstream result;
    result << "<frequency>" << frequency << "</frequency>";
    result << "<can>" << can << "</can>";
    result << "<name>" << name << "</name>";
    result << "<min>" << min << "</min>";
    result << "<max>" << max << "</max>";
    result << "<zero>" << zero << "</zero>";
    return result.str();
}

double AnalogicSensorParameter::normalize(double val)
{
    if (fabs(max)<0.01 && fabs(min)<0.01) {
        return val;
    } else {
        return norm_coef * ((int) val - zero ) ;
    }
}

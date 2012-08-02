/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
/**
 * MoveSchedulerConfig.cpp
 *
 *  Created on: 4 oct. 2011
 *      Author: hugo
 */
#include "MoveSchedulerConfig.h"
#include <iostream>
#include <sstream>

MoveSchedulerConfig mpconfig;

/*!
 *
 * Sensor parameters
 */
MoveSchedulerConfig::MoveSchedulerConfig()
{
    config_loaded = false;
}


string MoveSchedulerConfig::class_name() const
{
    return "MoveSchedulerConfig";
}


void MoveSchedulerConfig::from_xml(TiXmlNode * node)
{
    TiXmlNode * sub_node = node->FirstChild("ServosConfig");

    if(!sub_node)
        throw string("No ServosConfig node in moveschedulerconfig stream");
    servos_config.from_xml(sub_node);

    sub_node = node->FirstChild("SensorsConfig");

    if(!sub_node)
        throw string("No SensorsConfig node in moveschedulerconfig stream");
    sensors_config.from_xml(sub_node);

    sub_node = node->FirstChild("PinsConfig");

    config_loaded = true;


}

string MoveSchedulerConfig::to_xml() const
{
    stringstream result;
    result << "<ServosConfig>" << servos_config.to_xml() << "</ServosConfig>";
    result << "<SensorsConfig>" << sensors_config.to_xml() << "</SensorsConfig>";
    return result.str();
}

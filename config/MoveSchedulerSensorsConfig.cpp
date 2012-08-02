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
 * SensorsConfig.cpp
 *
 *  Created on: 10 mars 2012
 *      Author: Hugo
 */
#include "MoveSchedulerSensorsConfig.h"
#include <iostream>

MoveSchedulerSensorsConfig::MoveSchedulerSensorsConfig()
{
    compass_sensor = "";
    accx_sensor = "";
    accy_sensor = "";
    accz_sensor = "";
    gyrox_sensor = "";
    gyroy_sensor = "";
    joypad_sensor ="";

}

void MoveSchedulerSensorsConfig::from_xml(TiXmlNode * node)
{
    compass_sensor = XMLTools::get_string_element(node,"compass_sensor");
    accx_sensor = XMLTools::get_string_element(node,"accx_sensor");
    accy_sensor = XMLTools::get_string_element(node,"accy_sensor");
    accz_sensor = XMLTools::get_string_element(node,"accz_sensor");
    gyrox_sensor = XMLTools::get_string_element(node,"gyrox_sensor");
    gyroy_sensor = XMLTools::get_string_element(node,"gyroy_sensor");
    joypad_sensor = XMLTools::get_string_element(node,"joypad_sensor");
    //kinect_sensor = XMLTools::get_string_element(node,"kinect_sensor");

    //string serial = to_xml();
    //cout << "Sensors Config: " << serial << endl;
}

string MoveSchedulerSensorsConfig::to_xml() const
{
    stringstream result;
    result << "<compass_sensor>" << compass_sensor << "</compass_sensor>";
    result << "<accx_sensor>" << accx_sensor << "</accx_sensor>";
    result << "<accy_sensor>" << accy_sensor << "</accy_sensor>";
    result << "<accz_sensor>" << accz_sensor << "</accz_sensor>";
    result << "<gyrox_sensor>" << gyrox_sensor << "</gyrox_sensor>";
    result << "<gyroy_sensor>" << gyroy_sensor << "</gyroy_sensor>";
    result << "<joypad_sensor>" << joypad_sensor << "</joypad_sensor>";
    //result << "<kinect_sensor>" << kinect_sensor << "</kinect_sensor>";
    return result.str();
}

string MoveSchedulerSensorsConfig::class_name() const
{
    return "SensorsConfig";
}


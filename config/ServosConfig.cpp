/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
/*
 * ServosConfig.cpp
 *
 *  Created on: 8 mars 2012
 *      Author: Hugo
 */
#include "ServosConfig.h"
#include <iostream>
#include <map>

using namespace std;

string ServoConfig::class_name() const
{
	return "ServoConfig";
}

string ServosConfig::to_xml() const
{
	stringstream ss;
	ss << "<Servos>";
	for(map<string, ServoConfig>::const_iterator pservo = servos.begin(); pservo != servos.end(); pservo++)
		ss << "<ServoConfig>" << pservo->second.to_xml() << "</ServoConfig>";
	ss << "</Servos>";
	return ss.str();
}

void ServosConfig::from_xml(TiXmlNode * node)
{

	TiXmlNode * servos_node = node->FirstChild( "Servos" );

	if(!servos_node)
		throw string("No servos node in servos config");

	servos.clear();

	for ( TiXmlNode* child = servos_node->FirstChild(); child != 0; child = child->NextSibling())
	{
			ServoConfig servo;
			servo.from_xml(child);
			servos[servo.Name] = servo;
	}

	string serial = to_xml();
	//cout << "ServosConfig: " << serial << endl;
}


string ServosConfig::class_name() const
{
	return "ServosConfig";
}

string ServoConfig::to_xml() const
{
	stringstream ss;
	ss << "<Id>" << (int) Id << "</Id>";
	ss << "<Name>" << Name << "</Name>";
	ss << "<IniAngle>" << IniAngle << "</IniAngle>";
	ss << "<ZeroAngle>" << ZeroAngle << "</ZeroAngle>";
	ss << "<MinAngle>" << MinAngle << "</MinAngle>";
	ss << "<MaxAngle>" << MaxAngle << "</MaxAngle>";
	ss << "<MaxSpeed>" << MaxSpeed << "</MaxSpeed>";
	ss << "<MaxTorque>" << MaxTorque << "</MaxTorque>";
	ss << "<SmoothFactor>" << SmoothFactor << "</SmoothFactor>";
	ss << "<Bounce>" << Bounce << "</Bounce>";
	ss << "<Punch>" << Punch << "</Punch>";
	ss << "<Inverse>" << (Inverse ? 1 : 0) << "</Inverse>";
	ss << "<unit>" << (int) unit << "</unit>";
	return ss.str();
}

void ServoConfig::from_xml(TiXmlNode * node)
{
	Id = XMLTools::get_int_element(node,"Id");
	Name = XMLTools::get_string_element(node,"Name");

	IniAngle = XMLTools::get_double_element(node,"IniAngle");
	ZeroAngle = XMLTools::get_double_element(node,"ZeroAngle");
	MinAngle = XMLTools::get_double_element(node,"MinAngle");
	MaxAngle = XMLTools::get_double_element(node,"MaxAngle");
	MaxSpeed = XMLTools::get_int_element(node,"MaxSpeed");
	MaxTorque = XMLTools::get_int_element(node,"MaxTorque");
	Bounce = (float) XMLTools::get_int_element(node,"Bounce");
	Punch = (float) XMLTools::get_int_element(node, "Punch");
	SmoothFactor = XMLTools::get_double_element(node,"SmoothFactor");

	try
	{
	Inverse = (XMLTools::get_bool_element(node,"Inverse"));
	}catch(...)
	{
		Inverse = false;
	}

	try
	{
		unit = (ui8) XMLTools::get_int_element(node,"unit");
	}
	catch(...){ unit = 1;}

}


ServoConfig ServosConfig::find_servo(string servo_name) const
{
	map<string, ServoConfig>::const_iterator pservo = servos.find(servo_name);
	if(pservo == servos.end())
		throw string("No servo named '") + servo_name + "'";
	else
		return pservo->second;
}


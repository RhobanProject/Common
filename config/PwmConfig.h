/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef PWMCONFIG_H
#define PWMCONFIG_H

#include <cstdlib>
#include <iostream>

#include "names.h"
#include "Config.h"

using namespace std;

class PwmDeviceParameter;

class PwmConfig : public Config
{
    public:
        vector<PwmDeviceParameter*> pwms;

        virtual PwmDeviceParameter *create_parameter(TiXmlNode *node);

        void from_xml(TiXmlNode *node);
        string to_xml() const;

        string class_name() const;
};

class PwmDeviceParameter : public Config
{
public:
	PwmDeviceParameter():frequency(0), cyclic_rate(0){name=""; pin="";};

	string name;
	uint frequency;

	//entre 0 et 100
	float cyclic_rate;
	//maximal cyclic rate
	float min;
	//minimal cyclic rate
	float max;

	string pin;

	void from_xml(TiXmlNode * node);
	string to_xml() const;
	string class_name() const {return "PwmDeviceParameter";}
};

#endif // PWMCONFIG_H

/*************************************************
 * Publicly released by Rhoban System, August 2012
 *             www.rhoban-system.fr
 *
 * Freely usable for non-commercial purposes
 *
 * Licence Creative Commons *CC BY-NC-SA
 * http://creativecommons.org/licenses/by-nc-sa/3.0
 *************************************************/
#ifndef JOYPADCONFIG_H
#define JOYPADCONFIG_H

#include <cstdlib>
#include <iostream>

#include "names.h"
#include "Config.h"

using namespace std;

class JoypadSensorParameter;

class JoypadConfig : public Config
{
    public:
        vector<JoypadSensorParameter> joypads;

        void from_xml(TiXmlNode *node);
        string to_xml() const;

        JoypadSensorParameter create_parameter(TiXmlNode *node);

        JoypadSensorParameter * find_joypad_parameter(string sensor_name);

        string class_name() const;
};

#ifndef JOYPAD_AXIS_NB
#define JOYPAD_AXIS_NB 6
#endif

class JoypadSensorParameter : public Config
{
    public:
        JoypadSensorParameter();

        //empty name means that the sensor is deactivated
        string name;

        //etalonnage, cf normalize
        double min[JOYPAD_AXIS_NB];
        double max[JOYPAD_AXIS_NB];
        double zero[JOYPAD_AXIS_NB];

        double normalize(int val, uint axis_id);

        //loads a configuration file
        void from_xml(TiXmlNode * node);
        string to_xml() const;

        string class_name() const {return "JoypadSensorParameter";}

    private:
        double norm_coef[JOYPAD_AXIS_NB];

};

#endif // JOYPADCONFIG_H

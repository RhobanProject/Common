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
 * ServosConfig.hpp
 *
 *  Created on: 8 mars 2012
 *      Author: Hugo
 */

#ifndef SERVOSCONFIG_HPP_
#define SERVOSCONFIG_HPP_

#include "Config.h"
#include "util.h"

#ifdef NO_ALGEBRA
typedef double scalar;
#endif

class ServoConfig : public Config
{
    public:

        ui8 Id;
        string Name;

        ui8 unit; //0:rad 1:deg 2:raw
        scalar IniAngle;
        scalar ZeroAngle;
        scalar MinAngle;
        scalar MaxAngle;
        ui16 MaxSpeed;
        ui16 MaxTorque;
        float Bounce;
        float Punch;

        bool Inverse;

        float SmoothFactor;

        ServoConfig() : Id(0), unit(1), IniAngle(0), MinAngle(0), MaxAngle(0), MaxSpeed(1023), MaxTorque(1023), Bounce(50), Punch(0), Inverse(false), SmoothFactor(0) { Name = "";}; //Default unit is degree

        string class_name() const;

        void from_xml(TiXmlNode * node);
        string to_xml() const;

};

class ServosConfig : public Config
{

    public:
        void from_xml(TiXmlNode *node);

        string to_xml() const;

        string class_name() const;

        vector<ServoConfig> servos;

        ServoConfig find_servo(string servo_name);
};

#endif /* SERVOSCONFIG_HPP_ */

#include "JoypadConfig.h"

void JoypadConfig::from_xml(TiXmlNode * node)
{
    joypads.clear();
    TiXmlNode * joypads_node = node->FirstChild( "joypads" );

    if(joypads_node)
        for ( TiXmlNode* child = joypads_node->FirstChild(); child != 0; child = child->NextSibling())
        {
            joypads.push_back(create_parameter(child));
        }
}

JoypadSensorParameter JoypadConfig::create_parameter(TiXmlNode *node)
{
    JoypadSensorParameter jsp;
    jsp.from_xml(node);
    return jsp;
}

string JoypadConfig::to_xml() const
{
    string result;
    result += "<joypads>";
    for(uint i =0; i< joypads.size(); i++)
    {
        const JoypadSensorParameter * jsp = &(joypads[i]);
        result += "<" + jsp->class_name() + ">" + jsp->to_xml() + "</" + jsp->class_name() + ">";
    }
    result +="</joypads>";
    return result;
}

string JoypadConfig::class_name() const
{
    return JOYPAD_CONFIG_FILENAME;
}

JoypadSensorParameter * JoypadConfig::find_joypad_parameter(string sensor_name)
{
	for(uint i = 0; i< joypads.size(); i++)
	{
		JoypadSensorParameter * jsp = &(joypads[i]);
		if(jsp->name == sensor_name)
			return jsp;
	}
	return NULL;
}

JoypadSensorParameter::JoypadSensorParameter()
{
    name="";
    for(int i = 0; i< JOYPAD_AXIS_NB; i++)
    {
        min[i] = -1;
        max[i] = 1;
        zero[i] = 0;
    }
}

void JoypadSensorParameter::from_xml(TiXmlNode * node)
{
    name = XMLTools::get_string_element(node,"name");

    Matrix bound;

    bound = XMLTools::extract_double_array(node,"min");
    for(uint i = 0; (i< JOYPAD_AXIS_NB) && (i< bound.size()); i++)
        min[i] = bound[i];

    bound = XMLTools::extract_double_array(node,"max");
    for(uint i = 0; (i< JOYPAD_AXIS_NB) && (i< bound.size()); i++)
        max[i] = bound[i];

    bound = XMLTools::extract_double_array(node,"zero");
    for(uint i = 0; (i< JOYPAD_AXIS_NB) && (i< bound.size()); i++)
        zero[i] = bound[i];

    for(uint i = 0; (i< JOYPAD_AXIS_NB) && (i< bound.size()); i++)
        norm_coef[i] = 2.0 / (max[i] - min[i]);

}

string JoypadSensorParameter::to_xml() const
{
    ostringstream result;
    result << "<name>" << name << "</name>";

    vector<double> vec(JOYPAD_AXIS_NB);

    for(int i=0; i< JOYPAD_AXIS_NB; i++)
        vec[i] = min[i];
    result << "<min>" << XMLTools::serialize_double_array(vec) << "</min>";

    for(int i=0; i< JOYPAD_AXIS_NB; i++)
        vec[i] = max[i];
    result << "<max>" << XMLTools::serialize_double_array(vec) << "</max>";

    for(int i=0; i< JOYPAD_AXIS_NB; i++)
        vec[i] = zero[i];
    result << "<zero>" << XMLTools::serialize_double_array(vec) << "</zero>";

    return result.str();
}

double JoypadSensorParameter::normalize(int val, uint axis_id)
{
    if(axis_id >= JOYPAD_AXIS_NB)
        throw string("JoypadSensorParameter: normalize: axis_id too large");

    return norm_coef[axis_id] * (val - zero[axis_id] ) ;
}

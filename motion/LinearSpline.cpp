/*
 * Spline.cpp
 *
 *  Created on: 11 janv. 2013
 *      Author: Hugo
 */


#include <algorithm>
#include "util.h"

#include "LinearSpline.h"

namespace Rhoban
{

void LinearSplineSequence::set_raw_data(vector<float> raw_data, bool use_tangents)
{
    points.resize(raw_data.size() / (use_tangents ? 4 : 2) );
    uint ind = 0;
    for(uint i = 0 ; i< points.size(); i++)
    {
        SplinePoint * point = &points[i];
        point->x = raw_data[ind++];
        point->y = raw_data[ind++];
        if(use_tangents)
        {
            point->left_tangent = raw_data[ind++];
            point->right_tangent = raw_data[ind++];
        }
    }
    sort_sequence();
}

void LinearSplineSequence::to_raw_data(	vector<float> & result, bool use_tangents) const
{
    result.resize( (use_tangents ? 4 : 2)  * points.size());
    uint ind = 0;
    for(uint i = 0 ; i< points.size(); i++)
    {
        const SplinePoint * point = &points[i];
        result[ind++] = point->x;
        result[ind++] = point->y;
        if(use_tangents)
        {
            result[ind++] = point->left_tangent;
            result[ind++] = point->right_tangent;
        }
    }
}

void LinearSpline::set_raw_data(vector < vector<float> > raw_data)
{
    if(sequences.size() != raw_data.size())
        throw string("Sequence number and raw data size do not match for spline "+ name);
    for(uint i = 0; i < sequences.size(); i++)
    {
        LinearSplineSequence * ss = &sequences[i];
        ss->set_raw_data(raw_data[i], (ss->type == 1));
    }
}

void LinearSpline::to_raw_data(vector < vector<float> > & result) const
{
    result.resize(sequences.size());
    for(uint i = 0; i < sequences.size(); i++)
        sequences[i].to_raw_data(result[i]);
}



string LinearSplineSequence::SplinePoint::to_xml()
{
    ostringstream oss;
    oss << "<SplinePoint>";
    oss << "<x>" << x << "</x>";
    oss << "<y>" << y << "</y>";
    oss << "<left_tangent>" << left_tangent << "</left_tangent>";
    oss << "<right_tangent>" << right_tangent << "</right_tangent>";
    oss << "</SplinePoint>";
    return oss.str();
}

bool my_sort(LinearSplineSequence::SplinePoint a, LinearSplineSequence::SplinePoint b)
{
    return a.x < b.x;
}

void LinearSplineSequence::sort_sequence()
{
    sort( points.begin() , points.end(), my_sort);
}

string LinearSpline::to_xml(bool only_header)
{
    ostringstream oss;
    oss << "<name>" << name << "</name>";
    oss << "<gain>1</gain>";
    oss << "<shift>" << shift << "</shift>";
    oss << "<speed_factor>" << speed_factor << "</speed_factor>";
    oss << "<entrytype>" << (update_type  == time_entry ? 0 : (update_type == first_entry ? 1 : 2) ) << "</entrytype>";
    oss << "<auto_start>0</auto_start>";

    oss << "<sequences>";

    for(uint i = 0 ; i< sequences.size(); i++)
        oss << sequences[i].to_xml(only_header);
    oss << "</sequences>";
    return oss.str();
}

string LinearSplineSequence::to_xml(bool only_header)
{
    ostringstream oss;
    oss << "<SplineSequence>";
    oss << "<name>" << name << "</name>";
    oss << "<type>" << type << "</type>";
    oss << "<cyclic>" << cyclic << "</cyclic>";
    oss << "<bounded>" << bounded << "</bounded>";
    oss << "<Recordable>" << (Recordable ? "true" : "false") << "</Recordable>";
    oss << "<Points>";
    oss << "<Points>";
    if(!only_header)
    	for(uint i =0; i< points.size(); i++)
    		oss << points[i].to_xml();
    oss << "</Points>";
    oss << "</Points>";
    oss << "</SplineSequence>";
    return oss.str();
}

}



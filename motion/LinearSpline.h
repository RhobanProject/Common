/*
 * RecordedSpline.h
 *
 *  Created on: 11 janv. 2013
 *      Author: Hugo
 */

#ifndef RECORDEDSPLINE_H_
#define RECORDEDSPLINE_H_

using namespace std;

#include <xml/XMLTools.h>

namespace Rhoban
{


class LinearSplineSequence
{

public:
	/*
	 * The name of the sequence
	 */
	string name;

	/* The type of a spline:
	 * 0 : piecewise linear
	 * 1 : polynomial interpolation
	 */
	int type;

	/* The cyclicity of the spline
	 * 1 means cyclic:
	 * 	output(x) = f( x % length)

	 * 0 means infinite, the behaviour depends on the flag bounded
	 */
	int cyclic;

	/*
	 * 0 means that the spline is interpolated linearly when affix
	 * is smaller than x_min or bigger than x_max
	 *
	 * 1 means that affix cannot go above/below these values:
	 * output(x) = f(x') where
	 * 	 x' = x if x_min <= x <= x_max
	 * 	 x' = f(x_max) if x' > x_max
	 * 	 x' = f(x_min) if x' < x_min
	 */
	int bounded;

	class SplinePoint {
	public:
		float x;                     // the time position of the point
		float y;                     // the value of the spline at the point
		float left_tangent;          // the left tangent rate at the point
		float right_tangent;         // the right tangent rate at the point
		SplinePoint(float x, float y, float left_tangent, float right_tangent):
			x(x), y(y), left_tangent(left_tangent), right_tangent(right_tangent) {};
		SplinePoint() : x(0.0), y(0.0), left_tangent(0.0), right_tangent(0.0) {};
		string to_xml();
	};

	/* The list of all the points of the sequence sorted according to X values. X value must be
	 * in [0, length[ */
	vector<SplinePoint> points;

	void add_point(float x, float y);

	LinearSplineSequence(): type(0), cyclic(0), bounded(1), name(""){};
	LinearSplineSequence(TiXmlNode * sequencenode);
	virtual ~LinearSplineSequence(){};

	string to_xml(bool only_header = false);
	void to_raw_data(vector<float> & result, bool use_tangents = false) const;
	void set_raw_data(vector<float> raw_data, bool use_tangents = false);
};

/**
 * LinearSplines are collection of sequences of real values (x_n,y_n)_{n \in N}
 * They can be used to describe trajectories if x_n is used for time
 * In general they can be used to describe collection of linear mappings
 */
class LinearSpline : public Serializable {

public:
	/* The name of the spline */
	string name;

	/* The shift of the spline in millisecond
	 * when the spline is played entries
	 * are shifted by 'shift'
	 *
	 * output(x) = f(x + shift)
	 */
	double shift;

	/* The speed_factor of the spline in millisecond
	 * when the spline is played entries
	 * are multiplied by 'speed_factor'
	 * (shift is added first)
	 *
	 * output(x) = f( speed_factor * ( x + shift) )
	 *
	 */
	double speed_factor;


	/* The way he spline is updated */
	//time_entry means that x is the time since the spline is played
	//first_entry means that x is the first entry of the spline block
	//multi_entry means that x of sequence number n is value of input link number n
	typedef enum
	{
		time_entry,
		first_entry,
		multi_entry
	} SplineUpdateType;
	SplineUpdateType update_type;

	/* The list of all sequences of the spline*/
	vector<LinearSplineSequence> sequences;

	/*
	 * (de)serialization
	 */
	virtual void from_xml(TiXmlNode * node);
	string to_xml(bool only_header = false);
	void to_raw_data(vector < vector<float> > &) const;
	void set_raw_data(vector < vector<float> >);

	/* adds a new sequence */
	void add_sequence(string);

	/* dont modify */
	string class_name() const { return "Spline";}

	LinearSpline() : name(""), shift(0), speed_factor(1), update_type(time_entry) {};
	virtual ~LinearSpline(){};

};
}

#endif /* RECORDEDSPLINE_H_ */

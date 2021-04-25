#ifndef INCLUDED_CATMULL_ROM_CURVE_EVALUATOR_H
#define INCLUDED_CATMULL_ROM_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

struct Segment
{
	Point a;
	Point b;
	Point c;
	Point d;
};

class CatmullRomCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
	Point evaluatePoint(float t, Point p1, Point p2, Point p3, Point p4) const;

	float tension = 0.9;
};

#endif#pragma once

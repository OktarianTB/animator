#ifndef INCLUDED_BEZIER_CURVE_EVALUATOR_H
#define INCLUDED_BEZIER_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"

//using namespace std;

class BezierCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;

	Point evaluatePoint(float t, Point p0, Point p1, Point p2, Point p3) const;
	Point evaluateLinearPoint(float t, Point p0, Point p1) const;
	float findEdgeT(Point p0, Point p1, float edge) const;
};

#endif#pragma once

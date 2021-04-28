#ifndef INCLUDED_ADAPTIVE_BEZIER_CURVE_EVALUATOR_H
#define INCLUDED_ADAPTIVE_BEZIER_CURVE_EVALUATOR_H

#pragma warning(disable : 4786)  

#include "CurveEvaluator.h"
#include "modelerui.h"

//using namespace std;

class AdaptiveBezierCurveEvaluator : public CurveEvaluator
{
public:
	void evaluateCurve(const std::vector<Point>& ptvCtrlPts,
		std::vector<Point>& ptvEvaluatedCurvePts,
		const float& fAniLength,
		const bool& bWrap) const;
	void displayBezier(Point p0, Point p1, Point p2, Point p3, std::vector<Point>& evaluatedPts) const;
	bool isFlatEnough(Point p0, Point p1, Point p2, Point p3) const;
};

#endif#pragma once

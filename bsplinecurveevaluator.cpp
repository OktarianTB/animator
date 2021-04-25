#include "BSplineCurveEvaluator.h"
#include <assert.h>

void BSplineCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	int iCtrlPtCount = ptvCtrlPts.size();
	int numSegments = 20; // this decides how smooth the line looks
	float step = 1 / (float)numSegments;

	std::vector<Point> evaluatedPts; // these are the evaluating points which are returned
}

#include "BezierCurveEvaluator.h"
#include <assert.h>
#include <algorithm>

using namespace std;

void BezierCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	int iCtrlPtCount = ptvCtrlPts.size();
	int numSegments = 20; // this decides how smooth the line looks
	float step = 1 / (float)numSegments;

	std::vector<Point> evaluatedPts; // these are the evaluating points which are returned
	std::vector<Point> blacklist; // these are points that shouldn't be added as evaluating points

	if (bWrap)
	{
		if (iCtrlPtCount % 3 == 0) // we can wrap using bezier curve
		{
			Point p0(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y);
			Point p1(ptvCtrlPts[iCtrlPtCount - 2].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y);
			Point p2(ptvCtrlPts[iCtrlPtCount - 3].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y);
			Point p3(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);

			for (int seg = 1; seg <= numSegments; seg++)
			{
				float t = seg * step; 

				// construct the bezier curve on the left side
				Point left = evaluatePoint(t, p0, p1, p2, ptvCtrlPts[0]);
				evaluatedPts.push_back(left);

				// construct the bezier curve on the right side
				Point right = evaluatePoint(t, ptvCtrlPts[iCtrlPtCount - 3], ptvCtrlPts[iCtrlPtCount - 2], ptvCtrlPts[iCtrlPtCount - 1], p3);
				evaluatedPts.push_back(right);
			}

			blacklist.push_back(ptvCtrlPts[iCtrlPtCount - 2]);
			blacklist.push_back(ptvCtrlPts[iCtrlPtCount - 1]);
		}
		else
		{
			// default to linear interpolation
			Point p0(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y);
			float t1 = findEdgeT(p0, ptvCtrlPts[0], 0);
			Point left = evaluateLinearPoint(t1, p0, ptvCtrlPts[0]);
			evaluatedPts.push_back(left);

			Point p1(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
			float t2 = findEdgeT(ptvCtrlPts[0], p1, fAniLength);
			Point right = evaluateLinearPoint(t2, ptvCtrlPts[iCtrlPtCount - 1], p1);
			evaluatedPts.push_back(right);
		}
	}
	else {
		evaluatedPts.push_back(Point(0.0, ptvCtrlPts[0].y));
		evaluatedPts.push_back(Point(fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y));
	}

	int i = 0;
	while (i < iCtrlPtCount)
	{
		// Add first and last control points
		if ((i == 0 || i == iCtrlPtCount - 1) && std::find(blacklist.begin(), blacklist.end(), ptvCtrlPts[i]) == blacklist.end())
		{
			evaluatedPts.push_back(ptvCtrlPts[i]);
		}

		// If sufficient control points, create bezier curve
		if (i + 3 < iCtrlPtCount)
		{
			for (int seg = 1; seg <= numSegments; seg++)
			{
				float t = seg * step;
				Point p = evaluatePoint(t, ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvCtrlPts[i + 3]);
				evaluatedPts.push_back(p);
			}

			i += 3;
			continue;
		}
		// Otherwise add intermediary control points
		else if (i > 0 && i < iCtrlPtCount - 1 && std::find(blacklist.begin(), blacklist.end(), ptvCtrlPts[i]) == blacklist.end()) {
			evaluatedPts.push_back(ptvCtrlPts[i]);
		}

		i++;
	}

	ptvEvaluatedCurvePts = evaluatedPts;
}

Point BezierCurveEvaluator::evaluatePoint(float t, Point p0, Point p1, Point p2, Point p3) const
{
	// Formula for cubic Bezier curve: (1-t)^3 * P0 + 3 * t * (1-t)^2 * P1 + 3 * t^2 * (1-t) * P2 + t^3 * P4

	float t2 = (1 - t) * (1 - t); // (1-t)^2
	float t3 = t2 * (1 - t); // (1-t)^3
	float t_p1 = t2 * t;
	float t_p2 = t * t * (1 - t);
	float t_p3 = t * t * t;

	float x = t3 * p0.x + 3 * t_p1 * p1.x + 3 * t_p2 * p2.x + t_p3 * p3.x;
	float y = t3 * p0.y + 3 * t_p1 * p1.y + 3 * t_p2 * p2.y + t_p3 * p3.y;

	return Point(x, y);
}

Point BezierCurveEvaluator::evaluateLinearPoint(float t, Point p0, Point p1) const
{
	// Formula for linear interpolation: (1-t) * P0 + P1

	float x = (1 - t) * p0.x + t * p1.x;
	float y = (1 - t) * p0.y + t * p1.y;

	return Point(x, y);
}

// Function to find value of t such as it lies on the edge
float BezierCurveEvaluator::findEdgeT(Point p0, Point p1, float edge) const
{
	for (float t = 0; t < 1; t += 0.01)
	{
		Point p = evaluateLinearPoint(t, p0, p1);
		if (p.x >= edge)
			return t;
	}

	return 0.0f;
}

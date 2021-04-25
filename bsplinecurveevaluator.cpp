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

	if (bWrap)
	{
		// left side
		Point p1_left(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y);
		Point p2_left(ptvCtrlPts[iCtrlPtCount - 2].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y);

		// right side
		Point p1_right(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
		Point p2_right(ptvCtrlPts[1].x + fAniLength, ptvCtrlPts[1].y);

		for (int seg = 1; seg <= numSegments; seg++)
		{
			float t = seg * step;

			// construct the curve on the left side
			Point left = evaluatePoint(t, p1_left, p2_left, ptvCtrlPts[0], ptvCtrlPts[1]);
			evaluatedPts.push_back(left);

			// construct the curve on the right side
			Point right = evaluatePoint(t, ptvCtrlPts[iCtrlPtCount - 2], ptvCtrlPts[iCtrlPtCount - 1], p1_right, p2_right);
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
		// First and last segments
		if ((i == 1 || i == iCtrlPtCount - 2) && iCtrlPtCount >= 3)
		{
			Point p1(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y);
			Point p2(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);

			for (int seg = 1; seg <= numSegments; seg++)
			{
				float t = seg * step;
				Point p_left = evaluatePoint(t, p1, ptvCtrlPts[0], ptvCtrlPts[1], ptvCtrlPts[2]);
				evaluatedPts.push_back(p_left);
			}

			for (int seg = 1; seg <= numSegments; seg++)
			{
				float t = seg * step;
				Point p_right = evaluatePoint(t, ptvCtrlPts[iCtrlPtCount - 3], ptvCtrlPts[iCtrlPtCount - 2], ptvCtrlPts[iCtrlPtCount - 1], p2);
				evaluatedPts.push_back(p_right);
			}
		}

		// If sufficient control points, create b-spline curve
		else if (i + 3 < iCtrlPtCount)
		{
			float previous_x = 0;
			for (int seg = 1; seg <= numSegments; seg++)
			{
				float t = seg * step;
				Point p = evaluatePoint(t, ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvCtrlPts[i + 3]);
				evaluatedPts.push_back(p);
			}
		}

		i++;
	}

	ptvEvaluatedCurvePts = evaluatedPts;
}

Point BSplineCurveEvaluator::evaluatePoint(float t, Point p0, Point p1, Point p2, Point p3) const
{
	// Formula from the lecture notes L17 slide 26, I have no clue what it's doing but works

	float v0_x = 0.17 * (p0.x + 4 * p1.x + p2.x);
	float v0_y = 0.17 * (p0.y + 4 * p1.y + p2.y);
	Point v0(v0_x, v0_y);

	float v1_x = 0.17 * (4 * p1.x + 2 * p2.x);
	float v1_y = 0.17 * (4 * p1.y + 2 * p2.y);
	Point v1(v1_x, v1_y);

	float v2_x = 0.17 * (2 * p1.x + 4 * p2.x);
	float v2_y = 0.17 * (2 * p1.y + 4 * p2.y);
	Point v2(v2_x, v2_y);

	float v3_x = 0.17 * (p1.x + 4 * p2.x + p3.x);
	float v3_y = 0.17 * (p1.y + 4 * p2.y + p3.y);
	Point v3(v3_x, v3_y);

	float t2 = t * t;
	float t3 = t2 * t;

	float x = (-v0.x + 3 * v1.x - 3 * v2.x + v3.x) * t3 + (3 * v0.x - 6 * v1.x + 3 * v2.x) * t2 + (-3 * v0.x + 3 * v1.x) * t + v0.x;
	float y = (-v0.y + 3 * v1.y - 3 * v2.y + v3.y) * t3 + (3 * v0.y - 6 * v1.y + 3 * v2.y) * t2 + (-3 * v0.y + 3 * v1.y) * t + v0.y;

	return Point(x, y);
}

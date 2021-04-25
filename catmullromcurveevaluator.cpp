#include "CatmullRomCurveEvaluator.h"
#include <assert.h>

void CatmullRomCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
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
		// Add first and last control points
		if ((i == 0 || i == iCtrlPtCount - 1))
		{
			evaluatedPts.push_back(ptvCtrlPts[i]);
		}

		// First and last segments
		if ((i == 1 || i == iCtrlPtCount - 2)&& iCtrlPtCount >= 3)
		{
			Point p1(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y);
			Point p2(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);

			float previous_x = 0; // fixes bug with monotically increasing x
			for (int seg = 1; seg <= numSegments; seg++)
			{
				float t = seg * step;
				Point p_left = evaluatePoint(t, p1, ptvCtrlPts[0], ptvCtrlPts[1], ptvCtrlPts[2]);
				if (p_left.x > previous_x)
				{
					evaluatedPts.push_back(p_left);
					previous_x = p_left.x;
				}
			}

			previous_x = 0; // fixes bug with monotically increasing x
			for (int seg = 1; seg <= numSegments; seg++)
			{
				float t = seg * step;
				Point p_right = evaluatePoint(t, ptvCtrlPts[iCtrlPtCount - 3], ptvCtrlPts[iCtrlPtCount - 2], ptvCtrlPts[iCtrlPtCount - 1], p2);
				
				if (p_right.x > previous_x)
				{
					evaluatedPts.push_back(p_right);
					previous_x = p_right.x;
				}
			}
		}

		// If sufficient control points, create catmull-rom curve
		if (i + 3 < iCtrlPtCount)
		{
			float previous_x = 0; // fixes bug with monotically increasing x
			for (int seg = 1; seg <= numSegments; seg++)
			{
				float t = seg * step;
				Point p = evaluatePoint(t, ptvCtrlPts[i], ptvCtrlPts[i + 1], ptvCtrlPts[i + 2], ptvCtrlPts[i + 3]);
				if (p.x > previous_x)
				{
					evaluatedPts.push_back(p);
					previous_x = p.x;
				}
			}
		}
		// Otherwise add intermediary control points
		else if (i > 0 && i < iCtrlPtCount - 1) {
			evaluatedPts.push_back(ptvCtrlPts[i]);
		}

		i++;
	}

	ptvEvaluatedCurvePts = evaluatedPts;
}


Point CatmullRomCurveEvaluator::evaluatePoint(float t, Point p1, Point p2, Point p3, Point p4) const
{
	// From https://pomax.github.io/bezierinfo/#catmullconv

	Point k1 = p2;

	float k2_x = p2.x + (p3.x - p1.x) / (6 * tension);
	float k2_y = p2.y + (p3.y - p1.y) / (6 * tension);
	Point k2(k2_x, k2_y);

	float k3_x = p3.x - (p4.x - p2.x) / (6 * tension);
	float k3_y = p3.y - (p4.y - p2.y) / (6 * tension);
	Point k3(k3_x, k3_y);

	Point k4 = p3;

	// Formula for cubic Bezier curve: (1-t)^3 * P0 + 3 * t * (1-t)^2 * P1 + 3 * t^2 * (1-t) * P2 + t^3 * P4

	float t2 = (1 - t) * (1 - t); // (1-t)^2
	float t3 = t2 * (1 - t); // (1-t)^3
	float t_p1 = t2 * t;
	float t_p2 = t * t * (1 - t);
	float t_p3 = t * t * t;

	float x = t3 * k1.x + 3 * t_p1 * k2.x + 3 * t_p2 * k3.x + t_p3 * k4.x;
	float y = t3 * k1.y + 3 * t_p1 * k2.y + 3 * t_p2 * k3.y + t_p3 * k4.y;

	return Point(x, y);
}

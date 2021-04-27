#include "C2CurveEvaluator.h"
#include <assert.h>
#include "vec.h"
#include "mat.h"

void C2CurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	int iCtrlPtCount = ptvCtrlPts.size();
	int numSegments = 20; // this decides how smooth the line looks
	float step = 1 / (float)numSegments;

	std::vector<Point> evaluatedPts; // these are the evaluating points which are returned

	std::vector<Point> newCtrlPoints(ptvCtrlPts.begin(), ptvCtrlPts.end());
	float wrapLength = ptvCtrlPts[0].x + (fAniLength - ptvCtrlPts.back().x);

	if (bWrap)
	{
		Point temp = Point(ptvCtrlPts[0].x - wrapLength, ptvCtrlPts[iCtrlPtCount - 1].y);
		newCtrlPoints.insert(newCtrlPoints.begin(), temp);
		temp = Point(fAniLength + ptvCtrlPts[0].x, ptvCtrlPts[0].y);
		newCtrlPoints.push_back(temp);
		temp = Point(fAniLength + ptvCtrlPts[0].x + 1, ptvCtrlPts[0].y);
		newCtrlPoints.push_back(temp);
		temp = Point(fAniLength + ptvCtrlPts[0].x + 2, ptvCtrlPts[0].y);
		newCtrlPoints.push_back(temp);
	}
	else
	{
		Point temp = Point(-4.0, ptvCtrlPts[0].y);
		newCtrlPoints.insert(newCtrlPoints.begin(), temp);

		temp = Point(fAniLength + 4.0, ptvCtrlPts[iCtrlPtCount - 1].y);
		newCtrlPoints.push_back(temp);
		temp = Point(fAniLength + 5.0, ptvCtrlPts[iCtrlPtCount - 1].y);
		newCtrlPoints.push_back(temp);
		temp = Point(fAniLength + 6.0, ptvCtrlPts[iCtrlPtCount - 1].y);
		newCtrlPoints.push_back(temp);
	}

	Mat4f bezierBasis(-1,	3,	-3,   1,
					   3,	-6,  3,   0,
					  -3,	3,   0,   0,
					   1,	0,   0,	  0);

	Mat4f c2Inter(26,   -7,   2,   -1,
				  -7,   14,  -4,    2,
				   2,   -4,  14,   -7,
				  -1,    2,  -7,   26);

	for (int i = 0; i < newCtrlPoints.size() - 3; i++)
	{
		Vec4f xCoords = 3.0 * Vec4f(newCtrlPoints[i + 1].x - newCtrlPoints[i].x, newCtrlPoints[i + 2].x - newCtrlPoints[i].x, newCtrlPoints[i + 3].x - newCtrlPoints[i + 1].x, newCtrlPoints[i + 3].x - newCtrlPoints[i + 2].x);
		Vec4f yCoords = 3.0 * Vec4f(newCtrlPoints[i + 1].y - newCtrlPoints[i].y, newCtrlPoints[i + 2].y - newCtrlPoints[i].y, newCtrlPoints[i + 3].y - newCtrlPoints[i + 1].y, newCtrlPoints[i + 3].y - newCtrlPoints[i + 2].y);

		Vec4f xDs, yDs;
		xDs = c2Inter * xCoords / 45.0;
		yDs = c2Inter * yCoords / 45.0;

		Vec4f xInters, yInters;

		xInters = Vec4f(newCtrlPoints[i].x, newCtrlPoints[i].x + 1 / 3.0 * xDs[0], newCtrlPoints[i + 1].x - 1 / 3.0 * xDs[1], newCtrlPoints[i + 1].x);
		yInters = Vec4f(newCtrlPoints[i].y, newCtrlPoints[i].y + 1 / 3.0 * yDs[0], newCtrlPoints[i + 1].y - 1 / 3.0 * yDs[1], newCtrlPoints[i + 1].y);

		Vec4f xBez, yBez;

		xBez = bezierBasis * xInters;
		yBez = bezierBasis * yInters;

		float previous_x = -1;
		for (int seg = 1; seg <= numSegments; seg++)
		{
			float t = seg * step;

			float x = xBez[3] + t * xBez[2] + t * t * xBez[1] + t * t * t * xBez[0];
			float y = yBez[3] + t * yBez[2] + t * t * yBez[1] + t * t * t * yBez[0];

			Point p(x, y);

			if (p.x > previous_x)
			{
				previous_x = p.x;
				evaluatedPts.push_back(p);
			}
		}
	}

	ptvEvaluatedCurvePts = evaluatedPts;
}
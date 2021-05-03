#include "LaneRiesenfeldCurveEvaluator.h"
#include "vec.h"
#include "mat.h"

void LaneRiesenfeldCurveEvaluator::evaluateCurve(const std::vector<Point>& ptvCtrlPts,
	std::vector<Point>& ptvEvaluatedCurvePts,
	const float& fAniLength,
	const bool& bWrap) const
{
	int iCtrlPtCount = ptvCtrlPts.size();
	std::vector<Point> evaluatedPts; // these are the evaluating points which are returned
	std::vector<Point> newCtrlPoints(ptvCtrlPts.begin(), ptvCtrlPts.end());

	if (bWrap)
	{
		// Insert points at the beginning of the curve
		Point temp = Point(ptvCtrlPts[iCtrlPtCount - 1].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 1].y);
		newCtrlPoints.insert(newCtrlPoints.begin(), temp);
		temp = Point(ptvCtrlPts[iCtrlPtCount - 2].x - fAniLength, ptvCtrlPts[iCtrlPtCount - 2].y);
		newCtrlPoints.insert(newCtrlPoints.begin(), temp);

		// Insert points at the end of the curve
		temp = Point(ptvCtrlPts[0].x + fAniLength, ptvCtrlPts[0].y);
		newCtrlPoints.push_back(temp);
		temp = Point(ptvCtrlPts[1].x + fAniLength, ptvCtrlPts[1].y);
		newCtrlPoints.push_back(temp);
	}
	else
	{
		// Insert points at the beginning of the curve
		Point temp = Point(ptvCtrlPts[0].x, ptvCtrlPts[0].y);
		newCtrlPoints.insert(newCtrlPoints.begin(), temp);
		newCtrlPoints.insert(newCtrlPoints.begin(), temp);

		temp = Point(-4.0, ptvCtrlPts[0].y);
		newCtrlPoints.insert(newCtrlPoints.begin(), temp);
		newCtrlPoints.insert(newCtrlPoints.begin(), temp);

		// Insert points at the end of the curve
		temp = Point(ptvCtrlPts[iCtrlPtCount - 1].x, ptvCtrlPts[iCtrlPtCount - 1].y);
		newCtrlPoints.push_back(temp);
		newCtrlPoints.push_back(temp);

		temp = Point(fAniLength + 4, ptvCtrlPts[iCtrlPtCount - 1].y);
		newCtrlPoints.push_back(temp);
		newCtrlPoints.push_back(temp);
	}

	int numNewCtrlPoints = newCtrlPoints.size();

	int smoothness = 5;

	Mat3f subDivBasis(4, 4, 0,
					  1, 6, 1,
					  0, 4, 4);

	Vec3f xCoords, yCoords;

	std::vector<Point> tempEvalPoints;
	for (int i = 1; i <= smoothness; i++)
	{
		numNewCtrlPoints = newCtrlPoints.size();
		tempEvalPoints.clear();
		for (int j = 0; j < (numNewCtrlPoints - 2); j++)
		{
			xCoords = Vec3f(newCtrlPoints[j].x, newCtrlPoints[j + 1].x, newCtrlPoints[j + 2].x);
			yCoords = Vec3f(newCtrlPoints[j].y, newCtrlPoints[j + 1].y, newCtrlPoints[j + 2].y);

			xCoords = subDivBasis * xCoords / 8.0;
			yCoords = subDivBasis * yCoords / 8.0;

			tempEvalPoints.push_back(Point(xCoords[0], yCoords[0]));
			tempEvalPoints.push_back(Point(xCoords[1], yCoords[1]));
		}
		tempEvalPoints.push_back(Point(xCoords[2], yCoords[2]));
		newCtrlPoints.clear();
		newCtrlPoints.insert(newCtrlPoints.begin(), tempEvalPoints.begin(), tempEvalPoints.end());
	}

	evaluatedPts.insert(evaluatedPts.begin(), tempEvalPoints.begin(), tempEvalPoints.end());

	ptvEvaluatedCurvePts = evaluatedPts;
}
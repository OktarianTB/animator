#pragma once
#include <cmath>
#include <vector>

#define COLOR_LIGHT_YELLOW  1.00f, 1.00f, 0.30f
#define COLOR_DARK_BLUE     0.00f, 0.00f, 0.20f
#define PI 3.14
#define deg2rad(x) (x * (PI / 180))
#define rad2deg(x) ((x * 180.0) / PI)

struct Vector
{
	float x;
	float y;
	Vector() { x = 0; y = 0; }
	Vector(float x_, float y_) { x = x_; y = y_; }

	void setMagnitude(float newMag) {
		float mag = sqrt(x * x + y * y);
		x = x * newMag / mag;
		y = y * newMag / mag;
	}

	void mult(float m) {
		x *= m;
		y *= m;
	}
};

class Segment {
public:
	Vector a;
	Vector b;
	float angle = 0;
	float len;

	Segment(float x, float y, float len_);
	Segment(Segment parent, float len_);
	void follow(Segment child);
	void follow(float tx, float ty);
	void setA(Vector pos);
	void calculateB();
	void show();
};

class IK {
public: 
	IK(int numSegments, float length);
	~IK();
	void setTarget(float x, float y, float z);
	void show();

private:
	void update();

	int numSegments;
	float len;
	std::vector<Segment> segments;

	Vector base;
	Vector target;
	float targetZ;
};
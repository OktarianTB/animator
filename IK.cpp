#include "IK.h"
#include "modelerdraw.h"
#include <iostream>

using namespace std;

Segment::Segment(float x, float y, float len_) {
    a = Vector(x, y);
    len = len_;
    calculateB();
}

Segment::Segment(Segment parent, float len_) {
    a = parent.b;
    len = len_;
    calculateB();
}

void Segment::follow(Segment child)
{
    float targetX = child.a.x;
    float targetY = child.a.y;
    follow(targetX, targetY);
}

void Segment::follow(float tx, float ty)
{
    Vector dir(tx - a.x, ty - a.y);
    angle = atan2(dir.y, dir.x);
    dir.setMagnitude(len);
    dir.mult(-1);
    a = Vector(tx + dir.x, ty + dir.y);
}

void Segment::setA(Vector pos)
{
    a = pos;
    calculateB();
}

void Segment::calculateB()
{
    float dx = len * cos(angle);
    float dy = len * sin(angle);
    b = Vector(a.x + dx, a.y + dy);
}

void Segment::show()
{
   // cout << "a = (" << a.x << ", " << a.y << "), b = (" << b.x << ", " << b.y << "), angle = " << angle << endl;

    setDiffuseColor(COLOR_DARK_BLUE);
    glPushMatrix();
    glTranslated(a.x, a.y, 0);
    drawSphere(0.2);
    glPopMatrix();

    setDiffuseColor(COLOR_LIGHT_YELLOW);
    for (float t = 0.2; t < 1; t += 0.1)
    {
        float x = t * a.x + (1 - t) * b.x;
        float y = t * a.y + (1 - t) * b.y;

        glPushMatrix();
        glTranslated(x, y, 0);
        drawSphere(0.1);
        glPopMatrix();
    }

    setDiffuseColor(COLOR_DARK_BLUE);
    glPushMatrix();
    glTranslated(b.x, b.y, 0);
    drawSphere(0.2);
    glPopMatrix();
}


IK::IK(int numSegments, float length) : numSegments(numSegments), len(length)
{
    target = Vector(10, 10);
    base = Vector(0, 0);
    targetZ = 0;

    Segment seg = Segment(base.x, base.y, len);
    segments.push_back(seg);

    for (int i = 1; i < numSegments; i++) {
        Segment s = Segment(segments[i - 1], len);
        segments.push_back(s);
    }
}

IK::~IK()
{
}

void IK::setTarget(float x, float y, float z)
{
    target = Vector(x, y);
    targetZ = z;
}

void IK::update()
{
    segments[numSegments - 1].follow(target.x, target.y);
    segments[numSegments - 1].calculateB();

    for (int i = numSegments - 2; i >= 0; i--) {
        segments[i].follow(segments[i + 1]);
        segments[i].calculateB();
    }

    segments[0].setA(base);

    for (int i = 1; i < numSegments; i++) {
        segments[i].setA(segments[i - 1].b);
    }
}

void IK::show()
{
    update();

    glPushMatrix();

    for (Segment s : segments) {
        s.show();
    }

    glPopMatrix();
}

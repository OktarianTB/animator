// The sample robotarm model.  You should build a file
// very similar to this for when you make your model.
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning(disable : 4786)

#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "bitmap.h"
#include "particleSystem.h"

#include <FL/gl.h>
#include <stdlib.h>

#define M_DEFAULT 2.0f
#define M_OFFSET 3.0f
#define P_OFFSET 0.3f
#define MAX_VEL 200
#define MIN_STEP 0.1
#define PI 3.14159

// Colors
#define COLOR_BLACK		    0.00f, 0.00f, 0.00f
#define COLOR_WHITE		    1.00f, 1.00f, 1.00f
#define COLOR_RED		    1.00f, 0.00f, 0.00f
#define COLOR_GREEN		    0.00f, 1.00f, 0.00f
#define COLOR_YELLOW		1.00f, 1.00f, 0.00f
#define COLOR_BLUE		    0.00f, 0.00f, 1.00f
#define COLOR_CYAN		    0.00f, 1.00f, 1.00f
#define COLOR_PINK		    1.00f, 0.75f, 0.80f
#define COLOR_GREY		    0.88f, 0.95f, 0.95f
#define COLOR_LILAC         0.88f, 0.70f, 1.00f
#define COLOR_LIGHT_YELLOW  1.00f, 1.00f, 0.30f
#define COLOR_DARK_BLUE     0.00f, 0.00f, 0.20f
#define COLOR_SKY_BLUE      0.27f, 0.56f, 0.89f//0.53f, 0.81f, 0.92f


// Utilities
#define VAL(x) (ModelerApplication::Instance()->GetControlValue(x))
#define SET(x, value) (ModelerApplication::Instance()->SetControlValue(x, value))

// List of controls used to access the values of the controls from the UI
enum SteveControls
{ 
    XPOS=0, YPOS, ZPOS, 
	HEIGHT_NECK,
	ROTATE_HEAD_X, ROTATE_HEAD_Y, ROTATE_HEAD_Z, 
	ROTATE_RIGHT_ARM_SHOULDER, ROTATE_LEFT_ARM_SHOULDER, 
	ROTATE_RIGHT_ARM_ELBOW, ROTATE_LEFT_ARM_ELBOW,
	PARTICLE_COUNT, 
	NUMCONTROLS,
};

class Steve : public ModelerView 
{
public:
	Steve(int x, int y, int w, int h, char* label);
    virtual void draw();

	void drawFloor();

	void drawHead();
	void drawHeart();
	void drawRadioWaves();
	void drawTorus(float size);
	void drawTexturedBox(double x, double y, double z, int textureID, int textureWidth, int textureHeight, GLubyte* texture);

	void drawSteve();
	void drawLeftLeg();
	void drawRightLeg();
	void drawLeftArm();
	void drawRightArm();
	void drawFace();
private:
	int ironTextureWidth, ironTextureHeight;
	GLubyte* ironTexture = NULL;
	GLuint ironTextureID;
};

Steve::Steve(int x, int y, int w, int h, char* label) : ModelerView(x, y, w, h, label)
{
	if (ironTexture)
		delete[] ironTexture;

	ironTexture = readBMP("iron.bmp", ironTextureWidth, ironTextureHeight);
	glGenTextures(1, &ironTextureID);

}

ModelerView* createSteve(int x, int y, int w, int h, char *label)
{ 
    return new Steve(x,y,w,h,label);
}

void Steve::draw()
{
	// Make skyblue background
	glClearColor(COLOR_SKY_BLUE, 1.0);

	// This call takes care of a lot of the nasty projection matrix stuff
	ModelerView::draw();

	// Draw the floor
	drawFloor();

	// Draw the steve model
	drawSteve();
	
	endDraw();
}

void Steve::drawFloor()
{
	// draw the floor
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
	glTranslated(-5, 0, -5);
	drawBox(10, 0.01f, 10);
	glPopMatrix();
}

void Steve::drawSteve()
{
	// draw the steve model
	setAmbientColor(.1f, .1f, .1f);
	setDiffuseColor(COLOR_BLUE);

	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS) - 0.3, VAL(ZPOS));
		// START MAIN BODY
		glPushMatrix();
		glRotated(180, 0, 1, 0);
		glTranslated(-1, 2, -1);
		glScaled(2, 3, 1.7);
		drawTexturedBox(1, 1, 1, ironTextureID, ironTextureWidth, ironTextureHeight, ironTexture);
			// START HEART
			setDiffuseColor(COLOR_RED);
			glPushMatrix();
			glTranslated(0.5, 0.55, 1.02);
			glScaled(0.3, 0.2, 1);
			drawHeart();
			glPopMatrix();
			// END HEART

			// START HEAD
			drawHead();
			// END HEAD

		drawLeftArm();
		drawRightArm();
		drawRightLeg();
		drawLeftLeg();
		glPopMatrix();
		// END MAIN BODY
	glPopMatrix();
}

void Steve::drawHead()
{
	// Draw Neck
	setDiffuseColor(COLOR_DARK_BLUE);
	glPushMatrix();
	glRotated(-90, 1.0, 0.0, 0.0);
	glTranslated(0.5, -0.5, 0.80);
	glRotated(VAL(ROTATE_HEAD_X), 1, 0, 0);
	glRotated(4, 0, 1, 0);
	glRotated(16, 0, 0, 1);
	drawCylinder(VAL(HEIGHT_NECK), 0.24, 0.24);
	glTranslated(0, 0, VAL(HEIGHT_NECK) - 0.2);
	glRotated(90, 1.0, 0.0, 0.0);

		// START HEAD
		setDiffuseColor(COLOR_RED);

		glPushMatrix();
		glTranslated(-0.45, 0.19, -0.4);
	
		drawBox(0.9, 0.4, 0.8);

		// FACE MOOD
		drawFace();

			// START ANTENNA
			setDiffuseColor(COLOR_YELLOW);
			glPushMatrix();
			glRotated(-90, 1.0, 0.0, 0.0);
			glTranslated(0.7, -0.3, 0.3);
			drawCylinder(0.3, 0.05, 0.05);
			glRotated(90, 1.0, 0.0, 0.0);
			// Waves
			drawRadioWaves();
			glPopMatrix();
			// END ANTENNA
		glPopMatrix();
		// END HEAD

	glPopMatrix();
	// END NECK
}

void Steve::drawRadioWaves()
{
	setDiffuseColor(COLOR_GREY);
	// START RADIO WAVES
	glPushMatrix();

	glScaled(0.3, 0.2, 0.3);
	glTranslated(0, 2, 0);
	drawTorus(0.2);
	glTranslated(0, 0.8, 0);
	drawTorus(0.3);
	glTranslated(0, 0.8, 0);
	drawTorus(0.4);
	glTranslated(0, 0.8, 0);
	drawTorus(0.5);

	glPopMatrix();
	// END RADIO WAVES
}

void Steve::drawHeart()
{
	glPointSize(1);
	glColor3ub(255, 0, 0);  // Color Red
	glBegin(GL_QUAD_STRIP);

	for (float x = -1.139; x <= 1.139; x += 0.001)
	{
		float delta = cbrt(x * x) * cbrt(x * x) - 4 * x * x + 4;
		float y1 = (cbrt(x * x) + sqrt(delta)) / 2;
		float y2 = (cbrt(x * x) - sqrt(delta)) / 2;
		glNormal3d(0.0, 0.0, 1.0);
		glVertex2f(x, y1);
		glVertex2f(x, y2);
	}
	glEnd();
}


void Steve::drawFace()
{
	// START HERO MASK
	setDiffuseColor(COLOR_DARK_BLUE);
	glPushMatrix();
	glScaled(0.1, 0.06, 0.1);
	glTranslated(4.5, 3.5, 8.02);
	glPointSize(1);
	glBegin(GL_POLYGON);
	glNormal3d(0.0, 0.0, 1.0);
	glVertex2d(0.0, 1.0);
	glVertex2d(-4.0, 2.0);
	glVertex2d(-4.0, -1.0);
	glVertex2d(-2.0, -2.0);
	glVertex2d(0.0, -1.0);
	glVertex2d(2.0, -2.0);
	glVertex2d(4.0, -1.0);
	glVertex2d(4.0, 2.0);
	glEnd();
	glPopMatrix();
	// END HERO MASK

	// START EYES AND MOUTH
	setDiffuseColor(COLOR_WHITE);
	glPushMatrix();
	glScaled(0.1, 0.06, 0.1);
	glTranslated(4.5, 3.5, 8.04);
	glPointSize(1);
	glBegin(GL_POLYGON); // Left eye
	glNormal3d(0.0, 0.0, 1.0);
	glVertex2d(-1.5, 0.5);
	glVertex2d(-2.5, 0.5);
	glVertex2d(-2.5, -0.5);
	glVertex2d(-1.5, -0.5);
	glEnd();
	glBegin(GL_POLYGON); // Right eye
	glNormal3d(0.0, 0.0, 1.0);
	glVertex2d(1.5, 0.5);
	glVertex2d(2.5, 0.5);
	glVertex2d(2.5, -0.5);
	glVertex2d(1.5, -0.5);
	glEnd();
	glBegin(GL_POLYGON); // Mouth
	glNormal3d(0.0, 0.0, 1.0);
	glVertex2d(-1.0, -2.0);
	glVertex2d(0.0, -3.0);
	glVertex2d(1.0, -2.0);
	glEnd();
	glPopMatrix();
	// END EYES AND MOUTH
}

void Steve::drawLeftLeg()
{
	// START LEFT LEG
	setDiffuseColor(COLOR_DARK_BLUE);
	glPushMatrix();
	glRotated(-60, 1.0, 0.0, 0.0);
	glTranslated(0.12, -1.2, 0.24);
	drawBox(0.3, 0.8, 0.25);
	glRotated(60, 1.0, 0.0, 0.0);

		// START KNEE
		glPushMatrix();
		glRotated(-90, 0.0, 1.0, 0);
		glScaled(0.5, 0.33, 0.58);
		glScaled(0.8, 0.8, 1.29);
		glTranslated(0.2, 0.43, -0.4);
		drawCylinder(0.4, 0.4, 0.4);
		glRotated(90, 0.0, 1.0, 0.0);
			// START FOOT	
			glPushMatrix();
			glRotated(90, 1.0, 0.0, 0.0);
			glTranslated(-0.4, -0.32, 0);
			drawBox(0.4, 0.7, 1.05);
			glRotated(-90, 1.0, 0.0, 0.0);
			glPopMatrix();
			// END FOOT
		glPopMatrix();
		// END KNEE
	glPopMatrix();
	// END LEFT LEG
}

void Steve::drawRightLeg()
{
	// START RIGHT LEG
	setDiffuseColor(COLOR_DARK_BLUE);
	glPushMatrix();
	glRotated(30, 1.0, 0.0, 0.0);
	glTranslated(0.6, -0.2, 0.15);
	drawBox(0.3, 0.5, 0.3);
	glRotated(-30, 1.0, 0.0, 0.0);
		// START KNEE
		glPushMatrix();
		glRotated(-90, 0.0, 1.0, 0);
		glScaled(0.5, 0.33, 0.58);
		glScaled(0.8, 0.8, 1.29);
		glTranslated(0.35, -0.2, -0.4);
		drawCylinder(0.4, 0.4, 0.4);
		glRotated(90, 0.0, 1.0, 0.0);
			// START FOOT	
			glPushMatrix();
			glRotated(90, 1.0, 0.0, 0.0);
			glTranslated(-0.4, -0.32, 0);
			drawBox(0.4, 0.7, 1.0);
			glRotated(-90, 1.0, 0.0, 0.0);
			glPopMatrix();
			// END FOOT
		glPopMatrix();
		// END KNEE
	glPopMatrix();
	// END RIGHT LEG
}

void Steve::drawRightArm()
{
	// START RIGHT ARM
	glPushMatrix();
	glRotated(-90, 0.0, 1.0, 0);
	glScaled(0.5, 0.33, 0.75);
	// back, down
	glTranslated(1, 2.2, -1.35);
	glRotated(-50, 1, 0, 0);
	glRotated(180, 0, 1, 0);
	glRotated(180, 1, 0, 0);
	glRotated(180, 0, 1, 0);
	glRotated(-80, 1, 0, 0);
	drawCylinder(0.1, 0.1, 0.1);
	glScaled(1.1, 3, 1.33);
		// START UPPER ARM
		glPushMatrix();
		setDiffuseColor(COLOR_DARK_BLUE);
		glTranslated(-0.25, -0.25, -0.1);
		drawBox(0.6, 0.3, 0.3);
			// START ELBOW
			glPushMatrix();
			glRotated(-90, 0.0, 1.0, 0);
			glScaled(0.5, 0.33, 1);
			glScaled(0.8, 0.4, 1.5);
			glTranslated(0.4, 0, -0.4);
			drawCylinder(0.4, 0.4, 0.4);
			glRotated(90, 0.0, 1.0, 0.0);
				// START HAND
				glPushMatrix();
				glRotated(VAL(ROTATE_RIGHT_ARM_ELBOW), 1, 0, 0);
				glTranslated(-0.4, -0.38, 0);
				drawBox(0.4, 0.7, 2.2);
				glPopMatrix();
				// END HAND
			glPopMatrix();
			// END ELBOW
		glPopMatrix();
		// END UPPER ARM
	glPopMatrix();
	// END RIGHT ARM
}

void Steve::drawLeftArm()
{
	// START LEFT ARM
	glPushMatrix();
	glRotated(-90, 0.0, 1.0, 0);
	glScaled(0.5, 0.33, 0.75);
	glTranslated(1, 2.2, 0);
	glRotated(-50, 1, 0, 0);
	glRotated(180, 0, 1, 0);
	drawCylinder(0.1, 0.1, 0.1);
	glScaled(1.1, 3, 1.33);
		// START UPPER ARM
		glPushMatrix();
		setDiffuseColor(COLOR_DARK_BLUE);
		glTranslated(-0.25, -0.25, -0.1);
		drawBox(0.6, 0.3, 0.3);
			// START ELBOW
			glPushMatrix();
			glRotated(-90, 0.0, 1.0, 0);
			glScaled(0.5, 0.33, 1);
			glScaled(0.8, 0.4, 1.5);
			glTranslated(0.4, 0, -0.4);
			drawCylinder(0.4, 0.4, 0.4);
			glRotated(90, 0.0, 1.0, 0.0);
				// START HAND
				glPushMatrix();
				glRotated(VAL(ROTATE_LEFT_ARM_ELBOW), 1, 0, 0);
				glTranslated(-0.4, -0.38, 0);
				drawBox(0.4, 0.7, 2.2);
				glPopMatrix();
				// END HAND
			glPopMatrix();
			// END ELBOW
		glPopMatrix();
		// END UPPER ARM
	glPopMatrix();
	// END LEFT ARM
}

void Steve::drawTorus(float size)
{
	glScaled(size, size, size);

	double TWOPI = 2 * PI;
	double torusRadius = 0.1;
	double radius = 1;
	int numc = 10, numt = 10;
	for (int i = 0; i < numc; i++) {
		glBegin(GL_QUAD_STRIP);
		for (int j = 0; j <= numt; j++) {
			for (int k = 1; k >= 0; k--) {

				double s = (i + k) % numc + 0.5;
				double t = j % numt;

				double x = (radius + torusRadius * cos(s * TWOPI / numc)) * cos(t * TWOPI / numt);
				double y = (radius + torusRadius * cos(s * TWOPI / numc)) * sin(t * TWOPI / numt);
				double z = torusRadius * sin(s * TWOPI / numc);

				glVertex3d(2 * x, 2 * z, 2 * y);
			}
		}
		glEnd();
	}

	glScaled(1 / size, 1 / size, 1 / size);
}

void Steve::drawTexturedBox(double x, double y, double z, int textureID, int textureWidth, int textureHeight, GLubyte* texture)
{
	glBindTexture(GL_TEXTURE_2D, textureID);

	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, textureWidth, textureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_TEXTURE_2D);
	setDiffuseColor(COLOR_WHITE);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glScaled(x, y, z);
	glBegin(GL_QUADS);

	glNormal3b(0, 0, -1);
	glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3d(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3d(1.0, 1.0, 0.0);
	glTexCoord2f(0.0, 1.0); glVertex3d(1.0, 0.0, 0.0);

	glNormal3b(0, -1, 0);
	glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3d(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3d(1.0, 0.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex3d(0.0, 0.0, 1.0);

	glNormal3b(-1, 0, 0);
	glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3d(0.0, 0.0, 1.0);
	glTexCoord2f(1.0, 1.0); glVertex3d(0.0, 1.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex3d(0.0, 1.0, 0.0);

	glNormal3b(0, 0, +1);
	glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 0.0, 1.0);
	glTexCoord2f(1.0, 0.0); glVertex3d(1.0, 0.0, 1.0);
	glTexCoord2f(1.0, 1.0); glVertex3d(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex3d(0.0, 1.0, 1.0);

	// Top surface
	glNormal3b(0, +1, 0);
	glTexCoord2f(0.0, 0.0); glVertex3d(0.0, 1.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3d(0.0, 1.0, 1.0);
	glTexCoord2f(1.0, 1.0); glVertex3d(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex3d(1.0, 1.0, 0.0);

	glNormal3b(+1, 0, 0);
	glTexCoord2f(0.0, 0.0); glVertex3d(1.0, 0.0, 0.0);
	glTexCoord2f(1.0, 0.0); glVertex3d(1.0, 1.0, 0.0);
	glTexCoord2f(1.0, 1.0); glVertex3d(1.0, 1.0, 1.0);
	glTexCoord2f(0.0, 1.0); glVertex3d(1.0, 0.0, 1.0);

	glEnd();

	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_TEXTURE_2D);
}

int main()
{
    ModelerControl controls[NUMCONTROLS];
	controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
	controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
	controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
	controls[HEIGHT_NECK] = ModelerControl("Neck Height", 0.2, 1, 0.05f, 0.4);
	controls[ROTATE_HEAD_X] = ModelerControl("Rotate Head X", -60, 60, 1, 0);
	controls[ROTATE_HEAD_Y] = ModelerControl("Rotate Head Y", -60, 60, 1, 0);
	controls[ROTATE_HEAD_Z] = ModelerControl("Rotate Head Z", -135, 135, 1, 0);
	controls[ROTATE_RIGHT_ARM_SHOULDER] = ModelerControl("Rotate Right Arm Shoulder", -135, 135, 1, 0);
	controls[ROTATE_RIGHT_ARM_ELBOW] = ModelerControl("Rotate Right Arm Elbow", -20, 50, 1, 30);
	controls[ROTATE_LEFT_ARM_SHOULDER] = ModelerControl("Rotate Left Arm Shoulder", -135, 135, 1, 0);
	controls[ROTATE_LEFT_ARM_ELBOW] = ModelerControl("Rotate Left Arm Elbow", -20, 50, 1, 30);
    


	// You should create a ParticleSystem object ps here and then
	// call ModelerApplication::Instance()->SetParticleSystem(ps)
	// to hook it up to the animator interface.

    ModelerApplication::Instance()->Init(&createSteve, controls, NUMCONTROLS);

    return ModelerApplication::Instance()->Run();
}

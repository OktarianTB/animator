// The sample robotarm model.  You should build a file
// very similar to this for when you make your model.
#pragma warning (disable : 4305)
#pragma warning (disable : 4244)
#pragma warning(disable : 4786)

#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include "modelerui.h"
#include "bitmap.h"
#include "particleSystem.h"
#include "camera.h"
#include "IK.h"
#include "HeightMap.h"

#include <FL/gl.h>
#include <stdlib.h>

#define M_DEFAULT 2.0f
#define M_OFFSET 3.0f
#define P_OFFSET 0.3f
#define MAX_VEL 200
#define MIN_STEP 0.1
#define PI 3.14159

#define deg2rad(x) (x * (PI / 180))
#define rad2deg(x) ((x * 180.0) / PI)

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
	IK_X, IK_Y, IK_Z,
	PARTICLE_COUNT, 
	NUMCONTROLS,
};

class Steve : public ModelerView 
{
public:
	Steve(int x, int y, int w, int h, char* label);
    virtual void draw();

	void drawFloor();
	void drawSkybox();

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

	// Particle system
	Mat4d getModelViewMatrix();
	void drawParticleSystem(Mat4d CameraMatrix, int particle_count);

	// Inverse kinematics
	void drawMechanicalArm();
private:
	IK* ik;
	HeightMap* heightMap;

	int ironTextureWidth, ironTextureHeight;
	GLubyte* ironTexture = NULL;
	GLuint ironTextureID;
	
	int sbWidth0, sbWidth1, sbWidth2, sbWidth3, sbWidth4, sbWidth5;
	int sbHeight0, sbHeight1, sbHeight2, sbHeight3, sbHeight4, sbHeight5;

	GLubyte* sbTexture0 = NULL;
	GLubyte* sbTexture1 = NULL;
	GLubyte* sbTexture2 = NULL;
	GLubyte* sbTexture3 = NULL;
	GLubyte* sbTexture4 = NULL;
	GLubyte* sbTexture5 = NULL;

	GLuint sbTextureID0, sbTextureID1, sbTextureID2, sbTextureID3, sbTextureID4, sbTextureID5;
};

Steve::Steve(int x, int y, int w, int h, char* label) : ModelerView(x, y, w, h, label)
{
	ik = new IK(10, 1);
	heightMap = new HeightMap("heightmap2.bmp");

	if (ironTexture)
		delete[] ironTexture;

	ironTexture = readBMP("iron.bmp", ironTextureWidth, ironTextureHeight);
	glGenTextures(1, &ironTextureID);
	
	// front
	if (sbTexture0)
		delete[] sbTexture0;
	sbTexture0 = readBMP("front.bmp", sbWidth0, sbHeight0);
	glGenTextures(1, &sbTextureID0);

	// left
	if (sbTexture1)
		delete[] sbTexture1;
	sbTexture1 = readBMP("left.bmp", sbWidth1, sbHeight1);
	glGenTextures(1, &sbTextureID1);

	// back
	if (sbTexture2)
		delete[] sbTexture2;
	sbTexture2 = readBMP("back.bmp", sbWidth2, sbHeight2);
	glGenTextures(1, &sbTextureID2);

	// right
	if (sbTexture3)
		delete[] sbTexture3;
	sbTexture3 = readBMP("right.bmp", sbWidth3, sbHeight3);
	glGenTextures(1, &sbTextureID3);

	// top
	if (sbTexture4)
		delete[] sbTexture4;
	sbTexture4 = readBMP("top.bmp", sbWidth4, sbHeight4);
	glGenTextures(1, &sbTextureID4);

	// bottom
	if (sbTexture5)
		delete[] sbTexture5;
	sbTexture5 = readBMP("bottom.bmp", sbWidth5, sbHeight5);
	glGenTextures(1, &sbTextureID5);
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

	Mat4d CameraMatrix = getModelViewMatrix();

	// Draw skybox
	if(ModelerUI::getSkyboxActive())
		drawSkybox();

	// Draw the floor
	if (!ModelerUI::getHeightMapActive())
		drawFloor();

	if (ModelerUI::getIKActive())
		drawMechanicalArm();
	else if (ModelerUI::getHeightMapActive())
		heightMap->render();
	else
		// Draw the steve model
		drawSteve();
	
	// Draw the particle system
	//drawParticleSystem(CameraMatrix, VAL(PARTICLE_COUNT));

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

void Steve::drawSkybox()
{
	// Store the current matrix
	glPushMatrix();
	
	// Center skybox around the camera
	Vec3f pos = m_camera->getCameraPosition();
	glTranslated(pos[0], pos[1], pos[2]);

	// Enable/Disable features
	glPushAttrib(GL_ENABLE_BIT);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(GL_BLEND);

	// Just in case we set all vertices to white.
	glColor4f(1, 1, 1, 1);

	// Scale appropriately
	glScaled(3, 3, 3);

	// Parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// Render the front quad
	glBindTexture(GL_TEXTURE_2D, sbTextureID0);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sbWidth0, sbHeight0, 0, GL_RGB, GL_UNSIGNED_BYTE, sbTexture0);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 1); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(0, 1); glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();

	// Render the left quad
	glBindTexture(GL_TEXTURE_2D, sbTextureID1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sbWidth1, sbHeight1, 0, GL_RGB, GL_UNSIGNED_BYTE, sbTexture1);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 0); glVertex3f(0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, 0.5f, -0.5f);
	glTexCoord2f(0, 1); glVertex3f(0.5f, 0.5f, 0.5f);
	glEnd();

	// Render the back quad
	glBindTexture(GL_TEXTURE_2D, sbTextureID2);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sbWidth2, sbHeight2, 0, GL_RGB, GL_UNSIGNED_BYTE, sbTexture2);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 0); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
	glEnd();

	// Render the right quad
	glBindTexture(GL_TEXTURE_2D, ironTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sbWidth3, sbHeight3, 0, GL_RGB, GL_UNSIGNED_BYTE, sbTexture3);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(1, 0); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 1); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f, -0.5f);
	glEnd();

	// Render the top quad
	glBindTexture(GL_TEXTURE_2D, ironTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sbWidth4, sbHeight4, 0, GL_RGB, GL_UNSIGNED_BYTE, sbTexture4);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 1); glVertex3f(-0.5f, 0.5f, -0.5f);
	glTexCoord2f(0, 0); glVertex3f(-0.5f, 0.5f, 0.5f);
	glTexCoord2f(1, 0); glVertex3f(0.5f, 0.5f, 0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, 0.5f, -0.5f);
	glEnd();

	// Render the bottom quad
	glBindTexture(GL_TEXTURE_2D, ironTextureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, sbWidth5, sbHeight5, 0, GL_RGB, GL_UNSIGNED_BYTE, sbTexture5);

	glBegin(GL_QUADS);
	glTexCoord2f(0, 0); glVertex3f(-0.5f, -0.5f, -0.5f);
	glTexCoord2f(0, 1); glVertex3f(-0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 1); glVertex3f(0.5f, -0.5f, 0.5f);
	glTexCoord2f(1, 0); glVertex3f(0.5f, -0.5f, -0.5f);
	glEnd();

	// Restore enable bits and matrix
	glPopAttrib();
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
	glRotated(VAL(ROTATE_HEAD_Y), 0, 1, 0);
	glRotated(VAL(ROTATE_HEAD_Z), 0, 0, 1);
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
			// Particles
			Mat4d CameraMatrix = getModelViewMatrix();
			drawParticleSystem(CameraMatrix, VAL(PARTICLE_COUNT));
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

void Steve::drawMechanicalArm()
{
	ik->show(VAL(IK_X), VAL(IK_Y), VAL(IK_Z));

	// Draw target ball
	setDiffuseColor(COLOR_RED);
	glPushMatrix();
	glTranslated(VAL(IK_X), VAL(IK_Y), VAL(IK_Z));
	drawSphere(0.5);
	glPopMatrix();
}

Mat4d Steve::getModelViewMatrix() {
	/**************************
	**
	** GET THE OPENGL MODELVIEW MATRIX
	**
	** Since OpenGL stores it's matricies in
	** column major order and our library
	** use row major order, we will need to
	** transpose what OpenGL gives us before returning.
	**
	*******************************/
	GLdouble m[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, m);
	Mat4d matMV(m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]);

	return matMV.transpose();
}

void Steve::drawParticleSystem(Mat4d CameraMatrix, int particle_count)
{
	// Get the current MODELVIEW matrix.
	// "Undo" the camera transforms from the MODELVIEW matrix
	// by multiplying Inverse(CameraTransforms) * CurrentModelViewMatrix.
	// Store the result of this in a local variable called WorldMatrix.
	Mat4d ModelMatrix = getModelViewMatrix();
	Mat4d WorldMatrix = CameraMatrix.inverse() * ModelMatrix;

	Vec4d pos = WorldMatrix * Vec4d(0.0, 0.0, 0.0, 0.0);
	ParticleSystem* ps = ModelerApplication::Instance()->GetParticleSystem();
	ps->spawnParticles(Vec3d(pos[0], pos[1], pos[2]), particle_count);
	ps->computeForcesAndUpdateParticles(t);
	ps->drawParticles(t);
}

int main()
{
    ModelerControl controls[NUMCONTROLS];
	controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1f, 0);
	controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1f, 0);
	controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1f, 0);
	controls[HEIGHT_NECK] = ModelerControl("Neck Height", 0.2, 1, 0.05f, 0.4);
	controls[ROTATE_HEAD_X] = ModelerControl("Rotate Head X", -30, 30, 1, 0);
	controls[ROTATE_HEAD_Y] = ModelerControl("Rotate Head Y", -30, 30, 1, 0);
	controls[ROTATE_HEAD_Z] = ModelerControl("Rotate Head Z", -60, 60, 1, 0);
	controls[ROTATE_RIGHT_ARM_SHOULDER] = ModelerControl("Rotate Right Arm Shoulder", -135, 135, 1, 0);
	controls[ROTATE_RIGHT_ARM_ELBOW] = ModelerControl("Rotate Right Arm Elbow", -20, 50, 1, 30);
	controls[ROTATE_LEFT_ARM_SHOULDER] = ModelerControl("Rotate Left Arm Shoulder", -135, 135, 1, 0);
	controls[ROTATE_LEFT_ARM_ELBOW] = ModelerControl("Rotate Left Arm Elbow", -20, 50, 1, 30);
	controls[IK_X] = ModelerControl("IK: X", -10, 10, 0.1, 2.5);
	controls[IK_Y] = ModelerControl("IK: Y", -10, 10, 0.1, 2.5);
	controls[IK_Z] = ModelerControl("IK: Z", -10, 10, 0.1, 0.5);
	controls[PARTICLE_COUNT] = ModelerControl("Particles number", 0, 100, 1, 15);

	// You should create a ParticleSystem object ps here and then
	// call ModelerApplication::Instance()->SetParticleSystem(ps)
	// to hook it up to the animator interface.
	ParticleSystem* ps = new ParticleSystem(Vec3d(0, -4, 0), 0.2);
	ModelerApplication::Instance()->SetParticleSystem(ps);

    ModelerApplication::Instance()->Init(&createSteve, controls, NUMCONTROLS);

    return ModelerApplication::Instance()->Run();
}

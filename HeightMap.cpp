#include "HeightMap.h"

HeightMap::HeightMap(const char* fileName)
{
	if ((bitmap = readBMP(fileName, width, height)) == nullptr)
	{
		fl_alert("Can't load bitmap file");
	}
	else
	{
		glGenTextures(1, &textureID);

		for (int y = 0; y < height; y++)
		{
			std::vector<float> tmp;
			for (int x = 0; x < width; x++)
			{
				GLubyte color[3];
				memcpy(color, (GLubyte*)(bitmap + 3 * (y * width + x)), 3);

				tmp.push_back((float)color[0] / 255.0);
			}
			heights.push_back(tmp);
		}
	}

}

void HeightMap::render()
{
	glPushMatrix();
	glTranslated(-5, 0, -5);
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < heights.size() - 1; i++)
	{
		for (int j = 0; j < heights[0].size() - 1; j++)
		{
			glBegin(GL_TRIANGLE_STRIP);
			setDiffuseColor(heights[i][j], heights[i][j], heights[i][j]);
			glVertex3f(i * size, heights[i][j] * h, j * size);
			glVertex3f((i + 1) * size, heights[i + 1][j] * h, j * size);
			glVertex3f(i * size, heights[i][j + 1] * h, (j + 1) * size);
			glVertex3f((i + 1) * size, heights[i + 1][j + 1] * h, (j + 1) * size);
			glEnd();
		}
	}	
	glPopMatrix();
}



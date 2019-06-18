#include "..\include\Terrain.h"






Terrain::Terrain(const char * filename, GLfloat x, GLfloat y, GLfloat z)
	:filename{ filename }
	,Figura(x, y, z)
{
}

Terrain::~Terrain()
{
}

void Terrain::draw()
{
	objl::Loader floor;

	glRotatef(0, 0, 0, 0);
	glScalef(50, 50, 50);		//skalowanie powierzchni marsa

	float x_position = 0;
	float y_position = -0.05;
	float z_position = 0;

	if (floor.LoadFile(filename))
	{
		for (int i = 0; i < floor.LoadedMeshes.size(); i++)
		{
			objl::Mesh curMesh = floor.LoadedMeshes[i];

			for (int j = 0; j < curMesh.Indices.size(); j += 3)
			{
				glBegin(GL_TRIANGLES);
				//double random = 0.5f + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (0.55f - 0.5f)));
				glColor3fv(color);
				//if (i == 0) glColor3f(0, 0, 1);
				
				if (!strcmp(filename,"objects/mars.obj"))
					glTexCoord2d(0, 0);
				else
					glTexCoord2d(curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j]].TextureCoordinate.Y);
				glVertex3f(
					xpos + curMesh.Vertices[curMesh.Indices[j]].Position.X + x_position,
					ypos + curMesh.Vertices[curMesh.Indices[j]].Position.Y + y_position,
					zpos + curMesh.Vertices[curMesh.Indices[j]].Position.Z + z_position
				); 

				if (!strcmp(filename, "objects/mars.obj"))
					glTexCoord2d(0, 1);
				else
					glTexCoord2d(curMesh.Vertices[curMesh.Indices[j+1]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j+1]].TextureCoordinate.Y);
				
				glVertex3f(
					xpos + curMesh.Vertices[curMesh.Indices[j + 1]].Position.X + x_position,
					ypos + curMesh.Vertices[curMesh.Indices[j + 1]].Position.Y + y_position,
					zpos + curMesh.Vertices[curMesh.Indices[j + 1]].Position.Z + z_position
				);

				if (!strcmp(filename, "objects/mars.obj"))
					glTexCoord2d(1, 0);
				else
					glTexCoord2d(curMesh.Vertices[curMesh.Indices[j+2]].TextureCoordinate.X, curMesh.Vertices[curMesh.Indices[j+2]].TextureCoordinate.Y);
				
				glVertex3f(
					xpos + curMesh.Vertices[curMesh.Indices[j + 2]].Position.X + x_position,
					ypos + curMesh.Vertices[curMesh.Indices[j + 2]].Position.Y + y_position,
					zpos + curMesh.Vertices[curMesh.Indices[j + 2]].Position.Z + z_position
				);
				glEnd();
			}
		}
	}

}

#pragma once

#include<vector>
#include<fstream>	
#include<windows.h>
#include<DirectXMath.h>
using namespace DirectX;
using namespace std;

struct VertexFormat
{
	// Vertex
	union
	{
		float verts[3];

		struct
		{
			float x, y, z;
		};
	};

	// UV's
	union
	{
		float UVS[2];

		struct
		{
			float u, v;
		};
	};

	// Normals
	union
	{
		float normals[3];

		struct
		{
			float norm1, norm2, norm3;
		};
	};
};

class ModelLoader
{
public:

	ModelLoader();
	ModelLoader(const ModelLoader&);
	~ModelLoader();

	void LoadOBJModel(const char* fileName, vector<VertexFormat>&, vector<unsigned int>&);
};
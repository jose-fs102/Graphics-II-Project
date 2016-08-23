#include "ModelLoader.h"


ModelLoader::ModelLoader()
{

}

ModelLoader::ModelLoader(const ModelLoader&)
{

}

ModelLoader::~ModelLoader()
{

}

void ModelLoader::LoadOBJModel(const char * fileName, vector<VertexFormat>& vertices, vector<unsigned int>& indices)
{
	FILE* file;

	fopen_s(&file, fileName, "r");

	if (file == NULL)
		return;

	vector<unsigned int> numVertices, numUVS, numNormals;

	vector<XMFLOAT3> temp_vertices;
	vector<XMFLOAT2> temp_uvs;
	vector<XMFLOAT3> temp_normals;

	while (true)
	{
		char lineHeader[128];
		std::string readLine;
		int res = fscanf(file, "%s", lineHeader);
		readLine = lineHeader;
		if (res == EOF)
			break;

		if (readLine == "v")
		{
			XMFLOAT3 vertexPos;

			fscanf(file, "%f %f %f\n", &vertexPos.x, &vertexPos.y, &vertexPos.z);
			temp_vertices.push_back(vertexPos);
		}
		else if (readLine == "vt")
		{
			XMFLOAT2 uvs;

			fscanf(file, "%f %f\n", &uvs.x, &uvs.y);
			uvs.y = 1.0f - uvs.y;

			temp_uvs.push_back(uvs);
		}
		else if (readLine == "vn")
		{
			XMFLOAT3 normals;

			fscanf(file, "%f %f %f\n", &normals.x, &normals.y, &normals.z);
			temp_normals.push_back(normals);
		}
		else if (readLine == "f")
		{
			unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];

			int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
				&vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

			numVertices.push_back(vertexIndex[0]);
			numVertices.push_back(vertexIndex[1]);
			numVertices.push_back(vertexIndex[2]);

			numUVS.push_back(uvIndex[0]);
			numUVS.push_back(uvIndex[1]);
			numUVS.push_back(uvIndex[2]);

			numNormals.push_back(normalIndex[0]);
			numNormals.push_back(normalIndex[1]);
			numNormals.push_back(normalIndex[2]);
		}
	}

	for (unsigned int curr = 0; curr < numVertices.size(); curr++)
	{
		VertexFormat vertex;

		vertex.x = temp_vertices[numVertices[curr] - 1].x;
		vertex.y = temp_vertices[numVertices[curr] - 1].y;
		vertex.z = temp_vertices[numVertices[curr] - 1].z;

		vertex.u = temp_uvs[numUVS[curr] - 1].x;
		vertex.v = temp_uvs[numUVS[curr] - 1].y;

		vertex.norm1 = temp_normals[numNormals[curr] - 1].x;
		vertex.norm2 = temp_normals[numNormals[curr] - 1].y;
		vertex.norm3 = temp_normals[numNormals[curr] - 1].z;

		vertices.push_back(vertex);
		indices.push_back(curr);
	}
}
////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx10math.h>
#include <fstream>
#include "textureclass.h"
#include "TextureArray.h"
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		D3DXVECTOR3 position;
	    D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 binormal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
		float tx, ty, tz;
		float bx, by, bz;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};

public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename1, WCHAR* textureFilename2, WCHAR* textureFilename3);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	ID3D11ShaderResourceView** GetTextureArray();

	ID3D11ShaderResourceView* getTexture1() { return m_textureArray->GetTextureArray()[0]; };
	ID3D11ShaderResourceView* getTexture2() { return m_textureArray->GetTextureArray()[1]; };
	ID3D11ShaderResourceView* getTexture3() { return m_textureArray->GetTextureArray()[2]; };

	int GetIndexCount();


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTextures(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2, WCHAR* filename3);
	void ReleaseTextures();

	bool LoadModel(char*);
	void ReleaseModel();

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3, VectorType& tangent, VectorType& binormal);
	void CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal);

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	ModelType* m_model;
	TextureArray* m_textureArray;
};

#endif
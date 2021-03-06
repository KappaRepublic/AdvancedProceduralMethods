////////////////////////////////////////////////////////////////////////////////
// Filename: TextureArray.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _TEXTUREARRAY_H_
#define _TEXTUREARRAY_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <d3dx11tex.h>


////////////////////////////////////////////////////////////////////////////////
// Class name: TextureArray
////////////////////////////////////////////////////////////////////////////////
class TextureArray
{
public:
	TextureArray();
	TextureArray(const TextureArray&);
	~TextureArray();

	bool Initialize(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2, WCHAR* filename3);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView* m_textures[3];
};

#endif
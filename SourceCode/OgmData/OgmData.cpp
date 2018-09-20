//==========================
//===【OgmData.cpp】
//==========================
//    Ogmデータを保持します。
//    (OGL_D3D11で用いるモデルデータ)
//==========================

//=====================//
//===【インクルード記述】 ===//
//=====================//
#include	"OgmData.h"
#include<fstream>

OGL::OgmData::OgmData()
{
}

OGL::OgmData::~OgmData()
{
}


bool	OGL::OgmData::ImportFromModelData(PmxData* modelData)
{
	if (modelData->m_pmxVersion == 0)
	{
		MessageBox(NULL, TEXT("PmxDataにモデルが読み込まれていません。"), TEXT("Err"), MB_OK);
		return false;
	}

	//インデックスデータロード
	for (int i = 0; i < modelData->m_vertexIndex.size(); i++)
	{
		m_vertexIndex.push_back((int)modelData->m_vertexIndex[i].x);
		m_vertexIndex.push_back((int)modelData->m_vertexIndex[i].y);
		m_vertexIndex.push_back((int)modelData->m_vertexIndex[i].z);
	}

	//頂点データをロード
	for (int i = 0; i < modelData->m_vertexData.size(); i++)
	{
		m_vertexData.pos.push_back(D3DXVECTOR4(modelData->m_vertexData[i].pos, 1));
		m_vertexData.normal.push_back(D3DXVECTOR4(modelData->m_vertexData[i].normal, 1));
		m_vertexData.uv.push_back(D3DXVECTOR4(modelData->m_vertexData[i].texUV.x, modelData->m_vertexData[i].texUV.y, 0, 0));
		m_vertexData.boneIndex.push_back(D3DXVECTOR4(modelData->m_vertexData[i].boneIndex[0], modelData->m_vertexData[i].boneIndex[1], modelData->m_vertexData[i].boneIndex[2], modelData->m_vertexData[i].boneIndex[3]));
		m_vertexData.boneWeight.push_back(D3DXVECTOR4(modelData->m_vertexData[i].boneWeight[0], modelData->m_vertexData[i].boneWeight[1], modelData->m_vertexData[i].boneWeight[2], modelData->m_vertexData[i].boneWeight[3]));
	}

	//テクスチャ名をロード
	for (int i = 0; i < modelData->m_texturePass.size(); i++)
	{
		m_textureName.push_back(modelData->m_texturePass[i]);
	}
	m_textureName.push_back("shareTex/toon0.bmp");
	m_textureName.push_back("shareTex/toon01.bmp");
	m_textureName.push_back("shareTex/toon02.bmp");
	m_textureName.push_back("shareTex/toon03.bmp");
	m_textureName.push_back("shareTex/toon04.bmp");
	m_textureName.push_back("shareTex/toon05.bmp");
	m_textureName.push_back("shareTex/toon06.bmp");
	m_textureName.push_back("shareTex/toon07.bmp");
	m_textureName.push_back("shareTex/toon08.bmp");
	m_textureName.push_back("shareTex/toon09.bmp");
	m_textureName.push_back("shareTex/toon10.bmp");
	m_textureName.push_back("shareTex/blackTexture.png");
	m_textureName.push_back("shareTex/whiteTexture.png");
	//マテリアル情報をロード
	for (int i = 0; i < modelData->m_materialData.size(); i++)
	{
		m_materialRefVertexIndexCount.push_back(modelData->m_materialData[i].referenceVertexIndexCount);
		T_TextureIndex temp;
		if (modelData->m_materialData[i].diffuseTextureIndex == 255)
		{
			temp.diffuse = m_textureName.size() - 1;
		}
		else
		{
			temp.diffuse = modelData->m_materialData[i].diffuseTextureIndex;
		}
		if (modelData->m_materialData[i].toonTextureShareFlg == true)
		{
			temp.toon = m_textureName.size() - 12 + modelData->m_materialData[i].toonTextureShareIndex;
		}
		else
		{
			if (modelData->m_materialData[i].toonTextureIndex == 255)
			{
				temp.toon = m_textureName.size() - 1;
			}
			else
			{
				temp.toon = modelData->m_materialData[i].toonTextureIndex;
			}
		}

		switch (modelData->m_materialData[i].sphereType)
		{
		case 0:
			temp.addSphere = m_textureName.size() - 2;
			temp.mulSphere = m_textureName.size() - 1;
			break;
		case 1:
			temp.addSphere = m_textureName.size() - 2;
			temp.mulSphere = modelData->m_materialData[i].sphereTextureIndex;
			break;
		case 2:
			temp.addSphere = modelData->m_materialData[i].sphereTextureIndex;
			temp.mulSphere = m_textureName.size() - 1;
			break;
		case 3:
			MessageBox(NULL, TEXT("特殊スフィアマップは使えません"), TEXT("Err"), MB_OK);
			return false;
			break;
		}
		m_textureIndex.push_back(temp);
		T_MaterialData mat_temp;
		mat_temp.diffuse = modelData->m_materialData[i].diffuse;
		mat_temp.ambient = D3DXVECTOR4(modelData->m_materialData[i].ambient,1);
		mat_temp.specular = D3DXVECTOR4(modelData->m_materialData[i].specular, modelData->m_materialData[i].specularPower);
		m_materialData.push_back(mat_temp);
	}

	return true;
}

bool	OGL::OgmData::ImportFromModelFile(std::string fileName)
{
	std::ifstream  ogmFile(fileName, std::ios_base::in | std::ios_base::binary);
	if (!ogmFile)
	{
		MessageBox(NULL, TEXT("ファイルを読み込めていません。"), TEXT("Err"), MB_OK);
		return false;
	}
	int temp = 0;
	ogmFile.read((char*)&temp, sizeof(int));
	m_vertexIndex.resize(temp);
	ogmFile.read((char*)&m_vertexIndex[0], sizeof(int)*temp);

	ogmFile.read((char*)&temp, sizeof(int));
	m_vertexData.pos.resize(temp);
	ogmFile.read((char*)&m_vertexData.pos[0], sizeof(D3DXVECTOR4)*temp);
	m_vertexData.normal.resize(temp);
	ogmFile.read((char*)&m_vertexData.normal[0], sizeof(D3DXVECTOR4)*temp);
	m_vertexData.uv.resize(temp);
	ogmFile.read((char*)&m_vertexData.uv[0], sizeof(D3DXVECTOR4)*temp);
	m_vertexData.boneIndex.resize(temp);
	ogmFile.read((char*)&m_vertexData.boneIndex[0], sizeof(D3DXVECTOR4)*temp);
	m_vertexData.boneWeight.resize(temp);
	ogmFile.read((char*)&m_vertexData.boneWeight[0], sizeof(D3DXVECTOR4)*temp);

	ogmFile.read((char*)&temp, sizeof(int));
	for (int i = 0; i < temp; i++)
	{
		std::string::size_type strLen;
		ogmFile.read((char*)&strLen, sizeof(std::string::size_type));
		char* name = (char*)std::malloc(strLen + 1);
		ogmFile.read(name, strLen);
		name[strLen] = '\0';
		std::string str(name);
		m_textureName.push_back(str);
		std::free(name);
	}

	ogmFile.read((char*)&temp, sizeof(int));
	m_materialRefVertexIndexCount.resize(temp);
	ogmFile.read((char*)&m_materialRefVertexIndexCount[0], sizeof(int)*temp);
	m_textureIndex.resize(temp);
	ogmFile.read((char*)&m_textureIndex[0], sizeof(T_TextureIndex)*temp);
	m_materialData.resize(temp);
	ogmFile.read((char*)&m_materialData[0], sizeof(T_MaterialData)*temp);

	ogmFile.close();
	return true;
}

bool	OGL::OgmData::ExportFile(std::string fileName)
{
	std::ofstream ogmFile(fileName, std::ios::out | std::ios::trunc | std::ios::binary);
	int temp = m_vertexIndex.size();
	ogmFile.write((const char*)&temp, sizeof(int));
	ogmFile.write((const char*)&m_vertexIndex[0], sizeof(int)*temp);

	temp = m_vertexData.pos.size();
	ogmFile.write((const char*)&temp, sizeof(int));
	ogmFile.write((const char*)&m_vertexData.pos[0], sizeof(D3DXVECTOR4)*temp);
	ogmFile.write((const char*)&m_vertexData.normal[0], sizeof(D3DXVECTOR4)*temp);
	ogmFile.write((const char*)&m_vertexData.uv[0], sizeof(D3DXVECTOR4)*temp);
	ogmFile.write((const char*)&m_vertexData.boneIndex[0], sizeof(D3DXVECTOR4)*temp);
	ogmFile.write((const char*)&m_vertexData.boneWeight[0], sizeof(D3DXVECTOR4)*temp);

	temp = m_textureName.size();
	ogmFile.write((const char*)&temp, sizeof(int));
	for (int i = 0; i < temp; i++)
	{
		std::string::size_type strLen = m_textureName[i].size();
		ogmFile.write((const char*)&strLen, sizeof(strLen));
		ogmFile.write(m_textureName[i].c_str(), sizeof(char)*strLen);
	}

	temp = m_materialRefVertexIndexCount.size();
	ogmFile.write((const char*)&temp, sizeof(int));
	ogmFile.write((const char*)&m_materialRefVertexIndexCount[0], sizeof(int)*temp);
	ogmFile.write((const char*)&m_textureIndex[0], sizeof(T_TextureIndex)*temp);
	ogmFile.write((const char*)&m_materialData[0], sizeof(T_MaterialData)*temp);

	ogmFile.close();
	return true;
}
//==========================
//===【OgmData.h】
//==========================
//    Ogmデータを保持します。
//    (OGL_D3D11で用いるモデルデータ)
//==========================
#pragma	once
//=====================//
//===【インクルード記述】 ===//
//=====================//
#include"../PmxData/PmxData.h"
#include<string>
#include<vector>

namespace	OGL
{
	class PmxData;

	class	OgmData
	{
	public:
		bool	ImportFromModelData(PmxData* modelData);
		bool	ImportFromModelFile(std::string fileName);
		bool	ExportFile(std::string fileName);

		struct T_VertexData
		{
			std::vector<D3DXVECTOR4> pos;
			std::vector<D3DXVECTOR4> normal;
			std::vector<D3DXVECTOR4> uv;
			std::vector<D3DXVECTOR4> boneIndex;
			std::vector<D3DXVECTOR4> boneWeight;
		};
		struct T_TextureIndex
		{
			int diffuse;
			int toon;
			int addSphere;
			int mulSphere;
		};
		struct T_MaterialData
		{
			D3DXVECTOR4 diffuse;
			D3DXVECTOR4 specular;		//wはPower
			D3DXVECTOR4	ambient;
		};
	private:
		std::vector<int> m_vertexIndex;
		
		T_VertexData	m_vertexData;
		
		std::vector<std::string> m_textureName;

		std::vector<int> m_materialRefVertexIndexCount;
		std::vector<T_TextureIndex>	m_textureIndex;
		std::vector<T_MaterialData> m_materialData;

	public:
		OgmData();
		~OgmData();
	};	//EndOf__class_OgmData
}	//EndOf__namespace_OGL


/*
OGMファイルフォーマット

int インデックス数;
int[] インデックス配列;
int 頂点数;
D3DXVECTOR4[] pos;
D3DXVECTOR4[] normal;
D3DXVECTOR4[] uv;
D3DXVECTOR4[] boneIndex;
D3DXVECTOR4[] boneWeight;
int テクスチャ数;
配列
{
	std::string::size_type 文字数
	std::string[] テクスチャ名
}
int マテリアル数;
int[] マテリアル参照頂点インデックス数
T_TextureIndex[] テクスチャインデックス情報
T_MaterialData[] マテリアル情報
*/
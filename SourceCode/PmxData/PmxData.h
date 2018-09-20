//==========================
//===【PmxData.h】
//==========================
//    Pmxデータを保持します。
//==========================
#pragma	once
//=====================//
//===【インクルード記述】 ===//
//=====================//
#include	<d3dx9.h>
#include<iostream>
#include<fstream>
#include<string>
#include<vector>
#include<memory>
#include"../OgmData/OgmData.h"

namespace	OGL
{
	class OgmData;

	class	PmxData
	{
	enum HEADER_STYLE
	{
		HS_encodeStyle,		//エンコード方式0
		HS_addTexCount,		//追加UV数1
		HS_vertexIndexSize,	//頂点インデックスサイズ2
		HS_texIndexSize,		//テクスチャインデックスサイズ3
		HS_materialIndexSize,//マテリアルインデックスサイズ4
		HS_boneIndexSize,	//ボーンインデックスサイズ5
		HS_morphIndexSize,	//モーフインデックスサイズ6
		HS_rigidBodyIndexSize,//剛体インデックスサイズ7
		HS_MAX,
	};
	enum BONE_STYLE
	{
		BS_BDEF1,
		BS_BDEF2,
		BS_BDEF4,
		BS_SDEF,
		BS_MAX,
	};
	enum SPHERE_TYPE
	{
		ST_NONE,
		ST_MUL,
		ST_ADD,
		ST_SUB,
	};
	struct T_PmxVertexData
	{
		D3DXVECTOR3	pos;
		D3DXVECTOR3	normal;
		D3DXVECTOR2	texUV;
		D3DXVECTOR4	addTexUV[4];
		byte	boneWeightStyle;
		int					boneIndex[4];
		float				boneWeight[4];
		D3DXVECTOR3	sdefC;
		D3DXVECTOR3	sdefR0;
		D3DXVECTOR3	sdefR1;
		float				edgeSize;
	};
	struct T_PmxMaterialData
	{
		std::string		materialNameJapanese;
		std::string		materialNameEnglish;
		D3DXVECTOR4	diffuse;
		D3DXVECTOR3	specular;
		float				specularPower;
		D3DXVECTOR3	ambient;
		bool				drawBothAspectFlg;
		bool				drawGroundShadowFlg;
		bool				drawSelfShadowMapFlg;
		bool				drawSelfShadowFlg;
		bool				drawEdgeFlg;
		D3DXVECTOR4	edgeCollar;
		float				edgeSize;
		int					diffuseTextureIndex;
		int					sphereTextureIndex;
		SPHERE_TYPE	sphereType;
		bool				toonTextureShareFlg;
		int					toonTextureIndex;
		byte				toonTextureShareIndex;
		std::string		comment;
		int					referenceVertexIndexCount;
	};
	private:
		std::ifstream pmxData;
		float m_pmxVersion;
		byte m_headerData[HS_MAX];
		std::string	m_modelNameEnglish;
		std::string	m_modelNameJapanese;
		std::string	m_commentEnglish;
		std::string	m_commentJapanese;
		std::vector<T_PmxVertexData>		m_vertexData;
		std::vector<D3DXVECTOR3>			m_vertexIndex;
		std::vector<std::string>				m_texturePass;
		std::vector<T_PmxMaterialData>	m_materialData;


	private:
		std::string ReadText(void);
		int ReadStyleCast(byte readByte);
	public:
		PmxData();
		~PmxData();

		bool	LoadFile(char* fileName);

		friend OgmData;
	};	//EndOf__class_PmxData
}	//EndOf__namespace_OGL

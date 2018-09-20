//==========================
//===【PmxData.cpp】
//==========================
//    Pmxデータを保持します。
//==========================

//=====================//
//===【インクルード記述】 ===//
//=====================//
#include	"PmxData.h"

using namespace std;
OGL::PmxData::PmxData()
{
	m_pmxVersion = 0;
}

OGL::PmxData::~PmxData()
{
}
#define Read(data,style)(pmxData.read((char*)&data, sizeof(style)))

//■■■■■■■■■■■//
//■■■【public】■■■//
//■■■■■■■■■■■//

//■■■■■■■■■■■■■■■■■■■■■■
//■■■　機能：pmxファイルをロードします。
//■■■ 引数1：char*：ファイル名
//■■■   返値：void
//■■■■■■■■■■■■■■■■■■■■■■
bool	OGL::PmxData::LoadFile(char* fileName)
{
	setlocale(LC_CTYPE, "");

	pmxData.open(fileName, ios::in | ios::binary);

	//▼▼▼テクスチャファイルパス用の文字列を生成▼▼▼//
	string filePass = fileName;
	filePass.erase(filePass.find_last_of('/') + 1, filePass.length());

	if (pmxData.fail())
	{
		MessageBox(NULL, TEXT("ファイルの読み込みに失敗しました。\nファイルパスを間違えている可能性があります。"), TEXT("Err"), MB_OK);
		return false;
	}
	
	byte	pmxCheck[4];

	Read(pmxCheck[0], byte);
	Read(pmxCheck[1], byte); 
	Read(pmxCheck[2], byte); 
	Read(pmxCheck[3], byte); 
	if (!(pmxCheck[0] == 0x50 && pmxCheck[1] == 0x4d &&
		pmxCheck[2] == 0x58 && pmxCheck[3] == 0x20))
	{
		MessageBox(NULL, TEXT("このファイルはPMX形式ではありません。"), TEXT("Err"), MB_OK);
		return false;
	}
	
	Read(m_pmxVersion, float);
	printf("PMX VERSION：%2f\n", m_pmxVersion);

	byte headerSize=0;
	Read(headerSize, byte);
	for (int i = 0; i < headerSize; i++)
	{
		Read(m_headerData[i], byte);
	}
	if (m_headerData[HS_encodeStyle] != 0)
	{
		MessageBox(NULL, TEXT("PMXファイルのエンコード方式を\nUTF16に指定して下さい。"), TEXT("Err"), MB_OK);
		return false;
	}

	m_modelNameJapanese = ReadText();
	m_modelNameEnglish = ReadText();
	m_commentEnglish = ReadText();
	m_commentJapanese = ReadText();

	//▼▼▼頂点数抽出▼▼▼//
	int	count = 0;
	Read(count, int);
	//▼▼▼頂点情報抽出▼▼▼//
	for (int vertexNum = 0; vertexNum < count; vertexNum++)
	{
		T_PmxVertexData	tempVertexData;
		Read(tempVertexData.pos, D3DXVECTOR3);
		Read(tempVertexData.normal, D3DXVECTOR3);
		Read(tempVertexData.texUV, D3DXVECTOR2);
	
		int addUVNum = 0;
		for (addUVNum = 0; addUVNum < m_headerData[HS_addTexCount]; addUVNum++)
		{
			Read(tempVertexData.addTexUV[addUVNum], D3DXVECTOR4);
		}
		for (; addUVNum < 4; addUVNum++)
		{
			tempVertexData.addTexUV[addUVNum] = D3DXVECTOR4(0, 0, 0, 0);
		}

		Read(tempVertexData.boneWeightStyle, byte);
		switch (tempVertexData.boneWeightStyle)
		{
		case	BS_BDEF1:
			tempVertexData.boneIndex[0] = ReadStyleCast(m_headerData[HS_boneIndexSize]);
			tempVertexData.boneWeight[0] = 1;
			tempVertexData.boneIndex[1] = 0;
			tempVertexData.boneWeight[1] = 0;
			tempVertexData.boneIndex[2] = 0;
			tempVertexData.boneWeight[2] = 0;
			tempVertexData.boneIndex[3] = 0;
			tempVertexData.boneWeight[3] = 0;
			break;
		case	BS_BDEF2:
			tempVertexData.boneIndex[0] = ReadStyleCast(m_headerData[HS_boneIndexSize]);
			tempVertexData.boneIndex[1] = ReadStyleCast(m_headerData[HS_boneIndexSize]);
			Read(tempVertexData.boneWeight[0], float);
			tempVertexData.boneWeight[1] = 1 - tempVertexData.boneWeight[0];
			tempVertexData.boneIndex[2] = 0;
			tempVertexData.boneWeight[2] = 0;
			tempVertexData.boneIndex[3] = 0;
			tempVertexData.boneWeight[3] = 0;
			break;
		case	BS_BDEF4:
			tempVertexData.boneIndex[0] = ReadStyleCast(m_headerData[HS_boneIndexSize]);
			tempVertexData.boneIndex[1] = ReadStyleCast(m_headerData[HS_boneIndexSize]);
			tempVertexData.boneIndex[2] = ReadStyleCast(m_headerData[HS_boneIndexSize]);
			tempVertexData.boneIndex[3] = ReadStyleCast(m_headerData[HS_boneIndexSize]);
			Read(tempVertexData.boneWeight[0], float);
			Read(tempVertexData.boneWeight[1], float);
			Read(tempVertexData.boneWeight[2], float);
			Read(tempVertexData.boneWeight[3], float);
			break;
		case	BS_SDEF:
			tempVertexData.boneIndex[0] = ReadStyleCast(m_headerData[HS_boneIndexSize]);
			tempVertexData.boneIndex[1] = ReadStyleCast(m_headerData[HS_boneIndexSize]);
			Read(tempVertexData.boneWeight[0], float);
			tempVertexData.boneWeight[1] = 1 - tempVertexData.boneWeight[0];
			tempVertexData.boneIndex[2] = 0;
			tempVertexData.boneWeight[2] = 0;
			tempVertexData.boneIndex[3] = 0;
			tempVertexData.boneWeight[3] = 0;
			Read(tempVertexData.sdefC, D3DXVECTOR3);
			Read(tempVertexData.sdefR0, D3DXVECTOR3);
			Read(tempVertexData.sdefR1, D3DXVECTOR3);
			break;
		}
		Read(tempVertexData.edgeSize, float);
		m_vertexData.push_back(tempVertexData);
	}
	//▼▼▼頂点インデックス数を抽出▼▼▼//
	Read(count, int);
	if (count % 3 != 0)
	{
		MessageBox(NULL, TEXT("このPMXファイルは全て三角形で構成されていません。"), TEXT("Err"), MB_OK);
		return false;
	}
	//▼▼▼頂点インデックス情報を抽出▼▼▼//
	for (int polygonNum = 0; polygonNum < count / 3; polygonNum++)
	{
		D3DXVECTOR3 tempPolygonIndex;
		tempPolygonIndex.x = ReadStyleCast(m_headerData[HS_vertexIndexSize]);
		tempPolygonIndex.y = ReadStyleCast(m_headerData[HS_vertexIndexSize]);
		tempPolygonIndex.z = ReadStyleCast(m_headerData[HS_vertexIndexSize]);
		m_vertexIndex.push_back(tempPolygonIndex);
	}
	//▼▼▼テクスチャ数▼▼▼//
	Read(count, int);
	//▼▼▼テクスチャ情報を抽出▼▼▼//
	for (int textureCount = 0; textureCount < count; textureCount++)
	{
		std::string texPass;
		texPass=ReadText();
	//	texPass = filePass + texPass;
		m_texturePass.push_back(texPass);
	}
	//▼▼▼マテリアル数▼▼▼//
	Read(count, int);
	//▼▼▼マテリアル情報の抽出▼▼▼//
	for (int materialCount = 0; materialCount < count; materialCount++)
	{
		T_PmxMaterialData materialData;
		materialData.materialNameJapanese = ReadText();
		materialData.materialNameEnglish = ReadText();
		Read(materialData.diffuse, D3DXVECTOR4);
		Read(materialData.specular, D3DXVECTOR3);
		Read(materialData.specularPower, float);
		Read(materialData.ambient, D3DXVECTOR3);
		byte temp;
		Read(temp, byte);
		materialData.drawBothAspectFlg = temp & 0x01;
		materialData.drawGroundShadowFlg = temp & 0x02;
		materialData.drawSelfShadowMapFlg = temp & 0x04;
		materialData.drawSelfShadowFlg = temp & 0x08;
		materialData.drawEdgeFlg = temp & 0x10;
		Read(materialData.edgeCollar, D3DXVECTOR4);
		Read(materialData.edgeSize, float);
		materialData.diffuseTextureIndex = ReadStyleCast(m_headerData[HS_texIndexSize]);
		materialData.sphereTextureIndex = ReadStyleCast(m_headerData[HS_texIndexSize]);
		Read(temp, byte);
		switch (temp)
		{
		case 0:
			materialData.sphereType = ST_NONE;
			break;
		case 1:
			materialData.sphereType = ST_MUL;
			break;
		case 2:
			materialData.sphereType = ST_ADD;
			break;
		case 3:
			materialData.sphereType = ST_SUB;
			break;
		}
		Read(temp, byte);
		if (temp)
		{
			materialData.toonTextureShareFlg = true;
			Read(materialData.toonTextureShareIndex, byte);
			materialData.toonTextureIndex = -1;
		}
		else{
			materialData.toonTextureShareFlg = false;
			materialData.toonTextureIndex = ReadStyleCast(m_headerData[HS_texIndexSize]);
			materialData.toonTextureShareIndex = -1;
		}
		materialData.comment = ReadText();
		Read(materialData.referenceVertexIndexCount, int);
		m_materialData.push_back(materialData);
	}
	pmxData.close();

	return true;
}

//■■■■■■■■■■■■//
//■■■【private】 ■■■//
//■■■■■■■■■■■■//

//■■■■■■■■■■■■■■■■■■■■■■
//■■■　機能：pmxファイルのテキスト部分のロードを行います。
//■■■ 引数1：void
//■■■   返値：string：ロードしたテキスト
//■■■■■■■■■■■■■■■■■■■■■■
string OGL::PmxData::ReadText(void)
{
	int size;
	Read(size, int);
	size = (size / 2);
	wstring src(size,'a');
	string strData;
	for (int i = 0; i < size; i++)
	{
		wchar_t str;
		Read(str, wchar_t);
		src[i] = str;
	}
	char *mbs = new char[src.length() * MB_CUR_MAX + 1];
	wcstombs(mbs, src.c_str(), src.length() * MB_CUR_MAX + 1);
	strData = mbs;
	delete[] mbs;

	return	strData;
}

//■■■■■■■■■■■■■■■■■■■■■■
//■■■　機能：指定したバイト数を読み込み、intでキャストして返します。
//■■■ 引数1：byte：読み込むバイト数
//■■■   返値：int：読み込んだ値
//■■■■■■■■■■■■■■■■■■■■■■
int		OGL::PmxData::ReadStyleCast(byte readByte)
{
	switch (readByte)
	{
	case 1:
		byte	b_buff;
		Read(b_buff, byte);
		return (int)b_buff;
		break;
	case 2:
		short	s_buff;
		Read(s_buff, short);
		return (int)s_buff;
		break;
	case 4:
		int i_buff;
		Read(i_buff, int);
		return i_buff;
		break;
	default:
		return 0;
		break;
	}
	return 0;
}
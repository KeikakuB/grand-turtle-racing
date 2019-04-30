#pragma once
#include <fstream>
#include <DirectXMath.h>
using namespace DirectX;
using namespace std;

const int M = 257;
const int nombreIndex = (M - 1)*(M - 1) * 6;
const float EchelleZ = 0.05;

class CSommetTerrain
{
public:
	CSommetTerrain(){}
	CSommetTerrain(XMFLOAT3 p, XMFLOAT3 n)
	{
		position = p;
		normale = n;
	}

	XMFLOAT3 position;
	XMFLOAT3 normale;
};

class CTerrain
{
protected:
	CSommetTerrain sommets[M*M];
	int index[nombreIndex];
	unsigned char hauteurs[M*M];
	
public:
	
	~CTerrain();


	XMVECTOR ObtenirPosition(int x, int y)
	{
		return XMLoadFloat3(&sommets[y*M + x].position);
	}

	XMFLOAT3 CalculNormale(int x, int y)
	{
		XMVECTOR n1;
		XMVECTOR n2;
		XMVECTOR n3;
		XMVECTOR n4;

		XMVECTOR v1;
		XMVECTOR v2;
		XMVECTOR v3;
		XMVECTOR v4;

		n1 = n2 = n3 = n4 = XMVectorSet(0, 0, 1, 0);  // Le Z est le haut

		// v1 = p1 – p0, etc...
		XMVECTOR v = ObtenirPosition(x, y + 1);
		if (y < M - 1) 	v1 = ObtenirPosition(x, y + 1) - ObtenirPosition(x, y);
		if (x < M - 1)	v2 = ObtenirPosition(x + 1, y) - ObtenirPosition(x, y);
		if (y > 0)		v3 = ObtenirPosition(x, y - 1) - ObtenirPosition(x, y);
		if (x > 0)	v4 = ObtenirPosition(x - 1, y) - ObtenirPosition(x, y);

		// les produits vectoriels
		if (y < M - 1 && x < M - 1) n1 = XMVector3Cross(v2, v1);
		if (y > 0 && x < M - 1) n2 = XMVector3Cross(v3, v2);
		if (y > 0 && x > 0) n3 = XMVector3Cross(v4, v3);
		if (y < M - 1 && x > 0) n4 = XMVector3Cross(v1, v4);

		n1 = n1 + n2 + n3 + n4;

		n1 = XMVector3Normalize(n1);
		XMFLOAT3 resultat;

		XMStoreFloat3(&resultat, n1);

		return resultat;
	}

	void ConstruireTerrain()
	{
		for (int x = 0; x < 257; ++x)
		{
			for (int y = 0; y < 257; ++y)
			{
				XMFLOAT3 position = XMFLOAT3(float(x),float(y), EchelleZ*hauteurs[y*M + x]);
				XMFLOAT3 normale = XMFLOAT3(0.0f, 0.0f, 0.0f);
				sommets[y*M + x] = CSommetTerrain(position, normale);
			}
		}
	}
	void CalculNormales()
	{
		for (int x = 0; x < 257; ++x)
		{
			for (int y = 0; y < 257; ++y)
			{
				sommets[y*M + x].normale = CalculNormale(x, y);
			}
		}
	}

	void ConstruireIndex()
	{
		int k = 0; 

		for (int y = 0; y < M - 1; ++y)
		{
			for (int x = 0; x < M - 1; ++x)
			{
				// L'important ici est d'utiliser la même formule pour identifier
				// les sommets qu'au moment de leur création
				index[k++] = y*M + x;
				index[k++] = (y + 1) * M + (x + 1);
				index[k++] = y*M + (x + 1);
				index[k++] = y*M + x;
				index[k++] = (y + 1)* M + x;
				index[k++] = (y + 1)* M + (x + 1);
			}
		}
	}

	void LireFichierHeightmap()
	{
		ifstream fichier;
		fichier.open("Terrain.raw", ios::in | ios_base::binary);

		fichier.read((char*)hauteurs, M*M);

		fichier.close();
	}

	void EnregistrerTout()
	{
		// 1. SOMMETS  Écriture des sommets dans un fichier binaire

		ofstream fichier;
		fichier.open("Terrain.ter", ios::out | ios_base::binary);

		fichier.write((char*)sommets, M*M*sizeof(CSommetTerrain));

		// 2. INDEX 

		fichier.write((char*)index, nombreIndex * sizeof(int));

		fichier.close();
	}

};
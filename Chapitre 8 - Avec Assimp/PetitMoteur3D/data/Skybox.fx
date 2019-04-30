cbuffer param
{
  float4x4 matWorldViewProj;   // la matrice totale
  float4x4 matWorld;		// matrice de transformation dans le monde
  float remplissage;
}

struct VS_Sortie
{
  float4 Pos : SV_Position;
  float3 Norm :    NORMAL0;
  float2 coordTex : TEXCOORD0;
};

VS_Sortie SkyboxVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex : TEXCOORD)
{
  VS_Sortie sortie = (VS_Sortie) 0;

  sortie.Pos = mul(Pos, matWorldViewProj);
  sortie.Norm = mul(Normale, matWorld);

  // Coordonn�es d'application de texture
  sortie.coordTex = coordTex;

  return sortie;
}

Texture2D textureFront;
Texture2D textureBack;
Texture2D textureLeft;
Texture2D textureRight;
Texture2D textureUp;
Texture2D textureDown;
SamplerState SampleState;  // l'�tat de sampling

float fuzzy = 0.999;

float4 SkyboxPS(VS_Sortie vs) : SV_Target
{
float4 couleur;
  float3 N = normalize(vs.Norm);
  if(N.y < -fuzzy) {
    couleur = textureBack.Sample(SampleState, float2(vs.coordTex.x, 1.0 - vs.coordTex.y));
  } else if(N.y > fuzzy) {
    couleur = textureFront.Sample(SampleState, float2(1.0 - vs.coordTex.x, vs.coordTex.y));

  } else if(N.x < -fuzzy) {
    couleur = textureRight.Sample(SampleState, float2(vs.coordTex.x, 1.0 - vs.coordTex.y));
  } else if(N.x > fuzzy) {
    couleur = textureLeft.Sample(SampleState, float2(vs.coordTex.x, 1.0 - vs.coordTex.y));

  } else if(N.z < -fuzzy) {
    couleur = textureDown.Sample(SampleState, float2(1.0 - vs.coordTex.x, 1.0 -vs.coordTex.y));
  } else if(N.z > fuzzy) {
    couleur = textureUp.Sample(SampleState, float2(vs.coordTex.x,vs.coordTex.y));
  } else {
    couleur = float4(0,1,1,1);
  }

return couleur;
}

technique11 Skybox
{
  pass pass0
  {
    SetVertexShader(CompileShader( vs_4_0, SkyboxVS() ));
    SetPixelShader(CompileShader( ps_4_0, SkyboxPS() ));
    SetGeometryShader(NULL);
  }
}

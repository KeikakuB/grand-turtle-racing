cbuffer param
{
  float4x4 matWorldViewProj;   // la matrice totale
  float4x4 matWorld;		// matrice de transformation dans le monde
  float4 vLumiere; 		// la position de la source d'�clairage (Point)
  float4 vDEcl; 			// la valeur diffuse de l'�clairage
  float4 vDMat; 			// la valeur diffuse du mat�riau
  int bTex;
  float3 remplissage;
}

Texture2D textureEntree;  // la texture
SamplerState SampleState;  // l'�tat de sampling

//--------------------------- TOON SHADER PROPERTIES ------------------------------
// The color to draw the lines in.  Black is a good default.
float4 LineColor = float4( 0, 0, 0, 1 );

// The thickness of the lines.  This may need to change, depending on the scale of
// the objects you are drawing.
float LineThickness = 0.1;

struct VS_Sortie
{
  float4 Pos : SV_Position;
  float3 Norm :    TEXCOORD0;
  float3 vDirLum : TEXCOORD1;
  float2 CoordTex : TEXCOORD2;
};

VS_Sortie CelVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex : TEXCOORD)
{
  VS_Sortie sortie = (VS_Sortie) 0;

  sortie.Pos = mul(Pos, matWorldViewProj);
  sortie.Norm = mul(Normale, matWorld);

  float3 PosWorld = mul(Pos, matWorld);

  sortie.vDirLum = vLumiere - PosWorld;

  // Coordonn�es d'application de texture
  sortie.CoordTex = coordTex;

  return sortie;
}

float4 CelPS(VS_Sortie vs) : SV_Target
{
  // Normaliser les param�tres
  float3 N = normalize(vs.Norm);
  float3 L = normalize(vs.vDirLum);

// Calculate diffuse light amount
    float intensity = dot(L, N);
    if ( intensity < 0 )
        intensity = 0;

    float4 color;
    // Calculate what would normally be the final color, including texturing and diffuse lighting
    if(bTex > 0) {
      color = textureEntree.Sample(SampleState, vs.CoordTex) * vDEcl * vDMat;
    } else {
      color = float4(0,0,0,1);
    }



    color.a = 1;

    // Discretize the intensity, based on a few cutoff points
    if ( intensity > 0.95 )
        color = float4( 1.0,1,1,1.0 ) * color;
    else if ( intensity > 0.5 )
        color = float4( 0.7,0.7,0.7,1.0 ) * color;
    else if ( intensity > 0.05 )
        color = float4( 0.35,0.35,0.35,1.0 ) * color;
    else
        color = float4( 0.2,0.2,0.2,1.0 ) * color;

    color.a = 1;

    return color;
}

// The vertex shader that does the outlines
VS_Sortie OutlineVS(float4 Pos : POSITION, float3 Normale : NORMAL, float2 coordTex : TEXCOORD)
{
  VS_Sortie sortie = (VS_Sortie) 0;

  // Calculate where the vertex ought to be.  This line is equivalent
  // to the transformations in the CelVertexShader.
  float4 original = mul(Pos, matWorldViewProj);

  // Calculates the normal of the vertex like it ought to be.
  float4 normal = mul(Normale, matWorldViewProj);

  // Take the correct "original" location and translate the vertex a little
  // bit in the direction of the normal to draw a slightly expanded object.
  // Later, we will draw over most of this with the right color, except the expanded
  // part, which will leave the outline that we want.
  sortie.Pos = original + LineThickness * normal;

  return sortie;
}

// The pixel shader for the outline.  It is pretty simple:  draw everything with the
// correct line color.
float4 OutlinePS(VS_Sortie vs) : SV_Target
{
    return LineColor;
}

// The entire technique for doing toon shading
technique11 CelShading
{
  // The first pass will go through and draw the back-facing triangles with the outline shader,
  // which will draw a slightly larger version of the model with the outline color.  Later, the
  // model will get drawn normally, and draw over the top most of this, leaving only an outline.
  pass pass0
  {
    VertexShader = compile vs_4_0 OutlineVS();
    PixelShader = compile ps_4_0 OutlinePS();
  }

  // The second pass will draw the model like normal, but with the cel pixel shader, which will
  // color the model with certain colors, giving us the cel/toon effect that we are looking for.
  pass pass1
  {
    VertexShader = compile vs_4_0 CelVS();
    PixelShader = compile ps_4_0 CelPS();
  }
}

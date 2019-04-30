struct VS_Sortie
{
  float4 Pos : SV_POSITION;
  float2 CoordTex : TEXCOORD0;
};
int screen_width;
int screen_height;
float time;
float RadialBlur_distance;
int GradualBlur_starting_offset;

Texture2D textureEntree;    // la texture
SamplerState SampleState;   // l'?tat de sampling

//colors
float4 color_black = float4( 0.0, 0.0, 0.0, 1.0 );

//texture coords
float2 texture_coord_middle = float2( 0.5, 0.5 );

//tiles params
uniform float NumTiles = 64;
uniform float Threshhold = 0.1;
uniform float3 EdgeColor = float3( 1, 1, 1 );

bool IsInCircle(float2 center, float radius, float2 p)
{
  float distFromCenter = distance(center, p);
  return ( distFromCenter <= radius );
}

bool IsInRectangle(float2 top_left, float2 bot_right, float2 p)
{
  return
    p.x > top_left.x
    && p.x <= bot_right.x
    && p.y > top_left.y
    && p.y <= bot_right.y;
}

float4 GetGreyScaleColor(float4 color)
{
  float4 newColor;
  float greyscale;
  greyscale = ( color[ 0 ] + color[ 1 ] + color[ 2 ] ) / 3.0;
  newColor[ 0 ] = greyscale;
  newColor[ 1 ] = greyscale;
  newColor[ 2 ] = greyscale;
  newColor[ 3 ] = color[ 3 ];
  return newColor;
}

float4 GetSepiaColor(float4 color)
{
  float4 outputColor = color;
  outputColor.r = ( color.r * 0.393 ) + ( color.g * 0.769 ) + ( color.b * 0.189 );
  outputColor.g = ( color.r * 0.349 ) + ( color.g * 0.686 ) + ( color.b * 0.168 );
  outputColor.b = ( color.r * 0.272 ) + ( color.g * 0.534 ) + ( color.b * 0.131 );

  return outputColor;
}

float4 GetInversedColor(float4 color)
{
  float4 newColor;
  newColor = float4( 1.0 - color.r, 1.0 - color.g, 1.0 - color.b, color.a );
  return newColor;
}

float4 GetHorizontalShuttersColor(float2 coord, float t)
{
  //TODO(Bill): should only compute this once
  float2 top_rect_top_left = float2( 0.0, 0.0 );
  float2 top_rect_bot_right = float2( 1.0, 0.5 * t );
  float2 bot_rect_top_left = float2( 0, 1.0 - ( 0.5 * t ) );
  float2 bot_rect_bot_right = float2( 1, 1 );
  if ( IsInRectangle(top_rect_top_left, top_rect_bot_right, coord)
    || IsInRectangle(bot_rect_top_left, bot_rect_bot_right, coord) )
  {
    return color_black;
  }
  else
  {
    return textureEntree.Sample(SampleState, coord);
  }
}

//-------------------------------------------------------
// Vertex Shader ?Nul?
//-------------------------------------------------------
VS_Sortie NulVS(float4 Pos : POSITION, float2 CoordTex : TEXCOORD)
{
  VS_Sortie sortie = (VS_Sortie) 0;

  sortie.Pos = Pos;
  sortie.CoordTex = CoordTex;

  return sortie;
}

//-----------------------------------------------------
// Pixel Shader ?Nul?
//-----------------------------------------------------
float4 NulPS(VS_Sortie vs) : SV_Target
{
  float4 couleur;
  couleur = textureEntree.Sample(SampleState, vs.CoordTex);
  return couleur;
}

float4 RadialBlurPS(VS_Sortie vs) : SV_Target
{
  float4 couleur;
  float4 ct;
  float2 tc = vs.CoordTex;
  float d, dx, dy;

  couleur = 0;
  float x = tc.x * 2 - 1.0;
  float y = tc.y * 2 - 1.0;
  dx = sqrt(x*x);  // Distance du centre
  dy = sqrt(y*y);  // Distance du centre

  dx = x * ( RadialBlur_distance*dx );  	// Le d?grad? (blur) est en fonction de la
  dy = y * ( RadialBlur_distance*dy );	 	// distance du centre et de la variable distance.

  x = x - ( dx * 10 );   // Vous pouvez jouer avec le nombre d'it?rations
  y = y - ( dy * 10 );

  tc.x = ( x + 1.0 ) / 2.0;
  tc.y = ( y + 1.0 ) / 2.0;

  for ( int i = 0; i < 10; i++ )   // Vous pouvez jouer avec le nombre d'it?rations
  {
     ct = textureEntree.Sample(SampleState, tc);
     couleur = couleur * 0.6 + ct * 0.4; // Vous pouvez ?jouer? avec les %

     x = x + dx;
     y = y + dy;

     tc.x = ( x + 1.0 ) / 2.0;
     tc.y = ( y + 1.0 ) / 2.0;
  }
  return couleur;
}

float4 GreyScalePS(VS_Sortie vs) : SV_Target
{
  float4 couleur;
  couleur = textureEntree.Sample(SampleState, vs.CoordTex);
  return GetGreyScaleColor(couleur);
}

float4 SepiaPS(VS_Sortie vs) : SV_Target
{
  float4 couleur;
  couleur = textureEntree.Sample(SampleState, vs.CoordTex);
  return GetSepiaColor(couleur);
}

float4 InversedColorsPS(VS_Sortie vs) : SV_Target
{
  float4 couleur;
  couleur = textureEntree.Sample(SampleState, vs.CoordTex);
  return GetInversedColor(couleur);
}

float4 CombinedColorsPS(VS_Sortie vs) : SV_Target
{
  float4 computed_color;
  float4 original_color;
  original_color = textureEntree.Sample(SampleState, vs.CoordTex);
  if ( vs.CoordTex.x < 0.5 && vs.CoordTex.y < 0.5 )
  {
    computed_color = original_color;
  }
  else if ( vs.CoordTex.x >= 0.5 && vs.CoordTex.y < 0.5 )
  {
    computed_color = GetSepiaColor(original_color);
  }
  else if ( vs.CoordTex.x < 0.5 && vs.CoordTex.y >= 0.5 )
  {
    computed_color = GetGreyScaleColor(original_color);
  }
  else
  {
    computed_color = GetInversedColor(original_color);
  }
  float4 new_color;
  new_color = lerp(original_color, computed_color, time);
  new_color.a = 1.0;
  return new_color;
}

float4 GradualBlurPS(VS_Sortie vs) : SV_Target
{
  float4 sumColor = textureEntree.Sample(SampleState,vs.CoordTex);
  int count = 1;
  float2 pixel_offset = float2( 1.0 / screen_width, 1.0 / screen_height );
  int base_offset = GradualBlur_starting_offset * time;
  int2 offset_index = int2( -base_offset, -base_offset );
  for ( ; offset_index.x <= base_offset; ++offset_index.x )
  {
    for ( ; offset_index.y <= base_offset; ++offset_index.y )
    {
      if ( !all(offset_index) ) { continue; }
      sumColor += textureEntree.Sample(SampleState,
        float2( vs.CoordTex[ 0 ] + offset_index.x * pixel_offset.x, vs.CoordTex[ 1 ] + offset_index.y * pixel_offset.y ));
      ++count;
    }
    offset_index.y = -base_offset;
  }
  sumColor /= 0.0 + count;
  sumColor.a = 1.0;
  return sumColor;
}

float4 GrowingPeepholePS(VS_Sortie vs) : SV_Target
{
  if ( !IsInCircle(texture_coord_middle, time, vs.CoordTex) )
 {
return color_black;
}
else
{
return textureEntree.Sample(SampleState, vs.CoordTex);
}
}

float4 HorizontalShuttersClosePS(VS_Sortie vs) : SV_Target
{
  return GetHorizontalShuttersColor(vs.CoordTex, time);
}

float4 HorizontalShuttersOpenPS(VS_Sortie vs) : SV_Target
{
  return GetHorizontalShuttersColor(vs.CoordTex, 1.0 - time);
}

float4 RageVisionPS(VS_Sortie vs) : SV_Target
{
  float4 rage;
  float4 color;
  color = textureEntree.Sample(SampleState, vs.CoordTex);
  if ( IsInCircle(texture_coord_middle, 0.4, vs.CoordTex) )
 {
return color;
}
else
{
rage = color + float4( saturate(distance(vs.CoordTex,texture_coord_middle) - time),0,0,0 );
return rage;
}
}

float4 TilesPS(VS_Sortie vs) : SV_Target
{
    float size = 1.0 / NumTiles;
    float2 Pbase = vs.CoordTex - fmod(vs.CoordTex,size.xx);
    float2 PCenter = Pbase + ( size / 2.0 ).xx;
    float2 st = ( vs.CoordTex - Pbase ) / size;
    float4 c1 = ( float4 )0;
    float4 c2 = ( float4 )0;
    float4 invOff = float4( ( 1 - EdgeColor ),1 );
    if ( st.x > st.y ) { c1 = invOff; }
    float threshholdB = 1.0 - Threshhold;
    if ( st.x > threshholdB ) { c2 = c1; }
    if ( st.y > threshholdB ) { c2 = c1; }
    float4 cBottom = c2;
    c1 = ( float4 )0;
    c2 = ( float4 )0;
    if ( st.x > st.y ) { c1 = invOff; }
    if ( st.x < Threshhold ) { c2 = c1; }
    if ( st.y < Threshhold ) { c2 = c1; }
    float4 cTop = c2;
    float4 tileColor = textureEntree.Sample(SampleState, PCenter);
    float4 result = tileColor + cTop - cBottom;
    return result;
}

technique11 Nul
{
  pass p0
  {
    VertexShader = compile vs_4_0 NulVS();
    PixelShader = compile ps_4_0 NulPS();
    SetGeometryShader(NULL);
  }
}

technique11 RadialBlur
{
  pass p0
  {
    VertexShader = compile vs_4_0 NulVS();
    PixelShader = compile ps_4_0 RadialBlurPS();
    SetGeometryShader(NULL);
  }
}

technique11 GreyScale
{
  pass p0
  {
    VertexShader = compile vs_4_0 NulVS();
    PixelShader = compile ps_4_0 GreyScalePS();
    SetGeometryShader(NULL);
  }
}

technique11 Sepia
{
  pass p0
  {
    VertexShader = compile vs_4_0 NulVS();
    PixelShader = compile ps_4_0 SepiaPS();
    SetGeometryShader(NULL);
  }
}

technique11 InversedColors
{
  pass p0
  {
    VertexShader = compile vs_4_0 NulVS();
    PixelShader = compile ps_4_0 InversedColorsPS();
    SetGeometryShader(NULL);
  }
}

technique11 CombinedColors
{
  pass p0
  {
    VertexShader = compile vs_4_0 NulVS();
    PixelShader = compile ps_4_0 CombinedColorsPS();
    SetGeometryShader(NULL);
  }
}

technique11 GradualBlur
{
  pass p0
  {
    VertexShader = compile vs_4_0 NulVS();
    PixelShader = compile ps_4_0 GradualBlurPS();
    SetGeometryShader(NULL);
  }
}

technique11 GrowingPeephole
{
  pass p0
  {
    VertexShader = compile vs_4_0 NulVS();
    PixelShader = compile ps_4_0 GrowingPeepholePS();
    SetGeometryShader(NULL);
  }
}

technique11 HorizontalShuttersClose
{
  pass p0
  {
    VertexShader = compile vs_4_0 NulVS();
    PixelShader = compile ps_4_0 HorizontalShuttersClosePS();
    SetGeometryShader(NULL);
  }
}

technique11 HorizontalShuttersOpen
{
  pass p0
  {
    VertexShader = compile vs_4_0 NulVS();
    PixelShader = compile ps_4_0 HorizontalShuttersOpenPS();
    SetGeometryShader(NULL);
  }
}

technique11 RageVision
{
  pass p0
  {
    VertexShader = compile vs_4_0 NulVS();
    PixelShader = compile ps_4_0 RageVisionPS();
    SetGeometryShader(NULL);
  }
}

technique11 Tiles
{
  pass p0
  {
    VertexShader = compile vs_4_0 NulVS();
    PixelShader = compile ps_4_0 TilesPS();
    SetGeometryShader(NULL);
  }
}

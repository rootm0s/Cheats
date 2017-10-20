#include "stdafx.h"

struct tD3D
{
    IDirect3D9Ex* Interface;
    IDirect3DDevice9Ex* Device;
    D3DPRESENT_PARAMETERS Params;
    ID3DXLine* Line;
    ID3DXFont* Font;
    struct tD3DFunctions
    {
        void DrawLine(float X, float Y, float XX, float YY, float LineWidth, D3DCOLOR LineColor, bool Outlined, float OutlineWidth, D3DCOLOR OutlineColor);
        void DrawCircle(float X, float Y, float Radius, float LineWidth, D3DCOLOR LineColor, bool Outlined, float OutlineWidth, D3DCOLOR OutlineColor);
        void DrawRect(float X, float Y, float Width, float Height, float LineWidth, D3DCOLOR LineColor, bool Filled, bool Outlined, float OutlineWidth, D3DCOLOR OutlineColor);
        void DrawCross(float X, float Y, float Size, float LineWidth, D3DCOLOR CrossColor, bool Outlined, float OutlineWidth, D3DCOLOR OutlineColor);
        void DrawString(float X, float Y, std::wstring Text, D3DCOLOR TextColor, bool Outlined, D3DCOLOR OutlineColor, bool Centered);
        bool Setup();
        void BeginRender();
        void EndRender();
        void Destruct();
    } Functions;
};

extern tD3D pD3D;

#include	<d3dx9.h>
#include	<d3d9.h>
#include	<D3dx9tex.h>
#include	<D3dx9shape.h>

typedef const D3DXMATRIX& (* fWorldToScreenMatrix )( );


struct cfn_s
{
	fWorldToScreenMatrix WorldToScreenMatrix;
};

extern cfn_s cfn;

void cfn_init();
float IsVisible( Vector  vEnd, Vector pEnt );
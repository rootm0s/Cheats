#pragma once

#ifndef __GAME_CLASSES_H__
#define __GAME_CLASSES_H__

class IBaseClientDLL;
class IVEngineClient;
class IPanel;
class IClientEntityList;
class ISurface;
class IVModelInfo;
class CGlobalVarsBase;
class IEngineTrace;
class CInput;
class IMaterialSystem;
class IVModelRender;
class IGameMovement; 
class IPrediction;
class IVEfx;
class HFont;
class IVPhysicsDebugOverlay;
class RecvTable;
class RecvProp;
class IHandleEntity;
class CBaseHandle;
class IClientNetworkable;
class IClientEntity;
class IClientThinkable;
class IClientUnknown;
class IClientNetworkable;

typedef void*	(*CreateClientClassFn)( int entnum, int serialNum );
typedef void*	(*CreateEventFn)( );

template< typename Function > Function call_vfunc( PVOID Base, DWORD Index )
{
    PDWORD* VTablePointer = ( PDWORD* )Base;
    PDWORD VTableFunctionBase = *VTablePointer;
    DWORD dwAddress = VTableFunctionBase[ Index ];
    return ( Function )( dwAddress );
}

enum FontDrawType_t
{
	FONT_DRAW_DEFAULT = 0,
	FONT_DRAW_NONADDITIVE,
	FONT_DRAW_ADDITIVE,
	FONT_DRAW_TYPE_COUNT = 2,
};

enum EFontFlags
{
	FONTFLAG_NONE,
	FONTFLAG_ITALIC			= 0x001,
	FONTFLAG_UNDERLINE		= 0x002,
	FONTFLAG_STRIKEOUT		= 0x004,
	FONTFLAG_SYMBOL			= 0x008,
	FONTFLAG_ANTIALIAS		= 0x010,
	FONTFLAG_GAUSSIANBLUR	= 0x020,
	FONTFLAG_ROTARY			= 0x040,
	FONTFLAG_DROPSHADOW		= 0x080,
	FONTFLAG_ADDITIVE		= 0x100,
	FONTFLAG_OUTLINE		= 0x200,
	FONTFLAG_CUSTOM			= 0x400,
	FONTFLAG_BITMAP			= 0x800,
};

class Color
{
public:
	// constructors
	Color()
	{
		*((int *)this) = 0;
	}
	Color(int r,int g,int b)
	{
		SetColor(r, g, b, 0);
	}
	Color(int r,int g,int b,int a)
	{
		SetColor(r, g, b, a);
	}
	void SetColor(int r, int g, int b, int a = 0)
	{
		_color[0] = (unsigned char)r;
		_color[1] = (unsigned char)g;
		_color[2] = (unsigned char)b;
		_color[3] = (unsigned char)a;
	}

	void GetColor(int &r, int &g, int &b, int &a) const
	{
		r = _color[0];
		g = _color[1];
		b = _color[2];
		a = _color[3];
	}

	void SetRawColor( int color32 )
	{
		*((int *)this) = color32;
	}

	int GetRawColor() const
	{
		return *((int *)this);
	}

	inline int r() const	{ return _color[0]; }
	inline int g() const	{ return _color[1]; }
	inline int b() const	{ return _color[2]; }
	inline int a() const	{ return _color[3]; }
	
	unsigned char &operator[](int index)
	{
		return _color[index];
	}

	const unsigned char &operator[](int index) const
	{
		return _color[index];
	}

	bool operator == (const Color &rhs) const
	{
		return ( *((int *)this) == *((int *)&rhs) );
	}

	bool operator != (const Color &rhs) const
	{
		return !(operator==(rhs));
	}

	Color &operator=( const Color &rhs )
	{
		SetRawColor( rhs.GetRawColor() );
		return *this;
	}

private:
	unsigned char _color[4];
};
class player_info_t
{
public:
	char unknown0[0x10];
	char name[32];
	char unknown2[0x200];
};

class CGlobalVarsBase
{
public:
	CGlobalVarsBase( bool bIsClient );
	bool IsClient() const;
	int GetNetworkBase( int nTick, int nEntity );
	float			realtime;
	int				framecount;
	float			absoluteframetime;
	float			curtime;
	float			frametime;
	int				maxClients;
	int				tickcount;
	float			interval_per_tick;
	float			interpolation_amount;
	int				simTicksThisFrame;
	int				network_protocol;
	void *pSaveData;
	bool			m_bClient;
	int				nTimestampNetworkingBase;   
	int				nTimestampRandomizeWindow;  
};
class IBaseClientDLL
{
public:
	void CreateMove( int sequence_number, float frametime, bool active )
	{
		typedef void ( __thiscall* oMove )( PVOID,int,float,bool );
		return call_vfunc< oMove >( this, 21 )( this, sequence_number, frametime, active );
	}
};
class IVEngineClient
{
public:
	void GetScreenSize( int& width, int& height )
	{
		typedef void ( __thiscall* oGetScreenSize )( PVOID, int&, int& );
		return call_vfunc< oGetScreenSize >( this, 5 )( this, width, height );
	}
	bool GetPlayerInfo(int ent_num, player_info_t *pinfo)
	{
		typedef bool ( __thiscall* oGetPlayerInfo )( PVOID, int, player_info_t* );
		return call_vfunc< oGetPlayerInfo >( this, 8 )( this, ent_num, pinfo );
	}
	int GetLocalPlayer(  )
	{
		typedef int ( __thiscall* oLocal )( PVOID );
		return call_vfunc< oLocal >( this, 12 )( this );
	}
	float Time(  )
	{
		typedef float ( __thiscall* oTime )( PVOID );
		return call_vfunc< oTime >( this, 14 )( this );
	}
	void SetViewAngles(Vector& vAngles)
	{
		typedef void ( __thiscall* oSetViewAngles )( PVOID, Vector& );
		return call_vfunc< oSetViewAngles >( this, 19 )( this, vAngles );
	}
	int GetMaxClients( )
	{
		typedef bool ( __thiscall* oGetMaxClients )( PVOID );
		return call_vfunc< oGetMaxClients >( this, 20 )( this );
	}
	bool IsConnected(  )
	{
		typedef bool ( __thiscall* oGetScreenSize )( PVOID );
		return call_vfunc< oGetScreenSize >( this, 27 )( this );
	}
	bool IsInGame(  )
	{
		typedef bool ( __thiscall* oLocal )( PVOID );
		return call_vfunc< oLocal >( this, 26 )( this );
	}
	const D3DXMATRIX& WorldToScreenMatrix( )
	{
		typedef const D3DXMATRIX& ( __thiscall* oWorldToScreenMatrix )( PVOID );
		return call_vfunc< oWorldToScreenMatrix >( this, 37 )( this );
	}
};
class ISurface
{
public:
	void DrawSetColor(Color col)
	{
		typedef void ( __thiscall* oDrawSetColor )( PVOID, Color );
		//return call_vfunc< oDrawSetColor >( this, 13 )( this, col );
		return call_vfunc< oDrawSetColor >( this, 14 )( this, col );
	}
	void DrawFilledRect(int x0, int y0, int x1, int y1)
	{
		typedef void ( __thiscall* oDrawSetColor )( PVOID, int , int , int , int  );
		//return call_vfunc< oDrawSetColor >( this, 15 )( this, x0,  y0,  x1,  y1 );
		return call_vfunc< oDrawSetColor >( this, 16 )( this, x0,  y0,  x1,  y1 );
	}
	void DrawOutlinedRect(int x0, int y0, int x1, int y1)
	{
		typedef void ( __thiscall* oDrawOutlinedRect )( PVOID, int , int , int , int  );
//		return call_vfunc< oDrawOutlinedRect >( this, 17 )( this, x0,  y0,  x1,  y1 );
		return call_vfunc< oDrawOutlinedRect >( this, 18 )( this, x0,  y0,  x1,  y1 );
	}
	void DrawLine(int x0, int y0, int x1, int y1)
	{
		typedef void ( __thiscall* oDrawLine )( PVOID, int , int , int , int  );
//		return call_vfunc< oDrawLine >( this, 18 )( this, x0,  y0,  x1,  y1 );
		return call_vfunc< oDrawLine >( this, 19 )( this, x0,  y0,  x1,  y1 );
	}
	void DrawSetTextFont(unsigned long hFont)
	{
		typedef void ( __thiscall* oDrawSetTextFont )( PVOID, unsigned long  );
		//return call_vfunc< oDrawSetTextFont >( this, 22 )( this, hFont );
		return call_vfunc< oDrawSetTextFont >( this, 23 )( this, hFont );
	}
	void DrawSetTextColor(Color col)
	{
		typedef void ( __thiscall* oDrawSetTextColor )( PVOID, Color  );
		//return call_vfunc< oDrawSetTextColor >( this, 23 )( this, col );
		return call_vfunc< oDrawSetTextColor >( this, 24 )( this, col );
	}
	void DrawSetTextPos(int x,int y)
	{
		typedef void ( __thiscall* oDrawSetTextPos )( PVOID, int ,int  );
		//return call_vfunc< oDrawSetTextPos >( this, 25 )( this,  x, y );
		return call_vfunc< oDrawSetTextPos >( this, 26 )( this,  x, y );
	}
	void DrawPrintText(wchar_t *text, int textLen, FontDrawType_t drawType = FONT_DRAW_DEFAULT)
	{
		typedef void ( __thiscall* oDrawPrintText )( PVOID, wchar_t*, int, FontDrawType_t  );
		//return call_vfunc< oDrawPrintText >( this, 27 )( this,  text, textLen, drawType );
		return call_vfunc< oDrawPrintText >( this, 28 )( this,  text, textLen, drawType );
	}
	unsigned long FontCreate()
	{
		typedef unsigned long ( __thiscall* oCreateFont )( PVOID );
		//return call_vfunc< oCreateFont >( this, 70 )( this );
		return call_vfunc< oCreateFont >( this, 71 )( this );
	}
	bool SetFontGlyphSet(unsigned long font, const char *windowsFontName, int tall, int weight, int blur, int scanlines, int flags, int nRangeMin = 0, int nRangeMax = 0)
	{
		typedef bool( __thiscall* oCreateFont )( PVOID, unsigned long , const char *, int , int , int , int , int, int, int );
		//return call_vfunc< oCreateFont >( this, 71 )( this, font, windowsFontName,  tall,  weight,  blur,  scanlines, flags, nRangeMin, nRangeMax );
		return call_vfunc< oCreateFont >( this, 72 )( this, font, windowsFontName,  tall,  weight,  blur,  scanlines, flags, nRangeMin, nRangeMax );
	}
	bool GetTextSize(unsigned long font, const wchar_t *text, int &wide, int &tall )
	{
		typedef bool( __thiscall* oGetTextSize )( PVOID, unsigned long, const wchar_t*, int&, int& );
		//return call_vfunc< oGetTextSize >( this, 78 )( this, font, text, wide, tall );
		return call_vfunc< oGetTextSize >( this, 79 )( this, font, text, wide, tall );
	}
};//Size=0x0004(4)
class IClientRenderable
{
public:
	virtual void*					GetIClientUnknown() = 0;
	virtual Vector const&			GetRenderOrigin( void ) = 0;
	virtual Vector const&			GetRenderAngles( void ) = 0;
	virtual bool					ShouldDraw( void ) = 0;
	virtual bool					IsTransparent( void ) = 0;
	virtual bool					UsesPowerOfTwoFrameBufferTexture() = 0;
	virtual bool					UsesFullFrameBufferTexture() = 0;
	virtual void					GetShadowHandle() const = 0;
	virtual void*					RenderHandle() = 0;
	virtual const void*/*model_t* */GetModel( ) const = 0;
	virtual int						DrawModel( int flags ) = 0;
	virtual int						GetBody() = 0;
	virtual void					ComputeFxBlend( ) = 0;

	bool SetupBones( matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
	{
		typedef bool( __thiscall* oSetupBones )( PVOID, matrix3x4_t*, int, int, float );
		return call_vfunc< oSetupBones>( this, 13 )( this,pBoneToWorldOut, nMaxBones, boneMask, currentTime );
	}
};
class IClientNetworkable
{
public:
	virtual IClientUnknown*	GetIClientUnknown() = 0;	
	virtual void			Release() = 0;	
	virtual void*			GetClientClass() = 0;// FOR NETVARS FIND YOURSELF ClientClass* stuffs
	virtual void			NotifyShouldTransmit( /* ShouldTransmitState_t state*/ ) = 0;
	virtual void			OnPreDataChanged( /*DataUpdateType_t updateType*/ ) = 0;
	virtual void			OnDataChanged( /*DataUpdateType_t updateType*/ ) = 0;
	virtual void			PreDataUpdate( /*DataUpdateType_t updateType*/ ) = 0;
	virtual void			PostDataUpdate( /*DataUpdateType_t updateType*/ ) = 0;	
	virtual void			unknown();
	virtual bool			IsDormant( void ) = 0;
	virtual int				Index( void ) const = 0;	
	virtual void			ReceiveMessage( int classID /*, bf_read &msg*/ ) = 0;	
	virtual void*			GetDataTableBasePtr() = 0;
	virtual void			SetDestroyedOnRecreateEntities( void ) = 0;
};
class IClientUnknown
{
public:
	virtual void*		GetCollideable() = 0;
	virtual IClientNetworkable*	GetClientNetworkable() = 0;
	virtual IClientRenderable*	GetClientRenderable() = 0;
	virtual IClientEntity*		GetIClientEntity() = 0;
	virtual IClientEntity*		GetBaseEntity() = 0;
	virtual IClientThinkable*	GetClientThinkable() = 0;
};
class IClientThinkable
{
public:
	virtual IClientUnknown*		GetIClientUnknown() = 0;
	virtual void				ClientThink() = 0;
	virtual void*				GetThinkHandle() = 0;
	virtual void				SetThinkHandle( void* hThink ) = 0;
	virtual void				Release() = 0;
};
class __declspec ( novtable )IClientEntity : public IClientUnknown, public IClientRenderable, public IClientNetworkable, public IClientThinkable
{
public:
	public:
		virtual void			Release( void ) = 0;
		virtual void			blahblahpad( void ) = 0;
		virtual Vector&	GetAbsOrigin( void ) const = 0;//in broken place use GetOrigin Below
		virtual const Vector&	GetAbsAngles( void ) const = 0;
		
		Vector GetOrigin()	
		{ 
			return *reinterpret_cast< Vector* >( ( DWORD)this + ( DWORD )0x134 );
		}
		int GetTeamNum( )		
		{ 
			return *reinterpret_cast< int* >( ( DWORD)this + ( DWORD )0xF0 );
		}
		int GetFlags()		
		{ 
			return *reinterpret_cast< int* >( ( DWORD)this + ( DWORD )0x100 );
		}
};

class IClientEntityList
{
public:
	virtual void Function0();
	virtual void Function1();
	virtual void Function2();
	virtual IClientEntity *		GetClientEntity( int entnum );
	virtual IClientEntity *		GetClientEntityFromHandle( HANDLE hEnt ) = 0;
	virtual int					NumberOfEntities( bool bIncludeNonNetworkable ) = 0;
	virtual int					GetHighestEntityIndex( void );
	virtual void				SetMaxEntities( int maxents );
	virtual int					GetMaxEntities( );
};
class IPanel
{
public:
	const char *GetName(unsigned int vguiPanel)
	{
		typedef const char *( __thiscall* oGetName )( PVOID, unsigned int );
		return call_vfunc< oGetName >( this, 36 )( this, vguiPanel );
	}
	void PaintTraverse(unsigned int vguiPanel, bool forceRepaint, bool allowForce = true)
	{
		typedef void( __thiscall* oPaintTraverse )( PVOID, unsigned int, bool, bool );
		return call_vfunc< oPaintTraverse >( this, 41 )( this, vguiPanel, forceRepaint, allowForce );
	}
};
class CUserCmd
{
public:	
	virtual ~CUserCmd();//0x00
	int		command_number;//0x04
	int		tick_count;//0x08
	Vector viewangles;//0x0C
	Vector	aimdirection; 
	float	forwardmove; 
	float	sidemove;    
	float	upmove; 
	int		buttons; //0x30
	BYTE    impulse; //0x35
	int		weaponselect;//0x36	
	int		weaponsubtype;//0x3A
	int		random_seed; //0x3E
	short	mousedx;//0x42		
	short	mousedy;//0x43		
	bool	hasbeenpredicted; 
	Vector headangles;
	Vector	headoffset;	

	CRC32_t GetChecksum( void ) const
	{
		CRC32_t crc;

		CRC32_Init( &crc );
		CRC32_ProcessBuffer( &crc, &command_number, sizeof( command_number ) );
		CRC32_ProcessBuffer( &crc, &tick_count, sizeof( tick_count ) );
		CRC32_ProcessBuffer( &crc, &viewangles, sizeof( viewangles ) );  
		CRC32_ProcessBuffer( &crc, &aimdirection, sizeof( aimdirection ) );
		CRC32_ProcessBuffer( &crc, &forwardmove, sizeof( forwardmove ) );
		CRC32_ProcessBuffer( &crc, &sidemove, sizeof( sidemove ) );
		CRC32_ProcessBuffer( &crc, &upmove, sizeof( upmove ) );
		CRC32_ProcessBuffer( &crc, &buttons, sizeof( buttons ) );
		CRC32_ProcessBuffer( &crc, &impulse, sizeof( impulse ) );        
		CRC32_ProcessBuffer( &crc, &weaponselect, sizeof( weaponselect ) );    
		CRC32_ProcessBuffer( &crc, &weaponsubtype, sizeof( weaponsubtype ) );
		CRC32_ProcessBuffer( &crc, &random_seed, sizeof( random_seed ) );
		CRC32_ProcessBuffer( &crc, &mousedx, sizeof( mousedx ) );
		CRC32_ProcessBuffer( &crc, &mousedy, sizeof( mousedy ) );
		CRC32_ProcessBuffer( &crc, &hasbeenpredicted, sizeof( hasbeenpredicted ) );
		CRC32_ProcessBuffer( &crc, &headangles, sizeof( headangles ) );
		CRC32_ProcessBuffer( &crc, &headoffset, sizeof( headoffset ) );        
		CRC32_Final( &crc );

		return crc;
	}	
};
class CInput
{
public:	
	virtual		void		Init_All( void );
	virtual		void		Shutdown_All( void );
	virtual		int			GetButtonBits( int );
	virtual		void		CreateMove ( int sequence_number, float input_sample_frametime, bool active );
	virtual		void		ExtraMouseSample( float frametime, bool active );
	virtual		bool		WriteUsercmdDeltaToBuffer( /*bf_write*/int *buf, int from, int to, bool isnewcommand );
	virtual		void		EncodeUserCmdToBuffer( /*bf_write&*/ int buf, int slot );
	virtual		void		DecodeUserCmdFromBuffer( /*bf_read&*/int buf, int slot );	
	virtual		CUserCmd	*GetUserCmd( int something, int sequence_number );

	class CVerifiedUserCmd
	{
	public:
		CUserCmd        m_cmd;
		CRC32_t         m_crc;
	};

	CUserCmd         *m_pCommands;
	CVerifiedUserCmd *m_pVerifiedCommands;
};

extern IBaseClientDLL*			pClient				;
extern IVEngineClient*			pEngine				;
extern IPanel*					pPanel				;
extern IClientEntityList*		pClientEntList		;
extern ISurface*				pSurface			;
extern CGlobalVarsBase*		pGlobals			;
extern IEngineTrace*			pEngineTrace		;
extern unsigned long			espfont				;
#endif
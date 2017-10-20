#pragma once

#pragma warning (disable: 4244)
#pragma warning (disable: 4715)
#pragma warning (disable: 4305)

#define WIN32_LEAN_AND_MEAN

#define hackstr "csgo hack "__DATE__
#define date __DATE__

#include "windows.h"
#include "stdio.h"
#include "stdlib.h"
#include "math.h"
#include "md5.h"

inline long Virtual(void* p,int i) { return (*(long**)p)[i]; }

typedef void (_cdecl* MsgFn)(char*,...);

extern MsgFn Msg;

#define cmp(a,b) !_stricmp(a,b)
#define clamp(a,b,c) max(min(a,c),b);

extern long clientbase;
extern long enginebase;

#include "vec.h"

#include "netvars.h"

extern NetvarManager* netvars;

#include "entity.h"

extern Entity* local;
extern Entity* weapon;

#include "other.h"
#include "interfaces.h"

extern BaseClient* client;
extern BaseEngine* engine;
extern BaseVGUI* vgui;

#include "game.h"
#include "aimbot.h"
#include "prediction.h"
#include "esp.h"
#include "glow.h"
#include "cvars.h"
#include "gui.h"

extern GameUtil* game;
extern AimbotManager* aimbot;
extern PredictionManager* prediction;
extern ESPManager* esp;
extern GlowManager* glow;
extern CvarManager* cvars;
extern GUIManager* gui;

extern GlobalVars* globals;

bool CreateMove(float,UserCmd*);
bool InPrediction();
void RunCommand(Entity*,UserCmd*,void*);
bool KeyEvent(void*);
void PaintTraverse(int,bool,bool);
void DrawModel(void*,void*,RenderInfo*,void*);
void SceneEnd();
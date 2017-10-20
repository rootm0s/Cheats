#include "dll.h"

void GlowManager::Think()
{
	if (!cvars->esp_outline)
		return;

	if (!engine->context)
		return;

	if (!local || esp->index == -1)
		return;

	Entity* entity;
	int i;

	sten.enable = true;
	sten.pass = 3;
	sten.fail = 1;
	sten.zfail = 3;
	sten.compare = 8;
	
	engine->SetStencilState(sten);

	engine->SetBlend(0);

	for (i=0; i <= esp->index; ++i)
	{
		if (!(entity = client->GetEntity(esp->todraw[i])))
			continue;

		if (entity->GetTeam() == local->GetTeam())
			continue;

		entity->DrawModel();
	}

	engine->SetBlend(1);

	sten.compare = 6;

	engine->SetStencilState(sten);

	engine->FindVar("r_drawothermodels")->ivalue = 2;

	for (i=0; i <= esp->index; ++i)
	{
		if (!(entity = client->GetEntity(esp->todraw[i])))
			continue;

		if (entity->GetTeam() == local->GetTeam())
			continue;

		//engine->SetColor(0,1,0);

		entity->DrawModel();
	}

	engine->FindVar("r_drawothermodels")->ivalue = 1;

	engine->SetColor(1,1,1);

	sten.enable = false;

	engine->SetStencilState(sten);
}
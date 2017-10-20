#pragma once
#include "main.h"

namespace ValveSDK
{
	class IVModelInfo
	{
	public:
		int GetModelIndex(const char* ModelName)
		{
			typedef int(__thiscall* OriginalFn)(PVOID, const char*);
			return getvfunc<OriginalFn>(this, 2)(this, ModelName);
		}
		const char* GetModelName(const model_t *model)
		{
			typedef const char* (__thiscall* OriginalFn)(PVOID, const model_t*);
			return getvfunc<OriginalFn>(this, 3)(this, model);
		}
		studiohdr_t     *GetStudiomodel(const model_t *mod)
		{
			typedef studiohdr_t* (__thiscall* OriginalFn)(PVOID, const model_t*);
			return getvfunc<OriginalFn>(this, 30)(this, mod);
		}
		void GetModelMaterials(const model_t *model, int count, IMaterial** ppMaterial)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, const model_t*, int, IMaterial**);
			getvfunc<OriginalFn>(this, 17)(this, model, count, ppMaterial);
		}
		void SetMaterialVarFlag(MaterialVarFlags_t flag, bool on)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, MaterialVarFlags_t, bool);
			getvfunc<OriginalFn>(this, 29)(this, flag, on);
		}
		void SetColorModulation(float const* blend)
		{
			typedef void(__thiscall* OriginalFn)(PVOID, float const*);
			return getvfunc<OriginalFn>(this, 6)(this, blend);
		}
	};
}
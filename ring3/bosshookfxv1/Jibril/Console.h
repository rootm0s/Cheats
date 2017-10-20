#pragma once

#include "Includes.h"

struct sMenuItem{
	std::string m_Name;
	
	union{
		float* m_Float;
		int* m_Int;
		bool* m_Bool;
	};

	float m_FloatStep;
	int m_IntStep;
	int m_Type;

	union{
		float m_FloatMin;
		int m_IntMin;
	};

	union{
		float m_FloatMax;
		int m_IntMax;
	};
};

class CMenu{
public:
	void InsertMenuItems();
	void DrawMenu();
};

struct CVar{
	float flValue;
	std::string szName;

	template <typename f>
	f GetValue(){
		return flValue;
	}
};

struct Console{
public:
	std::vector<CVar*> CVars;
	bool bActive;



	std::vector<CVar*> autoCompleteCVars;
	std::vector<CVar> doAutoComplete(){
		std::vector<CVar*> cVars;
		for (size_t i = 0; i < CVars.size(); ++i){
			if ((int)CVars.at(i)->szName.find())
		}
	}
private:

};


extern Console* g_pConsole;
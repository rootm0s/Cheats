#ifndef BHOP_H
#define BHOP_H
#include "CGeneralEntity.h"
#include "ProcMem.h"
#include "Offsets.h"

class Bhop {
public:
	void Tick(CGeneralEntity);
private:
	int Jump = 6;
	int Jump2 = 5;
};

#endif
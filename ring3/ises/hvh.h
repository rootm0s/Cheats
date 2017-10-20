#include "main.h"

using namespace std;

bool XJitter = true;
bool YJitter = true;
int iSpin = 0;

void SilentXSpin(CUserCmd* cmd, int& iSpinx) {
	iSpinx++;

	if (iSpinx > 1) {
		iSpinx = 0;
	}

	if (iSpinx == 1) {
		cmd->viewangles.x = 89;
	}
}

void CorrectLisp(CUserCmd* pCmd) {
	if (GetAsyncKeyState('W'))
		pCmd->forwardmove = 450.f;
	if (GetAsyncKeyState('S'))
		pCmd->forwardmove = -450.f;
}

void SlowSpin(CUserCmd *pCmd)
{
	int random = rand() % 100;
	int random2 = rand() % 1000;

	static bool dir;
	static float current_y = pCmd->viewangles.y;

	if (random == 1) dir = !dir;

	if (dir)
		current_y += 5;
	else
		current_y -= 5;

	pCmd->viewangles.y = current_y;

	if (random == random2)
		pCmd->viewangles.y += random;

}

void FastSpint(CUserCmd *pCmd)
{
	int random = 160 + rand() % 40;
	static float current_y = pCmd->viewangles.y;
	current_y += random;
	pCmd->viewangles.y = current_y;
}



void DoAntiAim(CUserCmd* cmd, UISlider x, UISlider y) {
	if (x.getValue() == 1) {
		cmd->viewangles.x = 89;
	}
	if (x.getValue() == 2) {
		cmd->viewangles.x = 180;
	}
	
	if (x.getValue() == 3) {
		cmd->viewangles.x = -89;
	}
	if (x.getValue() == 4) {
		cmd->viewangles.x = 0;
	}
	if (x.getValue() == 5) {
		if (XJitter == true) {
			cmd->viewangles.x = -89;
			XJitter = !XJitter;
		}
		else {
			cmd->viewangles.x = 89;
			XJitter = !XJitter;
		}
	}

	/* Y */
	if (y.getValue() == 1) {
		cmd->viewangles.y = 0;
	}
	if (y.getValue() == 2) {


		bool yNightmare = false;
		if (yNightmare)
			cmd->viewangles.y -= 180;
		else
			cmd->viewangles.y += 180;

		yNightmare = !yNightmare;


	}
	if (y.getValue() == 3) {
		cmd->viewangles.y += 90.f;
	}
	if (y.getValue() == 4) {
		if (aimbotAngle.x == 0 || aimbotAngle.y == 0)
			cmd->viewangles.y = cmd->viewangles.y + 180;
		else
			cmd->viewangles.y = aimbotAngle.y - 180;
	}
	if (y.getValue() == 5) {
		if (YJitter == true) {
			cmd->viewangles.y += 140;
			YJitter = !YJitter;
		}
		else {
			cmd->viewangles.y -= 140;
			YJitter = !YJitter;
		}
	}
	if (y.getValue() == 6) {
		if (YJitter == true) {
			cmd->viewangles.y -= 140;
			YJitter = !YJitter;
		}
		else {
			cmd->viewangles.y += 140;
			YJitter = !YJitter;
		}
	}

	if (y.getValue() == 7) {
		FastSpint(cmd);
	}

	if (y.getValue() == 8) {
		SlowSpin(cmd);
	}

	if (y.getValue() == 9) {
		cmd->viewangles.y += 280;
	}


}
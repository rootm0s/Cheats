class CHvH
{
public:
	bool bSideways = false;
	int iSpinbot = 180;
	int iFakeLag = 0;
	void AntiAim(CUserCmd*, bool);
	void FakeLag(CUserCmd*);
	void Normalize(Vector &, Vector &);
	void AirStuck(CUserCmd*);
}; CHvH *HvH = new CHvH;

void CHvH::AntiAim(CUserCmd* cmd, bool bForce = false)
{
	if (!Vars::HvH::AntiAim || (!bForce && (cmd->buttons & IN_ATTACK)))
		return;

	float forward = cmd->forwardmove;
	float right = cmd->sidemove;
	float up = cmd->upmove;
	Vector viewforward, viewright, viewup, aimforward, aimright, aimup, vForwardNorm, vRightNorm, vUpNorm;
	AngleVectors(cmd->viewangles, &viewforward, &viewright, &viewup);

	QAngle qAngle = QAngle(0, 0, 0);

	if (Vars::HvH::AntiAimType != 1)
	{
		if (Vars::HvH::AntiAimType == 2)
			qAngle.x = 181;
		else if (Vars::HvH::AntiAimType == 3)
			qAngle.x = -181;
		else if (Vars::HvH::AntiAimType == 4)
			qAngle.x = 180 + rand() % 90;
		else if (Vars::HvH::AntiAimType == 5)
			qAngle.x = -85;
		else if (Vars::HvH::AntiAimType == 6)
			qAngle.x = 85;
		else
			qAngle.x = 180;
	}

	if (Vars::HvH::AntiAimTypeY != 1)
	{
		if (Vars::HvH::AntiAimTypeY == 2)
		{
			bSideways = !bSideways;
			qAngle.y = bSideways ? cmd->viewangles.y + 90 : cmd->viewangles.y - 90;
			if (!Vars::HvH::FakeLag)
				*bSendPacket = bSideways ? TRUE : FALSE;
		}
		else if (Vars::HvH::AntiAimTypeY == 3)
			qAngle.y = 360;
		else if (Vars::HvH::AntiAimTypeY == 4)
			qAngle.y = 540;
		else if (Vars::HvH::AntiAimTypeY == 5)
			qAngle.y = cmd->viewangles.y - 90;
		else if (Vars::HvH::AntiAimTypeY == 6)
			qAngle.y = cmd->viewangles.y + 90;
		else if (Vars::HvH::AntiAimTypeY == 7)
			qAngle.y = cmd->viewangles.y + 180;
		else if (Vars::HvH::AntiAimTypeY == 8)
		{
			iSpinbot = iSpinbot + 20;
			qAngle.y = iSpinbot;

			if (iSpinbot >= 720)
				iSpinbot = 180;
		}
		else
			qAngle.y = 180;
	}

	cmd->viewangles = qAngle;

	AngleVectors(cmd->viewangles, &aimforward, &aimright, &aimup);
	Normalize(viewforward, vForwardNorm);
	Normalize(viewright, vRightNorm);
	Normalize(viewup, vUpNorm);
	cmd->forwardmove = DotProduct(forward* vForwardNorm, aimforward) + DotProduct(right* vRightNorm, aimforward) + DotProduct(up* vUpNorm, aimforward);
	cmd->sidemove = DotProduct(forward* vForwardNorm, aimright) + DotProduct(right* vRightNorm, aimright) + DotProduct(up* vUpNorm, aimright);
	cmd->upmove = DotProduct(forward* vForwardNorm, aimup) + DotProduct(right* vRightNorm, aimup) + DotProduct(up* vUpNorm, aimup);
}

void CHvH::FakeLag(CUserCmd* cmd)
{
	if (!Vars::HvH::FakeLag || Vars::HvH::FakeLagChoke == 0 || Vars::HvH::FakeLagSend == 0)
		return;

	int iFakeLagTickLimit = Vars::HvH::FakeLagChoke + Vars::HvH::FakeLagSend;
	if (iFakeLag > iFakeLagTickLimit)
		iFakeLag = 1;
	else
		iFakeLag = iFakeLag + 1;

	if (Vars::HvH::FakeLagSend >= iFakeLag)
		*bSendPacket = TRUE;
	else
		*bSendPacket = FALSE;
}

void CHvH::Normalize(Vector &vecIn, Vector &vecOut){
	float flLen = vecIn.Length();

	if (flLen == 0){
		vecOut.Init(0, 0, 0);
		return;
	}

	flLen = 1 / flLen;
	vecOut.Init(vecIn.x * flLen, vecIn.y * flLen, vecIn.z * flLen);
}

void CHvH::AirStuck(CUserCmd* cmd)
{
	if (!Vars::HvH::AirStuck || !GetAsyncKeyState(VK_LCONTROL)) //  || cmd->buttons & IN_ATTACK
		return;

	cmd->tick_count = 0xFFFFF;
}
namespace ValveSDK
{
	class CGlobalVars
	{
	public:
		float realtime;
		int framecount;
		float absoluteframetime;
		float fNew;
		float curtime;
		float frametime;
		int maxclients;
		int tickcount;
		float interval_per_tick;
		float interpolation_amount;
		int	simTicksThisFrame;
		int	network_protocol;
		PVOID pSaveData;
		bool m_bClient;
		int	nTimestampNetworkingBase;
		int	nTimestampRandomizeWindow;
	};
}
class GlowManager
{
public:
	GlowManager()
	{
		sten.ref = 1;
		sten.mask1 = 0xFFFFFFFF;
		sten.mask2 = 0xFFFFFFFF;
	}
	void Think();

	StencilState sten;
};
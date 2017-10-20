// ALL CREDITS GO TO JOAN @UNKNOWNCHEATS.ME, THANKS!

class TextureHolder
{
public:
	TextureHolder(const unsigned char* pRawRGBAData, uint32 W, uint32 H)
	{
		m_iTexture = Interfaces->Surface->CreateNewTextureID(true);

		if (!m_iTexture)
			return;

		Interfaces->Surface->DrawSetTextureRGBA(m_iTexture, pRawRGBAData, W, H);
		m_bValid = true;
	};

	bool IsValid() const
	{
		return m_bValid;
	};

	int GetTextureId() const
	{
		return m_iTexture;
	};

	bool Draw(int x, int y, float scale = 1.0)
	{
		if (!Interfaces->Surface->IsTextureIDValid(m_iTexture))
			return false;

		Interfaces->Surface->DrawSetColor(m_bgColor);
		Interfaces->Surface->DrawSetTexture(m_iTexture);
		Interfaces->Surface->DrawTexturedRect(x, y, x + m_iW * scale, y + m_iH * scale);
		return true;
	};

protected:
	uint32 m_iTexture;
	uint32 m_iW, m_iH;
	Color  m_bgColor;
	bool   m_bValid;
};
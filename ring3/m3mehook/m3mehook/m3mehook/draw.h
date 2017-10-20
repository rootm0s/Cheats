namespace Draw
{
	void DrawStringA(HFont font, int x, int y, Color color, const char* msg, ...)
	{
		va_list va_alist;
		char buf[1024];
		va_start(va_alist, msg);
		_vsnprintf(buf, sizeof(buf), msg, va_alist);
		va_end(va_alist);
		wchar_t wbuf[1024];
		MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

		int r = 255, g = 255, b = 255, a = 255;
		color.GetColor(r, g, b, a);

		int Wid, Hit;

		Interfaces->Surface->GetTextSize(font, wbuf, Wid, Hit);
		Interfaces->Surface->DrawSetTextFont(font);
		Interfaces->Surface->DrawSetTextColor(r, g, b, a);
		Interfaces->Surface->DrawSetTextPos(x - Wid / 2, y - Hit / 2);
		Interfaces->Surface->DrawPrintText(wbuf, wcslen(wbuf));
	}

	void DrawStringB(HFont font, int x, int y, Color color, const char* msg, ...)
	{
		va_list va_alist;
		char buf[1024];
		va_start(va_alist, msg);
		_vsnprintf(buf, sizeof(buf), msg, va_alist);
		va_end(va_alist);
		wchar_t wbuf[1024];
		MultiByteToWideChar(CP_UTF8, 0, buf, 256, wbuf, 256);

		int r = 255, g = 255, b = 255, a = 255;
		color.GetColor(r, g, b, a);

		int Wid, Hit;

		Interfaces->Surface->GetTextSize(font, wbuf, Wid, Hit);
		Interfaces->Surface->DrawSetTextFont(font);
		Interfaces->Surface->DrawSetTextColor(r, g, b, a);
		Interfaces->Surface->DrawSetTextPos(x, y - Hit / 2);
		Interfaces->Surface->DrawPrintText(wbuf, wcslen(wbuf));
	}

	void DrawRect(int x, int y, int w, int h, Color col)
	{
		Interfaces->Surface->DrawSetColor(col);
		Interfaces->Surface->DrawFilledRect(x, y, x + w, y + h);
	}

	void DrawOutlinedRect(int x, int y, int w, int h, Color col)
	{
		Interfaces->Surface->DrawSetColor(col);
		Interfaces->Surface->DrawOutlinedRect(x, y, x + w, y + h);
	}

	void DrawLine(int x, int y, int x2, int y2, Color col)
	{
		Interfaces->Surface->DrawSetColor(col);
		Interfaces->Surface->DrawLine(x, y, x2, y2);
	}
} 

bool ScreenTransform(const Vector &point, Vector &screen) // made by me!!!!!!!
{
	float w;
	const VMatrix &worldToScreen = Interfaces->Engine->WorldToScreenMatrix();

	screen.x = worldToScreen[0][0] * point[0] + worldToScreen[0][1] * point[1] + worldToScreen[0][2] * point[2] + worldToScreen[0][3];
	screen.y = worldToScreen[1][0] * point[0] + worldToScreen[1][1] * point[1] + worldToScreen[1][2] * point[2] + worldToScreen[1][3];
	w = worldToScreen[3][0] * point[0] + worldToScreen[3][1] * point[1] + worldToScreen[3][2] * point[2] + worldToScreen[3][3];
	screen.z = 0.0f;

	bool behind = false;

	if (w < 0.001f)
	{
		behind = true;
		screen.x *= 100000;
		screen.y *= 100000;
	}
	else
	{
		behind = false;
		float invw = 1.0f / w;
		screen.x *= invw;
		screen.y *= invw;
	}
	return behind;
}

bool WorldToScreen(const Vector &origin, Vector &screen, int ScreenWidth, int ScreenHeight) // totally not pasted
{
	if (!ScreenTransform(origin, screen))
	{
		float x = ScreenWidth / 2;
		float y = ScreenHeight / 2;
		x += 0.5 * screen.x * ScreenWidth + 0.5;
		y -= 0.5 * screen.y * ScreenHeight + 0.5;
		screen.x = x;
		screen.y = y;
		return true;
	}

	return false;
}
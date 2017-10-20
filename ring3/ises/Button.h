#include "main.h"

class UIButton {
protected:
	int CoordX;
	int CoordY;
	int Width;
	int Height;
	bool Active;

	bool clicked;

	Color buttonColor;
	Color outlineColor;

	char* text;
public:

	int GetCoordX()
	{
		return this->CoordX;
	}

	int GetCoordY()
	{
		return this->CoordY;
	}

	int GetWidth()
	{
		return this->Width;
	}

	int GetHeight()
	{
		return this->Height;
	}

	bool getEvent() {
		if (this->clicked == true) {
			return true;
			this->clicked = false;
		}
		else {
			return false;
		}
	}

	bool GetActive()
	{
		return this->Active;
	}

	char* getText() {
		return this->text;
	}

	Color GetColor() {
		return this->buttonColor;
	}

	void SetCoordX(int Value)
	{
		this->CoordX = Value;
	}

	void SetCoordY(int Value)
	{
		this->CoordY = Value;
	}

	void SetWidth(int Value)
	{
		this->Width = Value;
	}

	void SetHeight(int Value)
	{
		this->Height = Value;
	}

	void SetActive(bool Value)
	{
		this->Active = Value;
	}

	void SetText(char* Value) {
		this->text = Value;
	}

	void initColor() {
		this->buttonColor = Color(255, 128, 34, 255);
		this->outlineColor = Color(60, 60, 60, 255);
	}

	void MouseMove()
	{
		if (CursorInArea(this->CoordX, this->CoordY, this->Width, this->Height)) {
			this->outlineColor = Color(130, 130, 130, 255);
		}
		else
		{
			this->outlineColor = Color(60, 60, 60, 255);
		}
	}

	void onMouseClick() {
		if (CursorInArea(this->CoordX, this->CoordY, this->Width, this->Height)) {
			this->clicked = true;
		}
	}

	void render() {
		if (this->Active) {
			FillRGB(this->CoordX + 1, this->CoordY + 1, this->Width - 1, this->Height - 1, this->buttonColor);
			DrawBox(this->CoordX, this->CoordY, this->Width, this->Height, this->outlineColor);
			DrawStringMiddle(pFont, true, this->CoordX + 1 + this->Width / 2, this->CoordY + 1 + this->Height / 2, Color(255, 255, 255, 255), this->text);
		}
	}


};
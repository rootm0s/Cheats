#include "main.h"
#include <string>

class UISlider {
protected:
	int CoordX;
	int CoordY;
	int Width;
	bool Active;

	int sliderValue;
	int maxSliderValue;

	Color itemColor;
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


	bool GetActive()
	{
		return this->Active;
	}

	int getValue() {
		return this->sliderValue;
	}

	int getMaxSliderValue() {
		return this->maxSliderValue;
	}

	char* getText() {
		return this->text;
	}

	Color GetColor() {
		return this->itemColor;
	}

	void setPos(int x, int y)
	{
		this->CoordX = x; this->CoordY = y;
	}

	void SetWidth(int Value)
	{
		this->Width = Value;
	}

	void SetActive(bool Value)
	{
		this->Active = Value;
	}

	void SetText(char* Value) {
		this->text = Value;
	}

	void setValue(int Valve) {
		this->sliderValue = Valve;
	}

	void setMaxSliderValue(int Value) {
		this->maxSliderValue = Value;
	}

	void initColor() {
		this->itemColor = Color(32, 32, 32, 255);
		this->outlineColor = Color(40, 40, 40, 255);
	}

	void MouseMove()
	{
		/*
		if (CursorInArea(this->CoordX, this->CoordY, this->Width, this->Height)) {
		this->outlineColor = Color(110, 110, 110, 255);
		}
		else
		{
		this->outlineColor = Color(40, 40, 40, 255);
		}
		*/
	}

	void onMouseHold() {
		if (CursorInArea(this->CoordX, this->CoordY, this->Width, 20)) {
			this->sliderValue = CalcPos(this->CoordX, this->CoordY, this->Width, 20) / ((float)this->Width / (float)(this->maxSliderValue));
		}
	}

	void onMouseClick() {

	}

	void Render() {
		DrawBox(this->CoordX, this->CoordY, this->Width, 20, themeColor);
		FillRGB(this->CoordX, this->CoordY, this->sliderValue * ((float)this->Width / (float)(this->maxSliderValue)), 20, themeColor);

		std::string s = std::to_string(this->sliderValue);
		char const *pchar = s.c_str();

		DrawString(this->CoordX + 7, this->CoordY + 4, Color(255, 255, 255, 255), pFont, this->text);
		DrawStringMiddle(pFont, true, this->CoordX + this->Width / 2, this->CoordY + 9, Color(255, 255, 255, 255), pchar);

	}
};

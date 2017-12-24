
#pragma once

#include "WinWidget.h"
#include "native/NativeScrollBar.h"

class EUIScrollBar;

class WinScrollBar : public NativeScrollBar
{

public:

	WinScrollBar(EUIWidget* owner);
	virtual ~WinScrollBar();

	EUIScrollBar* Owner();

	virtual bool ProcessWidget(long msg, WPARAM wParam, LPARAM lParam);

	virtual void SetPosition(int pos);
	void SetLimitImpl();
	virtual void SetLimit();
};

#include "EUIWindow.h"
#include "native/win/WinWindow.h"

EUIWindow::EUIWindow(const char* txt, bool popup, bool adjust, int set_x, int set_y, int w, int h) : EUIWidget(NULL, txt)
{
	need_strict_size = false;

	active = false;

	x = set_x;
	y = set_y;
	width = w;
	height = h;

	visible = false;

	is_minimazed = false;

	nativeWidget = new WinWindow(this, popup, adjust);
}

EUIWindow::~EUIWindow()
{
}

NativeWindow* EUIWindow::Native()
{
	return (NativeWindow*)nativeWidget;
}

void EUIWindow::SetText(const char* txt)
{
	EUIWidget::SetText(txt);
	Native()->SetText(txt);
}

bool EUIWindow::IsActive()
{
	return active;
}

void EUIWindow::SetAtScreenCenter()
{
	Native()->SetAtScreenCenter();
}

void EUIWindow::Maximaze()
{	
	if (IsVisible())
	{
		Native()->Maximaze();
	}
}

void EUIWindow::Close()
{
	Native()->Close();
}

bool EUIWindow::IsMinimazed()
{
	return is_minimazed;
}

void EUIWindow::SetNeedStrictSize(bool set_strict)
{
	need_strict_size = set_strict;
}
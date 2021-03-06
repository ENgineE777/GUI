
#include "EUIWidget.h"
#include "WinDX11Widget.h"
#include "EUIWindow.h"
#include "WinDX11Window.h"

#ifdef PLATFORM_WIN_DX11

int WinDX11Widget::timerID = 0;

std::vector<WinDX11Widget*> win_dx11_timers;

VOID CALLBACK WinDX11WidgetTimerProc(HWND hwnd, UINT message, UINT idTimer, DWORD dwTime)
{
	for (auto& win_dx11 : win_dx11_timers)
	{
		//if (!win_dx11->owner->IsVisible() || !win_dx11->owner->IsEnabled())
		{
			//continue;
		}

		if (win_dx11->cur_timerID == idTimer)
		{
			win_dx11->OnTimer();
			break;
		}
	}
}

WinDX11Widget::WinDX11Widget(EUIWidget* set_owner) : NativeWidget(set_owner)
{
	CalcGlopalPos();
}

WinDX11Widget::~WinDX11Widget()
{
	WinDX11Widget* root = ((WinDX11Widget*)(owner->GetRoot()->nativeWidget));

	if (root->mouse_over == this)
	{
		root->mouse_over = nullptr;
	}

	if (root->over_widget == this)
	{
		root->over_widget = nullptr;
	}

	if (root->focused_widget == this)
	{
		root->focused_widget = nullptr;
	}
}

void WinDX11Widget::SetTimer(int tick)
{
	cur_timerID = timerID;
	::SetTimer(((WinDX11Window*)owner->GetRoot()->nativeWidget)->handle, cur_timerID, 10, (TIMERPROC)WinDX11WidgetTimerProc);
	timerID++;

	win_dx11_timers.push_back(this);
}

void* WinDX11Widget::GetNative()
{
	return &((WinDX11Window*)owner->GetRoot()->nativeWidget)->handle;
}

void WinDX11Widget::Show(bool set)
{
	if (set)
	{
		CalcGlopalPos();
	}
}

void WinDX11Widget::Enable(bool set)
{
}

void WinDX11Widget::SetPos(int set_x, int set_y)
{
}

void WinDX11Widget::SetSize(int w, int h)
{
}

void WinDX11Widget::SetText(const char* txt)
{
}

void WinDX11Widget::Release()
{
	delete this;
}

void WinDX11Widget::Draw()
{
	for (auto widget : owner->childs)
	{
		if (widget->IsVisible())
		{
			WinDX11Widget* nativeWidget = (WinDX11Widget*)widget->nativeWidget;

			if (nativeWidget->global_y + nativeWidget->owner->y + nativeWidget->owner->height < global_y + owner->y)
			{
				continue;
			}

			if (global_y + owner->y + owner->height < nativeWidget->global_y + nativeWidget->owner->y)
			{
				continue;
			}

			nativeWidget->Draw();
		}
	}
}

void WinDX11Widget::Redraw()
{
}

void WinDX11Widget::CaptureMouse()
{
	SetCapture(((WinDX11Window*)owner->GetRoot()->nativeWidget)->handle);
}

void WinDX11Widget::ReleaseMouse()
{
	ReleaseCapture();
}

void WinDX11Widget::Resize()
{
	if (owner->listener && (owner->listenerFlag & EUIWidget::OnResize))
	{
		owner->listener->OnResize(owner);
	}

	for (int i = 0; i<(int)owner->childs.size(); i++)
	{
		owner->childs[i]->nativeWidget->Resize();
	}
}

void WinDX11Widget::NotifyMouseOver()
{
	is_howered = true;
}

void WinDX11Widget::OnMouseLeave()
{
	is_howered = false;
}

void WinDX11Widget::OnFocusLost()
{
	is_focused = false;
}

void WinDX11Widget::SetFocused()
{
	WinDX11Widget* root = (WinDX11Widget*)owner->GetRoot()->nativeWidget;

	if (root->focused_widget && root->focused_widget != this)
	{
		root->focused_widget->OnFocusLost();
	}
	
	is_focused = true;
	root->focused_widget = this;
}

bool WinDX11Widget::IsFocused()
{
	return is_focused;
}

bool WinDX11Widget::IsHowered()
{
	return is_howered;
}

void WinDX11Widget::GetMousePos(int& x, int& y)
{
	WinDX11Window* root = (WinDX11Window*)owner->GetRoot()->nativeWidget;

	x = root->ms_x - (global_x + owner->x);
	y = root->ms_y - (global_y + owner->y);
}

bool WinDX11Widget::IsTreeView()
{
	return false;
}

void WinDX11Widget::OnTextCopy()
{

}

void WinDX11Widget::OnTextPaste()
{

}

void WinDX11Widget::OnKeyDown(int key)
{
	if (owner->listener)
	{
		owner->listener->OnKey(owner, key);
	}
}

void WinDX11Widget::OnCharDown(int key)
{
	if (owner->listener)
	{
		owner->listener->OnKey(owner, key);
	}
}

void WinDX11Widget::OnTimer()
{

}

bool WinDX11Widget::IsHitted(int ms_x, int ms_y)
{
	if (global_x + owner->x < ms_x && ms_x < global_x + owner->x + owner->width &&
		global_y + owner->y < ms_y && ms_y < global_y + owner->y + owner->height)
	{
		return true;
	}

	return false;
}

void WinDX11Widget::OnMouseMove(int ms_x, int ms_y)
{
	if (owner->listener)
	{
		owner->listener->OnMouseMove(owner, ms_x, ms_y);
	}
}

void WinDX11Widget::OnMouseWheel(int delta)
{
	if (owner->listener)
	{
		owner->listener->OnMouseWheel(owner, delta);
	}
}

void WinDX11Widget::OnLeftMouseDown(int ms_x, int ms_y)
{
	mouse_pressed = true;

	if (owner->listener)
	{
		owner->listener->OnLeftMouseDown(owner, ms_x, ms_y);
	}
}

void WinDX11Widget::OnLeftMouseUp(int ms_x, int ms_y)
{
	mouse_pressed = false;

	if (owner->listener)
	{
		owner->listener->OnLeftMouseUp(owner, ms_x, ms_y);
	}
}

void WinDX11Widget::OnRightMouseUp(int ms_x, int ms_y)
{
	if (owner->listener)
	{
		owner->listener->OnRightMouseUp(owner, ms_x, ms_y);
	}
}

void WinDX11Widget::CalcGlopalPos()
{
	if (!owner->parent || (owner->parent && !owner->parent->parent))
	{
		global_x = 0;
		global_y = 0;
	}
	else
	{
		WinDX11Widget* prnt = (WinDX11Widget*)owner->parent->nativeWidget;
		global_x = prnt->global_x + prnt->owner->x;
		global_y = prnt->global_y + prnt->owner->y;
	}

	for (int i = 0; i<(int)owner->childs.size(); i++)
	{
		((WinDX11Widget*)owner->childs[i]->nativeWidget)->CalcGlopalPos();
	}
}

#endif
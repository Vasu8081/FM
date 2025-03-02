#ifndef TASK_BAR_ICON_HPP
#define TASK_BAR_ICON_HPP

#include <wx/wx.h>
#include <wx/taskbar.h>
#include <ui/home.hpp>

class task_bar_icon : public wxTaskBarIcon
{
public:
    task_bar_icon() {
#ifdef WIN32
        SetIcon(wxICON(IDI_APPICON));
#else
        SetIcon();
#endif
        _home = wxSharedPtr<home>(new home());
        Bind(wxEVT_TASKBAR_RIGHT_UP, &task_bar_icon::on_right_click, this);
        Bind(wxEVT_TASKBAR_LEFT_UP, &task_bar_icon::on_left_click, this);
    }

    wxMenu* tray_menu = nullptr;

    void show() {
        _home->Show();
    }

    void on_left_click(wxTaskBarIconEvent& event) {
        show();
    }

    void on_right_click(wxTaskBarIconEvent& event) {
        tray_menu = new wxMenu;
        tray_menu->Append(wxID_EXIT, "Exit");
        Bind(wxEVT_MENU, &task_bar_icon::on_exit, this, wxID_EXIT);
        PopupMenu(tray_menu);
        delete tray_menu;
        tray_menu = nullptr;
    }

    void on_exit(wxCommandEvent& event) {
        int answer = wxMessageBox("Are you sure you want to exit?", "Confirm Exit", wxYES_NO | wxICON_QUESTION);
        if (answer == wxYES) {
            wxTheApp->ExitMainLoop();  // Proper way to exit wxWidgets app
        }
    }

    
private:
    wxSharedPtr<home> _home;
};


#endif // TASK_BAR_ICON_HPP
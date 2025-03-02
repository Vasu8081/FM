#ifndef HOME_HPP
#define HOME_HPP

#include <wx/wx.h>
#include <models/models_manager.hpp>

class home : public wxFrame
{
public:
    home() : wxFrame(nullptr, wxID_ANY, "Home", wxDefaultPosition, wxSize(800, 600)) {
#ifdef WIN32
        SetIcon(wxICON(IDI_APPICON));
#else
        SetIcon();
#endif
        SetMinSize(wxSize(800, 600));
        SetMaxSize(wxSize(800, 600));
        Centre();
    }
private:
    model_manager& _models = model_manager::getInstance();
};

#endif // HOME_HPP
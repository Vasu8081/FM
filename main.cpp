#include <wx/wx.h>
#include <wx/taskbar.h>
#include <ui/task_bar_icon.hpp>
#include <app_config.hpp>
#include <onedrive/onedrive.hpp>
#include <models/models_manager.hpp>

class FM final : public wxApp
{
public:
    bool OnInit() override {
        if(!wxApp::OnInit()) return false;
        wxInitAllImageHandlers();
        
        onedrive& onedrive_ = onedrive::getInstance();

        model_manager& model_manager_ = model_manager::getInstance();
        model_manager_.loadAccounts();
        model_manager_.loadTransactions();
        
        task_bar_icon* task_bar_icon_ = new task_bar_icon();

        if(!wxTaskBarIcon::IsAvailable()) {
            task_bar_icon_->show();
        }
        
        return true;
    }
};

wxIMPLEMENT_APP(FM);
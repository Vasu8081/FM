// #include <wx/wx.h>
// #include <wx/taskbar.h>


// class FM final : public wxApp
// {
// public:
//     bool OnInit() override {
//         if(!wxApp::OnInit()) return false;
//         wxInitAllImageHandlers();

//         if(!wxTaskBarIcon::IsAvailable()) {
//             wxMessageBox("Taskbar icon is not available on this platform.", "Error", wxICON_ERROR);
//             return false;
//         }
        
//         return true;
//     }
// };

// wxIMPLEMENT_APP(FM);

#include <wx/wx.h>
#include <onedrive/onedrive.hpp>

int main() {
    wxInitializer initializer;
    if(!initializer) {
        wxLogError("Failed to initialize wxWidgets.");
        return -1;
    }

    onedrive od;
    od.sync_down();
    return 0;
}
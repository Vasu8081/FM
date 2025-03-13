#ifndef ADD_MODEL_HPP
#define ADD_MODEL_HPP

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <models/model.hpp>
#include <models/model_factory.hpp>
#include <json.hpp>
#include <memory>
#include <map>

class AddModelForm : public wxPanel
{
private:
    std::string _model_type;
    std::shared_ptr<Model> _model;
    wxButton *addButton;
    wxButton *cancelButton;
    std::map<int, std::string> accountMapping;
    std::map<int, std::string> categoryMapping;
    std::unordered_map<std::string, wxControl *> inputFields;
    std::unordered_map<std::string, std::string> fieldTypes;

    void OnAdd(wxCommandEvent &event);
    void OnCancel(wxCommandEvent &event);
    bool ValidateInputs();

public:
    AddModelForm(wxWindow *parent, const std::string &type, std::shared_ptr<Model> default_model = nullptr, bool from_account = false);
};

#endif // ADD_MODEL_HPP
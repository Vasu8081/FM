#ifndef EDIT_MODEL_HPP
#define EDIT_MODEL_HPP

#include <wx/wx.h>
#include <wx/datectrl.h>
#include <models/model.hpp>
#include <models/model_factory.hpp>
#include <json.hpp>
#include <memory>
#include <map>

class EditModelForm : public wxPanel
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

    void OnEdit(wxCommandEvent &event);
    void OnCancel(wxCommandEvent &event);
    bool ValidateInputs();

public:
    EditModelForm(wxWindow *parent, const std::string &type, std::shared_ptr<Model> model);
};

#endif // EDIT_MODEL_HPP
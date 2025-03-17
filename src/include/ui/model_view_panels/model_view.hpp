#ifndef MODEL_VIEW_HPP
#define MODEL_VIEW_HPP

#include <wx/wx.h>
#include <models/model.hpp>
#include <icons.hpp>

class ModelView : public wxPanel
{
public:
    ModelView(wxWindow *parent, std::shared_ptr<Model> model) : wxPanel(parent), _model(model)
    {
        model->registerObserver([this]()
                                { update(); });
    }

    virtual void update()
    {
        wxMessageBox("Not implemented");
    }

    wxBoxSizer *createStaticText(wxWindow *parent, const wxString &key, const wxString &value)
    {
        auto box_sizer = new wxBoxSizer(wxHORIZONTAL);
        auto staticKey = new wxStaticText(parent, wxID_ANY, key + " : ");
        staticKey->SetForegroundColour(_foregroundColour);
        wxFont font = staticKey->GetFont();
        font.SetWeight(wxFONTWEIGHT_BOLD);
        staticKey->SetFont(font);

        auto staticValue = new wxStaticText(parent, wxID_ANY, value);
        staticValue->SetForegroundColour(_foregroundColour);

        _staticTextFields[key.ToStdString()] = staticValue;
        box_sizer->Add(staticKey, 0, wxALL, 2);
        box_sizer->Add(staticValue, 0, wxEXPAND | wxALL, 2);

        return box_sizer;
    }

protected:
    std::shared_ptr<Model> _model;
    Icons &_icon = Icons::getInstance();
    wxColour _foregroundColour;
    wxColour _backgroundColour;
    std::unordered_map<std::string, wxStaticText *> _staticTextFields;
};

#endif // MODEL_VIEW_HPP
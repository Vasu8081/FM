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

protected:
    std::shared_ptr<Model> _model;
    Icons &_icon = Icons::getInstance();
};

#endif // MODEL_VIEW_HPP
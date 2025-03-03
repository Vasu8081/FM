#ifndef MODEL_VIEW_HPP
#define MODEL_VIEW_HPP

#include <wx/wx.h>
#include <models/model.hpp>

class ModelView : public wxPanel
{
public:
    ModelView(wxWindow *parent, std::shared_ptr<model> model) : wxPanel(parent), _model(model)
    {
        model->registerObserver([this]() { update(); });
    }

    virtual void update() = 0;

protected:
    std::shared_ptr<model> _model;
};

#endif // MODEL_VIEW_HPP
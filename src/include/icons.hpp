#ifndef ICONS_HPP
#define ICONS_HPP

#include <wx/artprov.h>
#include <wx/bitmap.h>
#include <wx/colour.h>
#include <wx/wx.h>
#include <icons/wxMaterialDesignArtProvider.hpp>

class Icons {
private:
    wxString _client = wxART_CLIENT_MATERIAL_FILLED;
    wxSize _size = wxSize(16, 16);
    wxColor _color = *wxBLACK;

    Icons() {
        wxArtProvider::Push(new wxMaterialDesignArtProvider);
    }

    ~Icons() {}
    

public:
    Icons(const Icons&) = delete;
    Icons& operator=(const Icons&) = delete;

    static Icons& getInstance() {
        static Icons instance;
        return instance;
    }

    wxBitmap get(const wxString& id) {
        return wxMaterialDesignArtProvider::GetBitmap(id, _client, _size, _color);
    }

};

#endif // ICONS_HPP
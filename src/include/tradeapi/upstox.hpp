#ifndef UPSTOX_H
#define UPSTOX_H

#include <string>
#include <curl/curl.h>
#include <wx/wx.h>
#include <app_config.hpp>

using nlohmann::json;

class Upstox {
public:
    Upstox(const Upstox&) = delete;
    Upstox& operator=(const Upstox&) = delete;

    static Upstox& getInstance() {
        static Upstox instance;
        return instance;
    }

    json getProduct(const std::string& symbol) {
        std::string url = "https://api-v2.upstox.com/market-quote/quotes?symbol=" + symbol;
        std::string response;
        CURL* curl = curl_easy_init();
        
        if (curl) {
            struct curl_slist* headers = nullptr;
            headers = curl_slist_append(headers, "Api-Version: 2.0");
            headers = curl_slist_append(headers, "Accept: application/json");
            headers = curl_slist_append(headers, ("Authorization: Bearer " + _access_token).c_str());

            curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
            curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
            CURLcode res = curl_easy_perform(curl);
            
            if (res != CURLE_OK) {
                wxMessageBox("CURL request failed: %s"+std::string(curl_easy_strerror(res)));
                wxLogError("CURL request failed: %s", curl_easy_strerror(res));
            }

            curl_slist_free_all(headers);
            curl_easy_cleanup(curl);
        }

        try {
            return json::parse(response);
        } catch (const json::exception& e) {
            wxMessageBox("JSON parsing error: %s"+std::string(e.what()));
            return {};
        }
    }

private:
    app_config& _config = app_config::getInstance();

    Upstox() {
        _access_token = _config.getUpstoxAccessToken();
        if (_access_token.empty()) {
            wxDialog updialog(NULL, wxID_ANY, "Enter upstox access token ", wxDefaultPosition, wxSize(300, 150));
            wxTextCtrl* uptextCtrl = new wxTextCtrl(&updialog, wxID_ANY, _access_token, wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER);
            wxButton* upokButton = new wxButton(&updialog, wxID_OK, "OK", wxDefaultPosition, wxDefaultSize);
            wxButton* upcancelButton = new wxButton(&updialog, wxID_CANCEL, "Cancel", wxDefaultPosition, wxDefaultSize);
            wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
            sizer->Add(uptextCtrl, 0, wxALL | wxEXPAND, 5);
            sizer->Add(upokButton, 0, wxALL | wxALIGN_CENTER, 5);
            sizer->Add(upcancelButton, 0, wxALL | wxALIGN_CENTER, 5);
            updialog.SetSizer(sizer);
            updialog.Fit();
            updialog.ShowModal();
            _access_token = uptextCtrl->GetValue().ToStdString();
            _config.setUpstoxAccessToken(_access_token);
            _config.save();
        }
    }

    std::string _access_token;

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
        size_t totalSize = size * nmemb;
        output->append((char*)contents, totalSize);
        return totalSize;
    }
};

#endif

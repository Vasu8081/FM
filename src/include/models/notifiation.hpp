#ifndef NOTIFICATION_HPP
#define NOTIFICATION_HPP

#include <string>
#include <wx/datetime.h>

class Notification {
public:
    Notification() = default;
    Notification(const std::string &message, const wxDateTime &date) : _message(message), _date(date) {}

    std::string getMessage() const {
        return _message;
    }

    wxDateTime getDate() const {
        return _date;
    }

    void setMessage(const std::string &message) {
        _message = message;
    }

    void setDate(const wxDateTime &date) {
        _date = date;
    }

private:
    std::string _message;
    wxDateTime _date;
};

#endif // NOTIFICATION_HPP
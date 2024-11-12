#ifndef ICHATVIEW_H
#define ICHATVIEW_H

#include <QString>

class IChatView {
public:
    virtual ~IChatView() = default;
    virtual void displayMessage(const QString &message) = 0;
    virtual void setPresenter(class ChatPresenter *presenter) = 0;
};

#endif // ICHATVIEW_H

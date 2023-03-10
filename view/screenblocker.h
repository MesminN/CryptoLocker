#ifndef SCREEN_BLOCKER_H
#define SCREEN_BLOCKER_H

#include <QApplication>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QHBoxLayout>
#include <QString>

class ScreenBlocker : public QWidget
{
    Q_OBJECT
    public:
        ScreenBlocker(QWidget *parent = nullptr);
        QString getKey();
        void clearKey();
    private:
        QLineEdit* textField;
        void handleButtonClick();
    private slots:
        void onButtonClicked();
};

#endif // SCREEN_BLOCKER_H

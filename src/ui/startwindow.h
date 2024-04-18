#ifndef STARTSCREEN_H
#define STARTSCREEN_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>

class StartScreen : public QWidget
{
Q_OBJECT
public:
    explicit StartScreen(QWidget *parent = nullptr);

signals:
    void createWorldClicked();
    void loadWorldHistoryClicked();
    void quitClicked();

public slots:
    void show();

private:
    QLabel *gameHeader;
    QPushButton *createWorldButton;
    QPushButton *loadWorldHistoryButton;
    QPushButton *quitButton;
};

#endif // STARTSCREEN_H

#include <QLabel>
#include "startwindow.h"

StartScreen::StartScreen(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    gameHeader = new QLabel("Random Adventure", this);
    createWorldButton = new QPushButton("Create World", this);
    loadWorldHistoryButton = new QPushButton("Load World History", this);
    quitButton = new QPushButton("Quit", this);

    gameHeader->setAlignment(Qt::AlignCenter);
    auto font = gameHeader->font();
    font.setPointSize(48);
    font.setBold(true);
    font.setCapitalization(QFont::Capitalize);
    gameHeader->setFont(font);

    layout->addWidget(gameHeader);
    layout->addWidget(createWorldButton);
    layout->addWidget(loadWorldHistoryButton);
    layout->addWidget(quitButton);

    connect(createWorldButton, &QPushButton::clicked, this, &StartScreen::createWorldClicked);
    connect(loadWorldHistoryButton, &QPushButton::clicked, this, &StartScreen::loadWorldHistoryClicked);
    connect(quitButton, &QPushButton::clicked, this, &StartScreen::quitClicked);
}

void StartScreen::show()
{
    showMaximized();
}

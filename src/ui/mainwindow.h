#ifndef MAINWORLDVIEW_H
#define MAINWORLDVIEW_H

#include <QMainWindow>
#include <QLabel>
#include <QTextEdit>
#include <QTableView>
#include <QSplitter>
#include <QStandardItemModel>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPushButton>
#include <QVBoxLayout>
#include "../world/WorldTile.h"
#include "../../include/PerlinNoise.hpp"

class MainWorldView : public QMainWindow
{
Q_OBJECT
public:
    explicit MainWorldView(int world_width, int world_height, const siv::PerlinNoise &perlin, QWidget *parent = nullptr);
    ~MainWorldView();

    void updateWorld(int world_width, int world_height, const siv::PerlinNoise &perlin);
    QTextEdit *getTextAddr();
    QLabel *getHeaderAddr();

public slots:
    void show();

private:
    QLabel *headerLabel;
    QTextEdit *textEdit;
    QTableView *tableView;

    void generateWorld(int world_width, int world_height, const siv::PerlinNoise &perlin);
};

#endif // MAINWORLDVIEW_H

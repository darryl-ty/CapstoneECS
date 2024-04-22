#include "mainwindow.h"

MainWorldView::MainWorldView(int world_width, int world_height, const siv::PerlinNoise &perlin, QWidget *parent) : QMainWindow(parent) {
    // Create the central widget
    QWidget *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    // Create a vertical layout for the central widget
    QVBoxLayout *mainLayout = new QVBoxLayout(centralWidget);

    // Create a header label
    headerLabel = new QLabel("Header Text", this);
    headerLabel->setAlignment(Qt::AlignCenter);
    QFont font = headerLabel->font();
    font.setPointSize(24);
    font.setBold(true);
    headerLabel->setFont(font);
    headerLabel->setStyleSheet("background-color: transparent;");
    headerLabel->setFixedHeight(30); // Set the height of the header

    // Create the text edit
    textEdit = new QTextEdit();
    textEdit->setReadOnly(true); // Make the text edit uneditable

    // Create a splitter to divide the space horizontally
    QSplitter *splitter = new QSplitter(Qt::Vertical, centralWidget);

    // Create the table view
    tableView = new QTableView();
    splitter->addWidget(tableView);

    // Add the header label to the main layout
    mainLayout->addWidget(headerLabel);

    // Add the text edit to the splitter
    splitter->addWidget(textEdit);

    // Set the sizes of the splitter
    QList<int> sizes = {2, 1}; // Proportional scaling: 2/3 for table view, 1/3 for text viewer
    splitter->setSizes(sizes);

    // Add the splitter to the main layout
    mainLayout->addWidget(splitter);

    // Generate the world
    generateWorld(world_width, world_height, perlin);
}

MainWorldView::~MainWorldView() {
}

QTextEdit* MainWorldView::getTextAddr(){
    return textEdit;
}
QLabel* MainWorldView::getHeaderAddr(){
    return headerLabel;
}

void MainWorldView::updateWorld(int world_width, int world_height, const siv::PerlinNoise &perlin){
    generateWorld(world_width, world_height, perlin);
}

void MainWorldView::generateWorld(int world_width, int world_height, const siv::PerlinNoise &perlin) {
    // Create a new model for the table view
    QStandardItemModel *model = new QStandardItemModel(world_width, world_height, this);
    tableView->setModel(model);

    // Set the headers invisible
    tableView->verticalHeader()->setVisible(false);
    tableView->horizontalHeader()->setVisible(false);

    // Generate and populate the world data
    for (int i = 0; i < world_width; ++i) {
        for (int j = 0; j < world_height; ++j) {
            const float noise = perlin.octave2D_01((i * 0.01), (j * 0.01), 4);
            auto *item = new QStandardItem(QString::fromStdString(WorldTile(noise).getTileType()));
            item->setBackground(WorldTile(noise).getTileColor());
            item->setForeground(QBrush(QColor(Qt::black)));
            item->setTextAlignment(Qt::AlignCenter);
            model->setItem(i, j, item);
        }
    }
}

void MainWorldView::show() {
    showMaximized();
}


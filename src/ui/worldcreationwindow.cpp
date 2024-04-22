#include "worldcreationwindow.h"

WorldCreationDialog::WorldCreationDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Create World");

    widthSpinBox = new QSpinBox(this);
    widthSpinBox->setMinimum(20);
    heightSpinBox = new QSpinBox(this);
    heightSpinBox->setMinimum(20);
    seedLineEdit = new QLineEdit(this);
    okButton = new QPushButton("OK", this);
    cancelButton = new QPushButton("Cancel", this);

    QFormLayout *layout = new QFormLayout(this);
    layout->addRow("Width:", widthSpinBox);
    layout->addRow("Height:", heightSpinBox);
    layout->addRow("Seed (optional):", seedLineEdit);
    layout->addWidget(okButton);
    layout->addWidget(cancelButton);

    connect(okButton, &QPushButton::clicked, this, &WorldCreationDialog::accept);
    connect(cancelButton, &QPushButton::clicked, this, &WorldCreationDialog::reject);
}
void WorldCreationDialog::accept()
{
    emit worldCreated(widthSpinBox->value(), heightSpinBox->value(), seedLineEdit->text());
    emit worldCreationStarted(); // Signal that world creation has started
    QDialog::accept();
}

int WorldCreationDialog::getWorldWidth() const{
    return widthSpinBox->value();
}
int WorldCreationDialog::getWorldHeight() const{
    return heightSpinBox->value();
}
int WorldCreationDialog::getWorldSeed() const{
    return seedLineEdit->text().toInt();
}

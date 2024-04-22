#ifndef WORLDCREATIONDIALOG_H
#define WORLDCREATIONDIALOG_H

#include <QDialog>
#include <QSpinBox>
#include <QLineEdit>
#include <QPushButton>
#include <QFormLayout>

class WorldCreationDialog : public QDialog
{
Q_OBJECT
public:
    explicit WorldCreationDialog(QWidget *parent = nullptr);

    int getWorldWidth() const;
    int getWorldHeight() const;
    int getWorldSeed() const;

signals:
    void worldCreated(int width, int height, QString seed);
    void worldCreationStarted();

private slots:
    void accept() override;

private:
    QSpinBox *widthSpinBox;
    QSpinBox *heightSpinBox;
    QLineEdit *seedLineEdit;
    QPushButton *okButton;
    QPushButton *cancelButton;
};

#endif // WORLDCREATIONDIALOG_H

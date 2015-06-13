#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveAsFile();
    bool exitEditor();

    void textChanged();

    void toCOE();
    void toMIPS();

private:
    Ui::MainWindow *ui;
    QString fileName;
    bool edited;

    void checkEdited();
};

#endif // MAINWINDOW_H

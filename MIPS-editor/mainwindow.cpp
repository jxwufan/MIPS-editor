#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QKeySequence>
#include <QMessageBox>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->actionNew, &QAction::triggered, this, &MainWindow::newFile);
    connect(ui->actionOpen, &QAction::triggered, this, &MainWindow::openFile);
    connect(ui->actionSave, &QAction::triggered, this, &MainWindow::saveFile);
    connect(ui->actionSave_as, &QAction::triggered, this, &MainWindow::saveAsFile);
    connect(ui->actionExit, &QAction::triggered, this, &MainWindow::exitEditor);

    ui->actionNew->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_N));
    ui->actionOpen->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_O));
    ui->actionSave->setShortcut(QKeySequence(Qt::CTRL + Qt::Key_S));
    ui->actionSave_as->setShortcut(QKeySequence(Qt::CTRL + Qt::SHIFT + Qt::Key_S));

    edited = false;
    fileName = "*";

    connect(ui->plainTextEdit, &QPlainTextEdit::textChanged, this, &MainWindow::textChanged);

    ui->statusBar->showMessage(fileName);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::newFile()
{
    checkEdited();
    ui->plainTextEdit->clear();
    fileName = "*";

    ui->statusBar->showMessage(fileName);
}

void MainWindow::openFile()
{
    checkEdited();
    fileName = QFileDialog::getOpenFileName();
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "File error", tr("Cannot read file %1:\n%2.").arg(file.fileName()).arg(file.errorString()));
        return;
    }
    QTextStream text(&file);
    ui->plainTextEdit->setPlainText(text.readAll());
    ui->statusBar->showMessage(fileName);
}

void MainWindow::saveFile()
{
    if (!edited) {
        QMessageBox::information(this, "Nothing to save", "Nothing has been changed!", QMessageBox::Ok);
        return;
    }
    if (fileName == "*")
        saveAsFile();
    else {
        QFile file(fileName);
        if (!file.open(QFile::WriteOnly | QFile::Text)) {
            QMessageBox::warning(this, "File error", tr("Cannot write file %1:\n%2.").arg(file.fileName()).arg(file.errorString()));
            return;
        }
        QTextStream text(&file);
        text << ui->plainTextEdit->toPlainText();
        edited = false;
        ui->statusBar->showMessage(fileName);
    }
}

void MainWindow::saveAsFile()
{
    fileName = QFileDialog::getSaveFileName();
    edited = true;
    saveFile();
}

void MainWindow::exitEditor()
{
    if (edited) {
        if (QMessageBox::warning(this, "Warning", "You have something didn't saved, do you want save them first?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            saveFile();
        else {
            if (QMessageBox::warning(this, "Warning", "Do you really want to exit the program?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                this->close();
        }
    } else {
        if (QMessageBox::warning(this, "Warning", "Do you really want to exit the program?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            this->close();
    }
}

void MainWindow::textChanged()
{
    edited = true;
}

void MainWindow::checkEdited()
{
    if (edited && (QMessageBox::warning(this, "Warning", "Do you want to save the edited file first?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes))
        saveFile();
}

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "assembler.h"
#include "binarystringtranslator.h"
#include "coetranslator.h"
#include "disassembler.h"

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
    connect(ui->toCOEButton, &QPushButton::clicked, this, &MainWindow::toCOE);
    connect(ui->toMIPSButton, &QPushButton::clicked, this, &MainWindow::toMIPS);

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

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (this->exitEditor()) event->accept();
        else event->ignore();
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
    QString newFileName = QFileDialog::getOpenFileName(this, "Open File", ".", "*.coe;;*.mips;;*.bin");
    if (newFileName != "") {
        QFile file(newFileName);
        if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this, "File error", tr("Cannot read file %1:\n%2.").arg(file.fileName()).arg(file.errorString()));
        return;
        }
        if (newFileName.endsWith("mips") || newFileName.endsWith("coe")) {
            QTextStream text(&file);
            ui->plainTextEdit->setPlainText(text.readAll());
            fileName = newFileName;
            ui->statusBar->showMessage(fileName);
        } else {
            BinaryStringTranslator translator;
            QString binstr = translator.toBinstr(file.readAll());
            if (QMessageBox::warning(this, "Translate File", "Do you want to translate it as mips file to edit? Click no to translate it as coe file.", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes) {
                Disassembler dis;
                ui->plainTextEdit->setPlainText(dis.bin2ass(binstr));
                fileName = "*";
                ui->statusBar->showMessage(fileName);
            } else {
                COETranslator trans;
                ui->plainTextEdit->setPlainText(trans.bin2coe(binstr));
                fileName = "*";
                ui->statusBar->showMessage(fileName);
            }
        }
    }
}

void MainWindow::saveFile()
{
    if (!edited) {
        QMessageBox::information(this, "Nothing to save", "Nothing has been changed!", QMessageBox::Ok);
        return;
    }
    if (fileName == "*")
        saveAsFile();
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

void MainWindow::saveAsFile()
{
    QString newFileName;
    newFileName = QFileDialog::getSaveFileName(this, "Save file", ".", "*.bin;;*.coe;;*.mips");
    COETranslator coetranslator;
    BinaryStringTranslator bintranslator;
    Assembler assembler;
    Disassembler disassembler;
    if (newFileName != "") {
        QFile file(newFileName);
        if (!file.open(QFile::WriteOnly)) {
            QMessageBox::warning(this, "File error", tr("Cannot write file %1:\n%2.").arg(file.fileName()).arg(file.errorString()));
            return;
        } else if (fileName == "*") {
            if (newFileName.endsWith("bin")) {
                QString bin;
                if (QMessageBox::warning(this, "Which type?", "Is the raw text is MIPS assembly language? Click no if it is coe file.", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes){
                    bin = assembler.ass2bin(ui->plainTextEdit->toPlainText());
                } else {
                    bin = bintranslator.toBin(coetranslator.coe2bin(ui->plainTextEdit->toPlainText()));
                }
                QDataStream data(&file);
                data.writeRawData(bin.toLatin1().data(), bin.toLatin1().size());
            } else {
                QTextStream text(&file);
                text << ui->plainTextEdit->toPlainText();
                fileName = newFileName;
                ui->statusBar->showMessage(fileName);
                edited = false;
            }
        } else {
            if (newFileName == fileName) {
                saveFile();
            } else {
                if (newFileName.endsWith("bin")) {
                    QString bin;
                    if ( fileName.endsWith("mips") ) {
                        bin = assembler.ass2bin(ui->plainTextEdit->toPlainText());
                    } else {
                        bin = bintranslator.toBin(coetranslator.coe2bin(ui->plainTextEdit->toPlainText()));
                    }
                    QDataStream data(&file);
                    data.writeRawData(bin.toLatin1().data(), bin.toLatin1().size());
                } else if (fileName.endsWith("mips") && newFileName.endsWith("coe")) {
                    QTextStream text(&file);
                    text << coetranslator.bin2coe(bintranslator.toBinstr(assembler.ass2bin(ui->plainTextEdit->toPlainText()).toLatin1()));
                } else if (fileName.endsWith("coe") && newFileName.endsWith("mips")) {
                    QTextStream text(&file);
                    text << disassembler.bin2ass(coetranslator.coe2bin(ui->plainTextEdit->toPlainText()));
                    qDebug() << disassembler.bin2ass(coetranslator.coe2bin(ui->plainTextEdit->toPlainText()));
                } else {
                    QTextStream text(&file);
                    text << ui->plainTextEdit->toPlainText();
                }
            }
        }
    }
}

bool MainWindow::exitEditor()
{
    if (edited) {
        if (QMessageBox::warning(this, "Warning", "You have something didn't saved, do you want save them first?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            saveFile();
        else {
            if (QMessageBox::warning(this, "Warning", "Do you really want to exit the program?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
                return true;
        }
    } else {
        if (QMessageBox::warning(this, "Warning", "Do you really want to exit the program?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
            return true;
    }
    return false;
}

void MainWindow::textChanged()
{
    edited = true;
    ui->statusBar->showMessage(fileName, 0);
}

void MainWindow::checkEdited()
{
    if (edited && (QMessageBox::warning(this, "Warning", "Do you want to save the edited file first?", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes))
        saveFile();
}

void MainWindow::toCOE()
{
    Assembler assembler;
    Disassembler disassembler;
    COETranslator coetranslator;
    BinaryStringTranslator bintranslator;
    ui->outputEdit->setPlainText(coetranslator.bin2coe(bintranslator.toBinstr(assembler.ass2bin(ui->plainTextEdit->toPlainText()).toLatin1())));
}

void MainWindow::toMIPS()
{
    Assembler assembler;
    Disassembler disassembler;
    COETranslator coetranslator;
    BinaryStringTranslator bintranslator;
    ui->outputEdit->setPlainText(disassembler.bin2ass(coetranslator.coe2bin(ui->plainTextEdit->toPlainText())));
}


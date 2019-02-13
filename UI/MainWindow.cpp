#include <cmath>
#include <iostream>
#include <QtWidgets/QFileDialog>
#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow), app(std::make_unique<App>()) {
    ui->setupUi(this);
    this->setupUiExtras();

    connect(app->finder.get(), SIGNAL(wordAdded(QString)), this, SLOT(addItemToList(QString)));
    connect(app->finder.get(), SIGNAL(progressChanged(int)), this, SLOT(setProgress(int)));
    connect(app->finder.get(), SIGNAL(findEnded()), this, SLOT(findEnded()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupUiExtras() {
    for(auto i = 0; i < 16; ++i) {
        addCharPlace();
    }
    for(auto i = 0; i < char_array.size(); ++i) {
        connect(char_array[i], &QLineEdit::textEdited, this, [=](const QString& value) {
            if(value.isEmpty()) {
                setColorBox(i, 64, 0, 0);
                if (i != 0) {
                    char_array[i - 1]->setFocus();
                    char_array[i - 1]->selectAll();
                }
            } else {
                clearColorBox(i);
                char_array[i]->setText(char_array[i]->text().toUpper());
                if (i + 1 == char_array.size())
                    ui->searchButton->setFocus();
                else {
                    char_array[i + 1]->setFocus();
                    char_array[i + 1]->selectAll();
                }
            }
        });
        connect(char_array[i], &QLineEdit::returnPressed, this, [=]() {
            on_actionSearch_triggered();
        });
    }
}

void MainWindow::on_searchButton_clicked() {
    on_actionSearch_triggered();
}

void MainWindow::on_clearButton_clicked() {
    on_actionClear_triggered();
}

void MainWindow::on_actionSearch_triggered() {
    std::vector<QChar> chars;
    clearAllColorBox();
    for(auto i = 0; i < char_array.size(); ++i) {
        if(char_array[i]->text().isEmpty()) {
            setColorBox(i, 64, 0, 0);
        } else {
            chars.push_back(char_array[i]->text().toUpper()[0]);
        }
    }
    if(char_array.size() != chars.size()) {
        setStatus("Wypełnij wszystkie pola");
        return;
    }

    ui->searchButton->setEnabled(false);
    ui->clearButton->setEnabled(false);
    ui->menuAction->setEnabled(false);
    for(auto i : char_array) {
        i->setEnabled(false);
    }
    ui->wordList->clear();
    setStatus("%p%");
    app->finder->startFind(chars);
}

void MainWindow::on_actionClear_triggered() {
    ui->wordList->clear();
    setStatus("%p%");
    setProgress(0);
    for(auto i : char_array) {
        i->clear();
    }
    clearAllColorBox();
    char_array.front()->setFocus();
}

void MainWindow::on_actionSaveResults_triggered() {
    QString filename =
            QFileDialog::getSaveFileName(this,
                    tr("Zapisywanie wyników"), "./",
                    tr("(*.*)"));
    if(filename.isEmpty()) return;
    try {
        File f(filename);
        std::vector<QString> result;
        for(auto i = 0; i < ui->wordList->count(); ++i) {
            result.push_back(ui->wordList->item(i)->text());
        }
        f.writeList(result);
        setStatus("Zapisano");
    } catch(std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
}

void MainWindow::on_actionOpenDictionary_triggered() {
    QString filename =
            QFileDialog::getOpenFileName(this,
                    tr("Otwieranie pliku słownika"), "./",
                    tr("(*.*)"));
    if(filename.isEmpty()) return;
    try {
        app->finder->openDictionaryFile(filename);
        setStatus("Słownik wczytany");
    } catch(std::exception & e) {
        std::cerr << e.what() << std::endl;
    }
}

void MainWindow::on_actionHowTo_triggered() {
    setStatus("W budowie");
}

void MainWindow::on_actionAbout_triggered() {
    About window;
    window.exec();
}

void MainWindow::addItemToList(QString object) {
    if(ui->wordList->count() == 0)
        ui->wordList->addItem(object);
    for(auto i = 0; i < ui->wordList->count(); ++i) {
        if(ui->wordList->item(i)->text().length() < object.length()) {
            ui->wordList->insertItem(i, object);
            break;
        }
    }
}

void MainWindow::setStatus(QString status) {
    ui->progressBar->setFormat(status);
}

void MainWindow::setProgress(int value) {
    ui->progressBar->setValue(value);
}

void MainWindow::findEnded() {
    setStatus("Zakończono!");
    ui->searchButton->setEnabled(true);
    ui->clearButton->setEnabled(true);
    ui->menuAction->setEnabled(true);
    for(auto i : char_array) {
        i->setEnabled(true);
    }
}

void MainWindow::setColorBox(int box, int red, int green, int blue) {
    std::string s = "background-color: rgb(" + std::to_string(red) + ", " + std::to_string(green) + ", " + std::to_string(blue) + ");";
    char_array[box]->setStyleSheet(s.c_str());
}

void MainWindow::clearColorBox(int box) {
    char_array[box]->setStyleSheet("");
}

void MainWindow::clearAllColorBox() {
    for(auto i : char_array) {
        i->setStyleSheet("");
    }
}

void MainWindow::addCharPlace() {
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QFont font;
    font.setPointSize(32);
    QLineEdit* charPlace = new QLineEdit(ui->gridLayoutWidget);
    charPlace->setObjectName(QString::fromStdString("charPlace" + char_array.size()));
    sizePolicy.setHeightForWidth(charPlace->sizePolicy().hasHeightForWidth());
    charPlace->setSizePolicy(sizePolicy);
    charPlace->setMinimumSize(QSize(64, 64));
    charPlace->setMaximumSize(QSize(128, 128));
    charPlace->setFont(font);
    charPlace->setCursor(QCursor(Qt::ArrowCursor));
    charPlace->setMouseTracking(false);
    charPlace->setInputMask(QStringLiteral(""));
    charPlace->setMaxLength(1);
    charPlace->setFrame(true);
    charPlace->setAlignment(Qt::AlignCenter);
    charPlace->setPlaceholderText(QStringLiteral("*"));
    char_array.push_back(charPlace);
    ui->charGrid->addWidget(charPlace, floor((char_array.size() - 1) / 4),
                            floor((char_array.size() - 1) % 4), 1, 1);
}
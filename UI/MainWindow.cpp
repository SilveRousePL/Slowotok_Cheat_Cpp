#include <cmath>
#include <iostream>
#include "MainWindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    for(auto i = 0; i < 16; ++i) {
        addCharPlace();
    }
    for(auto i = 0; i < char_array.size(); ++i) {
        connect(char_array[i], &QLineEdit::textChanged, this, [=]() {
            if (i+1 == char_array.size())
                ui->searchButton->setFocus();
            else {
                char_array[i + 1]->setFocus();
                char_array[i + 1]->selectAll();
            }
        });
    }

    app = std::make_unique<App>();
    connect(this->app->finder.get(), SIGNAL(wordAdded(QString)), this, SLOT(addItemToList(QString)));
    connect(this->app->finder.get(), SIGNAL(progressChanged(int)), this, SLOT(setProgress(int)));
    connect(this->app->finder.get(), SIGNAL(findEnded()), this, SLOT(findEnded()));
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_searchButton_clicked() {
    this->on_actionSearch_triggered();
}

void MainWindow::on_clearButton_clicked() {
    this->on_actionClear_triggered();
}

void MainWindow::on_actionSearch_triggered() {
    std::vector<QChar> chars;
    for(auto i : char_array) {
        if(i->text().isEmpty()) {
            setStatus("Wypełnij wszystkie pola");
            return;
        }
        chars.push_back(i->text().toUpper()[0]);
    }

    this->ui->searchButton->setEnabled(false);
    this->ui->clearButton->setEnabled(false);
    for(auto i : char_array) {
        i->setEnabled(false);
    }
    this->ui->wordList->clear();
    this->setStatus("%p%");
    this->app->finder->startFind(chars);
}

void MainWindow::on_actionClear_triggered() {
    this->ui->wordList->clear();
    this->setStatus("%p%");
    this->setProgress(0);
    for(auto i : char_array) {
        i->clear();
    }
    char_array.front()->setFocus();
}

void MainWindow::on_actionSaveResults_triggered() {
    std::cout << "Save" << std::endl;
}

void MainWindow::on_actionOpenDictionary_triggered() {
    std::cout << "Open" << std::endl;
}

void MainWindow::on_actionHowTo_triggered() {
    std::cout << "How To?" << std::endl;
}

void MainWindow::on_actionAbout_triggered() {
    About window;
    window.exec();
}

void MainWindow::addItemToList(QString object) {
    this->ui->wordList->addItem(object);
}

void MainWindow::setStatus(QString status) {
    ui->progressBar->setFormat(status);
}

void MainWindow::setProgress(int value) {
    ui->progressBar->setValue(value);
}

void MainWindow::findEnded() {
    this->setStatus("Zakończono!");
    this->ui->searchButton->setEnabled(true);
    this->ui->clearButton->setEnabled(true);
    for(auto i : char_array) {
        i->setEnabled(true);
    }
}

void MainWindow::addCharPlace() {
    QSizePolicy sizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    QFont font;
    font.setPointSize(32);
    QLineEdit* charPlace = new QLineEdit(ui->gridLayoutWidget);
    charPlace->setObjectName(QString::fromStdString("charPlace" + this->char_array.size()));
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
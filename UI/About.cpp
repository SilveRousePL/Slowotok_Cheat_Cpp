//
// Created by darek on 13.02.19.
//

#include "About.h"
#include "ui_about.h"

About::About(QWidget *parent) : QDialog(parent), ui(new Ui::About) {
    ui->setupUi(this);
}

About::~About() {
    delete ui;
}

void About::on_okButton_clicked() {
    this->close();
}

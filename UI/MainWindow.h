#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLineEdit>
#include <vector>
#include <memory>
#include "../App.h"
#include "About.h"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    std::unique_ptr<App> app;
    std::vector<QLineEdit*> char_array;

    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_clearButton_clicked();
    void on_searchButton_clicked();

    // Menu
    void on_actionSearch_triggered();
    void on_actionClear_triggered();
    void on_actionSaveResults_triggered();
    void on_actionOpenDictionary_triggered();
    void on_actionHowTo_triggered();
    void on_actionAbout_triggered();

    void addItemToList(QString object);
    void setStatus(QString status);
    void setProgress(int value);
    void findEnded();

private:
    Ui::MainWindow *ui;
    void setupUiExtras();
    void setColorBox(int box, int red, int green, int blue);
    void clearColorBox(int box);
    void clearAllColorBox();
    void addCharPlace();
};

#endif // MAINWINDOW_H

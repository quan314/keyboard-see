#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <Windows.h>
#include <QStringListModel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    LRESULT CALLBACK keyProc(int nCode,WPARAM wParam,LPARAM lParam );

    ~MainWindow();
public slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

private slots:
    void on_pushButton_3_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_5_clicked();

private:
    Ui::MainWindow *ui;
    HWND task;

};

#endif // MAINWINDOW_H

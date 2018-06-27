#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_keyhookvs.h"
#include <QMainWindow>
#include <Windows.h>
#include <QStringListModel>

namespace Ui {
class keyhookvs;
}
class keyhookvs : public QMainWindow
{
	Q_OBJECT

public:
	keyhookvs(QWidget *parent = Q_NULLPTR);
	LRESULT CALLBACK keyProc(int nCode, WPARAM wParam, LPARAM lParam);

public slots:
	void on_pushButton_clicked();

	void on_pushButton_2_clicked();

	private slots:
	void on_pushButton_3_clicked();

	void on_pushButton_4_clicked();

	void on_pushButton_5_clicked();

private:
	Ui::keyhookvsClass ui;
	HWND task;
};

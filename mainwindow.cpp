#include "mainwindow.h"
#include "ui_mainwindow.h"
//#include "./widgets/displayWidget/displayWidget.h"
#include "./widgets/centerWidget/centerWidget.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{
    CenterWidget* tv_centralWidget_p = new CenterWidget(this);
	//DisplayWidget* tv_centralWidget_p = new DisplayWidget(this);

    setCentralWidget(tv_centralWidget_p);
    //ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


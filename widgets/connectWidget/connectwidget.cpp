#include "connectwidget.h"
#include "QScreen"
#include <QPixmap>
#include <QGuiApplication>
#include <QPainter>

ConnectWidget::ConnectWidget(QWidget *parent, QWidget *display_p) : QWidget(parent)
{
    setFixedHeight(576);
    setFixedWidth(768);
    tcv_displayWidget_p = display_p;
    tcv_imageThread_p   = new ImageThread();
    connect(tcv_imageThread_p, SIGNAL(imageReady()), this, SLOT(imageUpdated()));
    tcv_imageThread_p->start();
}

void ConnectWidget::paintEvent(QPaintEvent *event) {
    if(tcv_displayWidget_p != NULL) {
        QScreen *screen = QGuiApplication::primaryScreen();
        QPixmap map = screen->grabWindow(tcv_displayWidget_p->winId());

        QPainter tv_painter(this);
        tv_painter.drawPixmap(0, 0, map);
        tv_painter.setPen(QColor(255,255,255));
        tv_painter.drawLine(0, 576/2, 768, 576/2);
        tv_painter.drawLine(768/2, 0, 768/2, 576);
    }
}

void ConnectWidget::imageUpdated() {
    update();
}



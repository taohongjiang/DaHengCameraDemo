#ifndef CONNECTWIDGET_H
#define CONNECTWIDGET_H

#include <QWidget>
#include <QThread>
extern volatile unsigned int tgv_state_ui;

class ImageThread:public QThread
{
    Q_OBJECT
    void run() Q_DECL_OVERRIDE {
        while(1) {
            if(tgv_state_ui == 1) {
                tgv_state_ui = 0;
                emit imageReady();
            }
        }
    }
    signals:
        void imageReady();
};

class ConnectWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ConnectWidget(QWidget *parent = 0, QWidget *display_p = 0);
    void paintEvent(QPaintEvent *event);
public:
    QWidget* tcv_displayWidget_p;
    ImageThread* tcv_imageThread_p;
signals:

public slots:
    void imageUpdated();
};



#endif // CONNECTWIDGET_H

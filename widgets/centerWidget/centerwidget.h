#ifndef CENTERWIDGET_H
#define CENTERWIDGET_H

#include <QWidget>
#include <QResizeEvent>
#include "QPushButton"
class CenterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit CenterWidget(QWidget *parent = 0);
	void resizeEvent(QResizeEvent* size);

public:
    QPushButton* tcv_commandButton_p;
	QPushButton* tcv_savePictureButton_p;
	QPushButton* tcv_saveVideoButton_p;
	QPushButton* tcv_zoomInButton_p;
	QPushButton* tcv_zoomOutButton_p;
signals:

public slots:
};

#endif // CENTERWIDGET_H

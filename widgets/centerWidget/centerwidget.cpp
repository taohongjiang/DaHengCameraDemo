#include "centerwidget.h"
#include "QHBoxLayout"
#include "QVBoxLayout"
#include "./widgets/displayWidget/displaywidget.h"
#include "./widgets/connectWidget/connectwidget.h"

DisplayWidget* tgv_displayWidget_p = NULL;

CenterWidget::CenterWidget(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout* tv_mainLayout_p = new QVBoxLayout();
    QHBoxLayout* tv_topLayout_p  = new QHBoxLayout();
    QVBoxLayout* tv_controllerLayout_p = new QVBoxLayout();

    tcv_commandButton_p     = new QPushButton(tr("Start"), this);
	tcv_savePictureButton_p = new QPushButton(tr("Save Picture"), this);
	tcv_saveVideoButton_p   = new QPushButton(tr("Save Video"), this);

    DisplayWidget* tv_displayWidget_p = new DisplayWidget(this);
	tgv_displayWidget_p = tv_displayWidget_p;
    //ConnectWidget* tv_connectWidget_p = new ConnectWidget(this, tv_displayWidget_p);

    tv_controllerLayout_p->addWidget(tcv_commandButton_p);
    tv_controllerLayout_p->addStretch();
    tv_topLayout_p->addWidget(tv_displayWidget_p);
    //tv_topLayout_p->addWidget(tv_connectWidget_p);
	tv_topLayout_p->addStretch();
	tv_topLayout_p->addLayout(tv_controllerLayout_p);
    tv_mainLayout_p->addLayout(tv_topLayout_p);
    //tv_mainLayout_p->addStretch();

    setLayout(tv_mainLayout_p);

	connect(tcv_commandButton_p, SIGNAL(clicked(bool)), tv_displayWidget_p, SLOT(startCapture()));
}

void CenterWidget::resizeEvent(QResizeEvent* size) {
    //tgv_displayWidget_p->resize();
}



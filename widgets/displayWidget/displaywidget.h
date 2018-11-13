#ifndef DISPLAYWIDGET_H
#define DISPLAYWIDGET_H

#include <QWidget>

#include <windows.h>
#include "./widgets/connectWidget/connectwidget.h"

extern "C"
{
#include "GxIAPI.h"
#include "DxImageProc.h"
}


class DisplayWidget : public QWidget
{
    Q_OBJECT
public:
    explicit DisplayWidget(QWidget *parent = 0);
	~DisplayWidget();
    int initDevice();
    void paintEvent(QPaintEvent *event);
	static void __stdcall OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame);
	void ShowErrorString(GX_STATUS emErrorStatus);
	GX_STATUS InitDevice();
	GX_STATUS SetPixelFormat8bit();
	GX_STATUS GetDeviceParam();
	virtual QSize sizeHint() const;
	void resizeEvent(QResizeEvent* size);


	int64_t                 m_nImageWidth;            ///< ������ͼ����
	int64_t                 m_nImageHeight;           ///< ������ͼ��߶�
	int64_t                 m_nPayLoadSize;           ///< �豸���ԭʼͼ���С
	int64_t                 m_nPixelColorFilter;      ///< ��ɫ�����Bayer��ʽ
	bool                    m_bIsColorFilter;         ///< �ж�����Ƿ�֧��Bayer��ʽ

	int     tcv_imageWidth_i;
	int     tcv_imageHeight_i;
	int     tcv_windowWidth_i;
	int     tcv_windowHeight_i;

	QPixmap* tcv_map_p;
	QImage*  tcv_image_p;


public:
    ConnectWidget* tcv_subWindow_p;
    HANDLE deviceChancel;
signals:
    void imageUpdated();
public slots:
	void startCapture();
};

#endif // DISPLAYWIDGET_H

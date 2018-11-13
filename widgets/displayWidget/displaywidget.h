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


	int64_t                 m_nImageWidth;            ///< 相机输出图像宽度
	int64_t                 m_nImageHeight;           ///< 相机输出图像高度
	int64_t                 m_nPayLoadSize;           ///< 设备输出原始图像大小
	int64_t                 m_nPixelColorFilter;      ///< 彩色相机的Bayer格式
	bool                    m_bIsColorFilter;         ///< 判断相机是否支持Bayer格式

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

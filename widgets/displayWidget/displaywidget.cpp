#include "displaywidget.h"
#include <QPainter>
#include <QMessageBox>
extern "C"
{
#include "GxIAPI.h"
#include "DxImageProc.h"
}

volatile unsigned int tgv_state_ui = 0;
GX_DEV_HANDLE hDevice = NULL;
unsigned char* tgv_imageBuffer_p;
unsigned char* tgv_resultBuffer_p;
extern DisplayWidget* tgv_displayWidget_p;

void __stdcall DisplayWidget::OnFrameCallbackFun(GX_FRAME_CALLBACK_PARAM *pFrame)
{
	VxUint32 nImageHeight = (VxUint32)pFrame->nHeight;
	VxUint32 nImageWidth = (VxUint32)pFrame->nWidth;

	if (pFrame->status == 0)	{
		memcpy(tgv_imageBuffer_p, pFrame->pImgBuf, pFrame->nImgSize);
		/*
		memcpy(tgv_resultBuffer_p, pFrame->pImgBuf, pFrame->nImgSize);
		
		// 黑白相机需要翻转数据后显示
		for (VxUint32 i = 0; i < nImageHeight; i++) {
			memcpy(tgv_imageBuffer_p + i * nImageWidth, tgv_resultBuffer_p + (nImageHeight - i - 1) * nImageWidth, (size_t)nImageWidth);
		}*/
	}
	tgv_displayWidget_p->update();
}

DisplayWidget::DisplayWidget(QWidget *parent) : QWidget(parent)
{
    tcv_subWindow_p = NULL;
    //setFixedHeight(576);
    //setFixedWidth(768);
	tcv_imageWidth_i  = 0;
	tcv_imageHeight_i = 0;

	tcv_imageWidth_i   = 752;
	tcv_windowHeight_i = 480;

	unsigned char* tv_displayBuffer_p = new unsigned char[768 * 576];
	GX_STATUS emStatus = GX_STATUS_ERROR;
	//初始化设备库
	
	emStatus = GXInitLib();
	if (emStatus != GX_STATUS_SUCCESS)
	{
		ShowErrorString(emStatus);
		exit(0);
	}

	uint32_t nDeviceNum = 0;

	emStatus = GXUpdateDeviceList(&nDeviceNum, 1000);

	if (emStatus == GX_STATUS_SUCCESS && nDeviceNum > 0)

	{
		GX_DEVICE_BASE_INFO *pBaseinfo = new GX_DEVICE_BASE_INFO[nDeviceNum];

		size_t nSize = nDeviceNum * sizeof(GX_DEVICE_BASE_INFO);



		//获取所有设备的基础信息

		emStatus = GXGetAllDeviceBaseInfo(pBaseinfo, &nSize);

		delete[]pBaseinfo;

		GX_OPEN_PARAM stOpenParam;
		stOpenParam.accessMode = GX_ACCESS_EXCLUSIVE;
		stOpenParam.openMode = GX_OPEN_INDEX;
		stOpenParam.pszContent = (char*)"1";
		emStatus = GXOpenDevice(&stOpenParam, &hDevice);
		//emStatus = GXCloseDevice(hDevice);
		emStatus = InitDevice();
		emStatus = GetDeviceParam();

		//setFixedHeight(m_nImageHeight);
		//setFixedWidth(m_nImageWidth);

		tcv_imageWidth_i  = m_nImageWidth;
		tcv_imageHeight_i = m_nImageHeight;
		tcv_map_p          = new QPixmap(tcv_imageWidth_i, tcv_imageHeight_i);
		tgv_imageBuffer_p  = new unsigned char[tcv_imageWidth_i * tcv_imageHeight_i];
		tgv_resultBuffer_p = new unsigned char[tcv_imageWidth_i * tcv_imageHeight_i];
		tcv_image_p        = new QImage(tcv_imageWidth_i, tcv_imageHeight_i, QImage::Format_Grayscale8);
	}
	else {
		QMessageBox msgBox;
		msgBox.setText("No camera found!");
		msgBox.exec();
	}
}

DisplayWidget::~DisplayWidget() {
	GXCloseDevice(hDevice);
	GXCloseLib();
}

int DisplayWidget::initDevice()
{
    int numDevice=0;
    return numDevice;

}

#define RAYTAP 200
void DisplayWidget::paintEvent(QPaintEvent *event) {
    QWidget::paintEvent(event);

	for (int i = 0; i < tcv_imageWidth_i;i++) {
		if (tgv_imageBuffer_p[tcv_imageWidth_i*tcv_imageHeight_i / 2 + i] < RAYTAP) {
			tgv_imageBuffer_p[tcv_imageWidth_i*tcv_imageHeight_i / 2 + i] = 255;
		}
		else {
			tgv_imageBuffer_p[tcv_imageWidth_i*tcv_imageHeight_i / 2 + i] = 0;
		}
	}

	for (int i = 0; i < tcv_imageHeight_i; i++) {
		if (i != tcv_imageHeight_i / 2) {
			if (tgv_imageBuffer_p[i*tcv_imageWidth_i + tcv_imageWidth_i / 2] < RAYTAP) {
				tgv_imageBuffer_p[i*tcv_imageWidth_i + tcv_imageWidth_i / 2] = 255;
			}
			else {
				tgv_imageBuffer_p[i*tcv_imageWidth_i + tcv_imageWidth_i / 2] = 0;
			}
		}
	}
	memcpy(tcv_image_p->bits(), tgv_imageBuffer_p, tcv_imageWidth_i*tcv_imageHeight_i);
	QPainter tv_painter(this);
	//int tv_windowWidth_i = this->width;
	tv_painter.drawPixmap(0, 0, QPixmap::fromImage(*tcv_image_p));
	//tv_painter.setPen(QColor(255, 255, 255));
	//tv_painter.drawLine(0, m_nImageHeight / 2, tcv_imageWidth_i, m_nImageHeight / 2);
	//tv_painter.drawLine(tcv_imageWidth_i / 2, 0, tcv_imageWidth_i / 2, m_nImageHeight);
}


void DisplayWidget::ShowErrorString(GX_STATUS emErrorStatus)
{
	char*     pchErrorInfo = NULL;
	size_t    nSize = 0;
	
	GX_STATUS emStatus = GX_STATUS_ERROR;

	// 获取错误信息长度，并申请内存资源
	emStatus = GXGetLastError(&emErrorStatus, NULL, &nSize);
	pchErrorInfo = new char[nSize];
	if (NULL == pchErrorInfo)
	{
		return;
	}

	// 获取错误信息，并显示
	emStatus = GXGetLastError(&emErrorStatus, pchErrorInfo, &nSize);
	if (emStatus != GX_STATUS_SUCCESS)
	{
		QMessageBox msgBox;
		msgBox.setText("GXGetLastError failed");
		msgBox.exec();
	}
	else
	{
		QMessageBox msgBox;
		msgBox.setText(pchErrorInfo);
		msgBox.exec();
	}

	// 释放申请的内存资源
	if (NULL != pchErrorInfo)
	{
		delete[] pchErrorInfo;
		pchErrorInfo = NULL;
	}
}

GX_STATUS DisplayWidget::InitDevice()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	HWND hand = HWND(this->winId());
	//设置采集模式连续采集
	emStatus = GXSetEnum(hDevice, GX_ENUM_ACQUISITION_MODE, GX_ACQ_MODE_CONTINUOUS);
	//VERIFY_STATUS_RET(emStatus);

	// 设置触发开关为关
	emStatus = GXSetEnum(hDevice, GX_ENUM_TRIGGER_MODE, GX_TRIGGER_MODE_OFF);
	//VERIFY_STATUS_RET(emStatus);

	// 已知当前相机支持哪个8位图像数据格式可以直接设置
	// 例如设备支持数据格式GX_PIXEL_FORMAT_BAYER_GR8,则可按照以下代码实现
	// emStatus = GXSetEnum(hDevice, GX_ENUM_PIXEL_FORMAT, GX_PIXEL_FORMAT_BAYER_GR8);
	// VERIFY_STATUS_RET(emStatus);

	// 不清楚当前相机的数据格式时，可以调用以下函数将图像数据格式设置为8Bit
	emStatus = SetPixelFormat8bit();

	return emStatus;
}

GX_STATUS DisplayWidget::SetPixelFormat8bit()
{
	GX_STATUS emStatus = GX_STATUS_SUCCESS;
	int64_t   nPixelSize = 0;
	uint32_t  nEnmuEntry = 0;
	size_t    nBufferSize = 0;
	BOOL      bIs8bit = TRUE;

	GX_ENUM_DESCRIPTION  *pEnumDescription = NULL;
	GX_ENUM_DESCRIPTION  *pEnumTemp = NULL;

	// 获取像素点大小
	emStatus = GXGetEnum(hDevice, GX_ENUM_PIXEL_SIZE, &nPixelSize);
	//VERIFY_STATUS_RET(emStatus);

	// 判断为8bit时直接返回,否则设置为8bit
	if (nPixelSize == GX_PIXEL_SIZE_BPP8)
	{
		return GX_STATUS_SUCCESS;
	}
	else
	{
		// 获取设备支持的像素格式枚举数
		emStatus = GXGetEnumEntryNums(hDevice, GX_ENUM_PIXEL_FORMAT, &nEnmuEntry);
		//VERIFY_STATUS_RET(emStatus);

		// 为获取设备支持的像素格式枚举值准备资源
		nBufferSize = nEnmuEntry * sizeof(GX_ENUM_DESCRIPTION);
		pEnumDescription = new GX_ENUM_DESCRIPTION[nEnmuEntry];

		// 获取支持的枚举值
		emStatus = GXGetEnumDescription(hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription, &nBufferSize);
		if (emStatus != GX_STATUS_SUCCESS)
		{
			if (pEnumDescription != NULL)
			{
				delete[]pEnumDescription;
				pEnumDescription = NULL;
			}

			return emStatus;
		}

		// 遍历设备支持的像素格式,设置像素格式为8bit,
		// 如设备支持的像素格式为Mono10和Mono8则设置其为Mono8
		for (uint32_t i = 0; i<nEnmuEntry; i++)
		{
			if ((pEnumDescription[i].nValue & GX_PIXEL_8BIT) == GX_PIXEL_8BIT)
			{
				emStatus = GXSetEnum(hDevice, GX_ENUM_PIXEL_FORMAT, pEnumDescription[i].nValue);
				break;
			}
		}

		// 释放资源
		if (pEnumDescription != NULL)
		{
			delete[]pEnumDescription;
			pEnumDescription = NULL;
		}
	}

	return emStatus;
}

GX_STATUS DisplayWidget::GetDeviceParam()
{
	GX_STATUS emStatus = GX_STATUS_ERROR;
	bool      bIsImplemented = false;

	// 获取图像大小
	emStatus = GXGetInt(hDevice, GX_INT_PAYLOAD_SIZE, &m_nPayLoadSize);
	//VERIFY_STATUS_RET(emStatus);

	// 获取宽度
	emStatus = GXGetInt(hDevice, GX_INT_WIDTH, &m_nImageWidth);
	//VERIFY_STATUS_RET(emStatus);

	// 获取高度
	emStatus = GXGetInt(hDevice, GX_INT_HEIGHT, &m_nImageHeight);
	//VERIFY_STATUS_RET(emStatus);

	// 判断相机是否支持bayer格式
	emStatus = GXIsImplemented(hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_bIsColorFilter);
	//VERIFY_STATUS_RET(emStatus);

	// 支持表示输出彩色图像
	if (m_bIsColorFilter)
	{
		emStatus = GXGetEnum(hDevice, GX_ENUM_PIXEL_COLOR_FILTER, &m_nPixelColorFilter);
	}

	return emStatus;
}

void DisplayWidget::startCapture() {
	GX_STATUS emStatus = GX_STATUS_ERROR;
	emStatus = GXRegisterCaptureCallback(hDevice, this, OnFrameCallbackFun);
	emStatus = GXSendCommand(hDevice, GX_COMMAND_ACQUISITION_START);
}

QSize DisplayWidget::sizeHint() const {
	return QSize(752, 480);
}

void DisplayWidget::resizeEvent(QResizeEvent* size) {
	;
}

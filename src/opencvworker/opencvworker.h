#ifndef OPENCVWORKER_H
#define OPENCVWORKER_H

#include <QObject>
#include <QImage>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <QList>
#include <vector>

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"
#include "opencv2/opencv.hpp"



class OpenCvWorker: public QObject
{
    Q_OBJECT

private:
    //Video capture
    cv::VideoCapture *cap;

    //Variables to process images
    cv::Mat _frameOriginal;
    cv::Mat _frameProcessed;
    cv::Mat _frameThreshold;
    cv::Mat _frameHSV;
    int H_min = 70;
    int H_max = 90;
    int S_min = 66;
    int S_max = 252;
    int V_min = 74;
    int V_max = 255;

    //Variables to detect object
    bool objectDetected = false;
    double area;
    double xCoord;
    double yCoord;
    cv::Moments _frameMoments;
    cv::Mat _frameErode = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3,3));
    cv::Mat _frameDilate = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(8,8));

    //Stream variables
    bool status;
    bool toggleStream;
    bool calibrationMode = false;

    void checkIfDeviceAlreadyOpened(const int device);
    void processColorTracking();


public:
    explicit OpenCvWorker(QObject *parent = nullptr);
    ~OpenCvWorker();

signals:
    void sendFrame(QImage frameProcessed);

public slots:
    void receiveGrabFrame();
    void receiveSetup(const int device);
    void receiveToggleStream();

    void receiveCheckBox();
    void receiveHMIN(int value);
    void receiveSMIN(int value);
    void receiveVMIN(int value);
    void receiveHMAX(int value);
    void receiveSMAX(int value);
    void receiveVMAX(int value);

};

#endif // OPENCVWORKER_H

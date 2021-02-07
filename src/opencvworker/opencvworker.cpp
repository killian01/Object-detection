#include "opencvworker.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/video.hpp"
#include "opencv2/opencv.hpp"

#include <iostream>
#include <vector>
#include <string>
#include <QObject>

using namespace std;

OpenCvWorker::OpenCvWorker(QObject *parent) :
    QObject(parent),
    status(false),
    toggleStream(false)

{
    cap = new cv::VideoCapture;
}

OpenCvWorker::~OpenCvWorker()
{
    if(cap->isOpened()) cap->release();
    delete cap;
}

void OpenCvWorker::receiveGrabFrame()
{
    //prevent capturing of new data if we don't want
    if(!toggleStream) return;

    (*cap).read(_frameOriginal);

    //Verify if not empty
    if(_frameOriginal.empty()) return;

    //process image
    processColorTracking();

    //Show thresholded image if calibration mode
    if(!calibrationMode)
    {
        QImage output((const unsigned char *)_frameProcessed.data, _frameProcessed.cols, _frameProcessed.rows, QImage::Format_RGB888);
        output = output.rgbSwapped();
        emit sendFrame(output);
    }
    else
    {
        QImage output((const unsigned char *)_frameProcessed.data, _frameProcessed.cols, _frameProcessed.rows, QImage::Format_Indexed8);
        emit sendFrame(output);
    }

}

void OpenCvWorker::checkIfDeviceAlreadyOpened(const int device)
{
    if(cap->isOpened())
    {
        cap->release();
    }
    cap->open(device);
}

void OpenCvWorker::processColorTracking()
{
    //Convert image to HSV
    cv::cvtColor(_frameOriginal, _frameHSV, cv::COLOR_RGB2HSV);

    //Treshold for green detection
    cv::inRange(_frameHSV, cv::Scalar(H_min, S_min, V_min), cv::Scalar(H_max, S_max, V_max), _frameThreshold);

    //erode and dilate, remove noise and improve object detection
    cv::erode(_frameThreshold, _frameThreshold, _frameErode);
    cv::dilate(_frameThreshold, _frameThreshold, _frameDilate);

    if (!calibrationMode)
    {
        //Compute moments
        _frameMoments = cv::moments(_frameThreshold);
        area = _frameMoments.m00;

        //Compute center of the area
        if (area > 40000)
        {
            xCoord = _frameMoments.m10/area;
            yCoord = _frameMoments.m01/area;
            objectDetected = true;
        }
        else
        {
             objectDetected = false;
        }

        //Add forms and coordinates of the object
        if (objectDetected == true)
        {
            circle(_frameOriginal,cv::Point(xCoord,yCoord),20,cv::Scalar(0,255,0),2);
            line(_frameOriginal,cv::Point(xCoord,yCoord),cv::Point(xCoord,yCoord-25),cv::Scalar(0,255,0),2);
            line(_frameOriginal,cv::Point(xCoord,yCoord),cv::Point(xCoord,yCoord+25),cv::Scalar(0,255,0),2);
            line(_frameOriginal,cv::Point(xCoord,yCoord),cv::Point(xCoord-25,yCoord),cv::Scalar(0,255,0),2);
            line(_frameOriginal,cv::Point(xCoord,yCoord),cv::Point(xCoord+25,yCoord),cv::Scalar(0,255,0),2);
            cv::putText(_frameOriginal,std::to_string(xCoord)+","+std::to_string(yCoord),cv::Point(xCoord-50,yCoord-30),1,1,cv::Scalar(0,255,0),2);
        }
        _frameProcessed = _frameOriginal;
    }

    else
    {
        _frameProcessed = _frameThreshold;
    }
}

void OpenCvWorker::receiveSetup(const int device)
{
    checkIfDeviceAlreadyOpened(device);
    if(!cap->isOpened())
    {
        status = false;
        return;
    }
    status = true;
}

void OpenCvWorker::receiveToggleStream()
{
    toggleStream = !toggleStream;
}

void OpenCvWorker::receiveCheckBox()
{
    calibrationMode = !calibrationMode;
}

void OpenCvWorker::receiveHMIN(int value)
{
    H_min = value;
}

void OpenCvWorker::receiveSMIN(int value)
{
    S_min = value;
}

void OpenCvWorker::receiveVMIN(int value)
{
    V_min = value;
}

void OpenCvWorker::receiveHMAX(int value)
{
    H_max = value;
}

void OpenCvWorker::receiveSMAX(int value)
{
    S_max = value;
}

void OpenCvWorker::receiveVMAX(int value)
{
    V_max = value;
}

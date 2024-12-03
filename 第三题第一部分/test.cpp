#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

int main() {
    // 打开视频源
    cv::VideoCapture cap(0); // 如果使用摄像头，传入0；如果是文件视频，传入文件路径
    if (!cap.isOpened()) {
        std::cerr << "无法打开视频源!" << std::endl;
        return -1;
    }

    // 创建二维码检测器
    cv::QRCodeDetector qrDecoder;

    cv::Mat frame;
    while (true) {
        cap >> frame; // 读取一帧
        if (frame.empty()) {
            std::cerr << "读取视频帧失败!" << std::endl;
            break;
        }

        std::vector<cv::Point> bbox;
        std::string decodedText = qrDecoder.decode(frame, bbox); // 解码二维码

        if (!decodedText.empty()) {
            // 如果解码成功，绘制二维码的边框
            cv::polylines(frame, bbox, true, cv::Scalar(255, 0, 0), 3); // 绘制二维码的边框
            cv::putText(frame, decodedText, cv::Point(bbox[0].x, bbox[0].y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 0, 0), 2); // 显示二维码内容
        }

        // 显示视频帧
        cv::imshow("QR Code Detection", frame);

        // 按键 'q' 退出循环
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // 释放视频源
    cap.release();
    cv::destroyAllWindows();

    return 0;
}


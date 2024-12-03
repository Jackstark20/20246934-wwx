#include <opencv2/opencv.hpp>
#include <opencv2/objdetect.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <iostream>

int main() {
    // ����ƵԴ
    cv::VideoCapture cap(0); // ���ʹ������ͷ������0��������ļ���Ƶ�������ļ�·��
    if (!cap.isOpened()) {
        std::cerr << "�޷�����ƵԴ!" << std::endl;
        return -1;
    }

    // ������ά������
    cv::QRCodeDetector qrDecoder;

    cv::Mat frame;
    while (true) {
        cap >> frame; // ��ȡһ֡
        if (frame.empty()) {
            std::cerr << "��ȡ��Ƶ֡ʧ��!" << std::endl;
            break;
        }

        std::vector<cv::Point> bbox;
        std::string decodedText = qrDecoder.decode(frame, bbox); // �����ά��

        if (!decodedText.empty()) {
            // �������ɹ������ƶ�ά��ı߿�
            cv::polylines(frame, bbox, true, cv::Scalar(255, 0, 0), 3); // ���ƶ�ά��ı߿�
            cv::putText(frame, decodedText, cv::Point(bbox[0].x, bbox[0].y - 10),
                        cv::FONT_HERSHEY_SIMPLEX, 0.8, cv::Scalar(255, 0, 0), 2); // ��ʾ��ά������
        }

        // ��ʾ��Ƶ֡
        cv::imshow("QR Code Detection", frame);

        // ���� 'q' �˳�ѭ��
        if (cv::waitKey(1) == 'q') {
            break;
        }
    }

    // �ͷ���ƵԴ
    cap.release();
    cv::destroyAllWindows();

    return 0;
}


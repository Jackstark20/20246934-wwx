#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

const Scalar RED_LOW(170, 100, 180);
const Scalar RED_HIGH(179, 255, 255);

const Scalar GREEN_LOW(35, 100, 150);
const Scalar GREEN_HIGH(85, 255, 255);

const Scalar YELLOW_LOW(20, 100, 150);
const Scalar YELLOW_HIGH(30, 255, 255);

string detectTrafficLight(const Mat& frame) {
    Mat hsv, maskRed, maskGreen, maskYellow;
    cvtColor(frame, hsv, COLOR_BGR2HSV);

    // Detect red light
    inRange(hsv, RED_LOW, RED_HIGH, maskRed);
    // Detect green light
    inRange(hsv, GREEN_LOW, GREEN_HIGH, maskGreen);
    // Detect yellow light
    inRange(hsv, YELLOW_LOW, YELLOW_HIGH, maskYellow);

    // Calculate pixel counts
    int redCount = countNonZero(maskRed);
    int greenCount = countNonZero(maskGreen);
    int yellowCount = countNonZero(maskYellow);

    // Determine mode
    if (redCount > 100&& redCount > greenCount) return "RED";
    if (greenCount > 100&&redCount < greenCount) return "GREEN";
    return "NOT";
}


void showMasks(const Mat& frame) {
    Mat hsv, maskRed, maskGreen, maskYellow;
    cvtColor(frame, hsv, COLOR_BGR2HSV);

    // Detect red light
    inRange(hsv, RED_LOW, RED_HIGH, maskRed);
    // Detect green light
    inRange(hsv, GREEN_LOW, GREEN_HIGH, maskGreen);
    // Detect yellow light
    inRange(hsv, YELLOW_LOW, YELLOW_HIGH, maskYellow);

    // Display masks
    imshow("Red Mask", maskRed);
    imshow("Green Mask", maskGreen);
}



void detectAndDrawDominantColor(const Mat& frame) {
    Mat hsv, maskRed, maskGreen, maskYellow;
    cvtColor(frame, hsv, COLOR_BGR2HSV);

    // Detect red light
    inRange(hsv, RED_LOW, RED_HIGH, maskRed);
    // Detect green light
    inRange(hsv, GREEN_LOW, GREEN_HIGH, maskGreen);

    // Calculate pixel counts
    int redCount = countNonZero(maskRed);
    int greenCount = countNonZero(maskGreen);

    // Determine the dominant color
    Mat dominantMask;
    string dominantColor;
    if (redCount > greenCount ) {
        dominantMask = maskRed;
        dominantColor = "RED";
    } else if (greenCount > redCount) {
        dominantMask = maskGreen;
        dominantColor = "GREEN";
    } 
    // Find contours of the dominant color
    vector<vector<Point>> contours;
    findContours(dominantMask, contours, RETR_TREE, CHAIN_APPROX_SIMPLE);

    // Draw rectangles around the dominant color regions
    for (size_t i = 0; i < contours.size(); i++) {
        Rect rect = boundingRect(contours[i]);
        rectangle(frame, rect.tl(), rect.br(), Scalar(0, 255, 0), 2);
    }

    // Display the result
    //putText(frame, "Dominant Color: " + dominantColor, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
    imshow("Dominant Color Detection", frame);
}



void processVideo(const string& videoPath) {
    VideoCapture cap(videoPath);
    if (!cap.isOpened()) {
        cerr << "Error: Cannot open video file." << endl;
        return;
    }

    Mat frame;
    while (cap.read(frame)) {
        string mode = detectTrafficLight(frame);
        putText(frame, mode, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
        imshow("Traffic Light Detection", frame);

        showMasks(frame);
        detectAndDrawDominantColor(frame);

        if (waitKey(30) >= 0) break; // Exit on any key
    }
    cap.release();
}

void processCamera() {
    VideoCapture cap(0);
    if (!cap.isOpened()) {
        cerr << "Error: Cannot access camera." << endl;
        return;
    }

    Mat frame;
    while (true) {
        cap >> frame;
        if (frame.empty()) break;

        string mode = detectTrafficLight(frame);
        putText(frame, mode, Point(10, 30), FONT_HERSHEY_SIMPLEX, 1, Scalar(255, 0, 0), 2);
        imshow("Traffic Light Detection (Camera)", frame);

        showMasks(frame);
        detectAndDrawDominantColor(frame);

        if (waitKey(30) >= 0) break; // Exit on any key
    }
    cap.release();
}

int main() {
    string videoPath = "./video/move_green.avi";
    int n;
    cout << "Enter a number to select file to open: ";
    cin >> n;
    switch (n)
    {
    case 0:
        processCamera();
        break;
    case 1:
        videoPath = "./video/move_green.avi";
        processVideo(videoPath);
        break;
    case 2:
        videoPath = "./video/move_red_yellow.avi";
        processVideo(videoPath);
        break;
    case 3:
        videoPath = "./video/move_red.avi";
        processVideo(videoPath);    
        break;
    case 4:
        videoPath = "./video/random.avi";
        processVideo(videoPath);
        break;
    case 5:
        videoPath = "./video/red_10s.avi";
        processVideo(videoPath);
        break;
    }
    //processVideo(videoPath);
    //processCamera();

    return 0;
}

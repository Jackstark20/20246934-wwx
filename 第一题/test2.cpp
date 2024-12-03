#include <iostream>
#include <fstream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <chrono>

using namespace std;
using namespace cv;

// RGB to YUV Conversion
void RGBToNV12(const Mat& rgb, vector<uint8_t>& nv12) {
    int width = rgb.cols;
    int height = rgb.rows;

    nv12.resize(width * height * 3 / 2);
    uint8_t* y_plane = nv12.data();
    uint8_t* uv_plane = y_plane + width * height;

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            Vec3b pixel = rgb.at<Vec3b>(j, i);
            uint8_t R = pixel[2], G = pixel[1], B = pixel[0];

            uint8_t Y = static_cast<uint8_t>(0.299 * R + 0.587 * G + 0.114 * B);
            y_plane[j * width + i] = Y;

            if (j % 2 == 0 && i % 2 == 0) {
                uint8_t U = static_cast<uint8_t>(-0.14713 * R - 0.28886 * G + 0.436 * B + 128);
                uint8_t V = static_cast<uint8_t>(0.615 * R - 0.51499 * G - 0.10001 * B + 128);

                int uv_index = (j / 2) * (width / 2) + (i / 2);
                uv_plane[uv_index * 2] = U;
                uv_plane[uv_index * 2 + 1] = V;
            }
        }
    }
}

// NV12 to RGB Conversion
Mat NV12ToRGB(const vector<uint8_t>& nv12, int width, int height) {
    Mat rgb(height, width, CV_8UC3);
    const uint8_t* y_plane = nv12.data();
    const uint8_t* uv_plane = y_plane + width * height;

    for (int j = 0; j < height; ++j) {
        for (int i = 0; i < width; ++i) {
            uint8_t Y = y_plane[j * width + i];
            int uv_index = (j / 2) * (width / 2) + (i / 2);
            uint8_t U = uv_plane[uv_index * 2] - 128;
            uint8_t V = uv_plane[uv_index * 2 + 1] - 128;

            int R = static_cast<int>(Y + 1.13983 * V);
            int G = static_cast<int>(Y - 0.39465 * U - 0.58060 * V);
            int B = static_cast<int>(Y + 2.03211 * U);

            rgb.at<Vec3b>(j, i) = Vec3b(
                saturate_cast<uint8_t>(B),
                saturate_cast<uint8_t>(G),
                saturate_cast<uint8_t>(R));
        }
    }
    return rgb;
}

int main() {
    // Load the input image
    Mat rgb = imread("test1.jpg");
    if (rgb.empty()) {
        cerr << "Error: Cannot load image!" << endl;
        return -1;
    }

    int width = rgb.cols;
    int height = rgb.rows;

    vector<uint8_t> nv12;

    // Record start time
    auto start = chrono::high_resolution_clock::now();

    // RGB to NV12
    RGBToNV12(rgb, nv12);

    // Save NV12 to file
    ofstream nv12_file("test1_nv12.yuv", ios::binary);
    nv12_file.write(reinterpret_cast<const char*>(nv12.data()), nv12.size());
    nv12_file.close();

    // NV12 to RGB
    Mat reconstructed_rgb = NV12ToRGB(nv12, width, height);

    // Save the reconstructed RGB image
    imwrite("test1_reconstructed.jpg", reconstructed_rgb);

    // Record end time
    auto end = chrono::high_resolution_clock::now();

    // Calculate time and FPS
    double elapsed = chrono::duration_cast<chrono::milliseconds>(end - start).count();
    cout << "Processing time: " << elapsed << " ms" << endl;
    cout << "FPS: " << 1000.0 / elapsed << endl;

    return 0;
}
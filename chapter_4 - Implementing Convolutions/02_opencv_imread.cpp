#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    
    cv::Mat source = cv::imread("C:\\Users\\luqma\\Downloads\\flower.jpeg", cv::IMREAD_COLOR);

    cv::Mat rotated, gray;
    cv::rotate(source, rotated, cv::ROTATE_90_CLOCKWISE);
    cv::cvtColor(rotated, gray, cv::COLOR_BGR2GRAY);

    cv::imshow("source", source);
    cv::imshow("rotated", rotated);
    cv::imshow("gray", gray);

    cv::imwrite("leds_rot.jpg", gray);

    cv::waitKey();
    
    return 0;
}
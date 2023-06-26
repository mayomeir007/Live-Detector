#pragma once
#include <iostream>
#include <opencv2/opencv.hpp>

//fills the input vector of strings with names of the objects the DNN module can detect
void get_object_names(std::vector<std::string>& class_list);

//loads the model
cv::dnn::Net get_brandbrigade_net();

//draws the bounding rectangle and the label including condifdence and name of object detected
void draw_label(cv::Mat& input_image, std::string label, int left, int top);

//returns a vector of the detections found on the given 'Mat' using the given model
std::vector<cv::Mat> pre_process(cv::Mat& input_image, cv::dnn::Net& net, const std::vector<cv::String>& outNames);

//filters out detections 'outputs' based on score, confidence and overlap, and draws them on the input 'Mat' 
cv::Mat post_process(cv::Mat& input_image, std::vector<cv::Mat>& outputs, const std::vector<std::string>& class_name);





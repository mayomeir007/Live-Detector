#include "pch.h"
#include "OpenCV_dnn.h"

#include <fstream>
// Namespaces.
using namespace cv;
using namespace std;
using namespace cv::dnn;

// Constants.
const float INPUT_WIDTH = 256.0;
const float INPUT_HEIGHT = 256.0;
const float SCORE_THRESHOLD = 0.5;
const float NMS_THRESHOLD = 0.45;
const float CONFIDENCE_THRESHOLD = 0.45;

// Text parameters.
const float FONT_SCALE = 0.7;
const int FONT_FACE = FONT_HERSHEY_SIMPLEX;
const int THICKNESS = 1;

// Colors.
Scalar BLACK = Scalar(0, 0, 0);
Scalar BLUE = Scalar(255, 178, 50);
Scalar YELLOW = Scalar(0, 255, 255);
Scalar RED = Scalar(0, 0, 255);


void get_object_names(vector<string>& class_list)
{
	ifstream ifs("../../../../coco.names");
	string line;
	while (getline(ifs, line))
	{
		class_list.push_back(line);
	}
}

Net get_brandbrigade_net()
{
	string model = "yolov3.weights";
	string config = "../../../../yolov3.cfg";
	return readNet(model, config, "Darknet");

}

void draw_label(Mat& input_image, string label, int left, int top)
{
	// Display the label at the top of the bounding box.
	int baseLine;
	cv::Size label_size = getTextSize(label, FONT_FACE, FONT_SCALE, THICKNESS, &baseLine);
	top = max(top, label_size.height);
	// Top left corner.
	cv::Point tlc = cv::Point(left, top);
	// Bottom right corner.
	cv::Point brc = cv::Point(left + label_size.width, top + label_size.height + baseLine);
	// Draw white rectangle.
	rectangle(input_image, tlc, brc, BLACK, FILLED);
	// Put the label on the black rectangle.
	putText(input_image, label, cv::Point(left, top + label_size.height), FONT_FACE, FONT_SCALE, YELLOW, THICKNESS);
}

vector<Mat> pre_process(Mat& input_image, Net& net, const vector<cv::String>& outNames)
{
	// Convert to blob.

	Mat blob;
	blobFromImage(input_image, blob, 1. / 255., cv::Size(INPUT_WIDTH, INPUT_HEIGHT), Scalar(), true, false);

	net.setInput(blob);
	cout << "before forward" << time(nullptr) << endl;

	// Forward propagate.
	vector<Mat> outputs;
	Mat output;
	net.forward(outputs, outNames);
	cout << "after forward" << time(nullptr) << endl;

	//outputs.push_back(output);
	return outputs;
}

Mat post_process(Mat& input_image, vector<Mat>& outputs, const vector<string>& class_name)
{
	// Initialize vectors to hold respective outputs while unwrapping     detections.
	vector<int> class_ids;
	vector<float> confidences;
	vector<Rect> boxes;
	// Resizing factor.
	float x_factor = input_image.cols;// / INPUT_WIDTH;
	float y_factor = input_image.rows;// / INPUT_HEIGHT;
	float* data = (float*)outputs[0].data;
	const int dimensions = 85;
	// 25200 for default size 640.
	const int rows = outputs[0].rows; // 25200;
	// Iterate through 25200 detections.
	for (int i = 0; i < rows; ++i)
	{
		float confidence = data[4];
		// Discard bad detections and continue.
		if (confidence >= CONFIDENCE_THRESHOLD)
		{
			float* classes_scores = data + 5;
			// Create a 1x85 Mat and store class scores of 80 classes.
			Mat scores(1, class_name.size(), CV_32FC1, classes_scores);
			// Perform minMaxLoc and acquire the index of best class  score.
			cv::Point class_id;
			double max_class_score;
			minMaxLoc(scores, 0, &max_class_score, 0, &class_id);
			// Continue if the class score is above the threshold.
			if (max_class_score > SCORE_THRESHOLD)
			{
				// Store class ID and confidence in the pre-defined respective vectors.
				confidences.push_back(confidence);
				class_ids.push_back(class_id.x);
				// Center.
				float cx = data[0];
				float cy = data[1];
				// Box dimension.
				float w = data[2];
				float h = data[3];
				// Bounding box coordinates.
				int left = int((cx - 0.5 * w) * x_factor);
				int top = int((cy - 0.5 * h) * y_factor);
				int width = int(w * x_factor);
				int height = int(h * y_factor);
				// Store good detections in the boxes vector.
				boxes.push_back(Rect(left, top, width, height));
			}
		}
		// Jump to the next row.
		data += 85;
	}
	// Perform Non-Maximum Suppression and draw predictions.
	vector<int> indices;
	NMSBoxes(boxes, confidences, SCORE_THRESHOLD, NMS_THRESHOLD, indices);
	for (int i = 0; i < indices.size(); i++)
	{
		int idx = indices[i];
		Rect box = boxes[idx];
		int left = box.x;
		int top = box.y;
		int width = box.width;
		int height = box.height;
		// Draw bounding box.
		rectangle(input_image, cv::Point(left, top), cv::Point(left + width, top + height), BLUE, 3 * THICKNESS);
		// Get the label for the class name and its confidence.
		string label = format("%.2f", confidences[idx]);
		label = class_name[class_ids[idx]] + ":" + label;
		// Draw class labels.
		draw_label(input_image, label, left, top);
	}
	return input_image;
}


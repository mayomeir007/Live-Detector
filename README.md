### Prerequisites
1. [OpenCV 3.0](https://opencv.org/)
2. Go to https://github.com/patrick013/Object-Detection---Yolov3/tree/master/model and download the weights file 'yolov3.weights'. Place this file in the output path to the binaries you specified.

### Description
This is a windows application that allows capturing a video from either a webcam or a file, detect
objects in the video’s frame, and show the frame and the detected objects using rectangles and
text-labels.

The C# part implements the GUI only, and delegates all the calls to a C++ dll.

In the C++ DLL, the following technologies are intgerated:

1. MediaLook MFormats SDK - for capturing the frames from the webcam and video files 
2. OpenCV's DNN module - for detecting objects in the frames
3. OpenCV - for drawing on the frames 
4. CLR windows forms - for displaying the frames on the panel window of the C# GUI
5. Finally, for calling C++ functions in a C# environment, I utilized data type marshalling

The dll also uses multithreading to enable switching between webcam, video file, and stopping options during runtime.

Webcam
![image](https://github.com/mayomeir007/BrandBrigade3/assets/59373530/b0009fda-e364-411a-94d0-a7f37c565149)

File
![image](https://github.com/mayomeir007/BrandBrigade3/assets/59373530/0c979a54-3779-48ce-8c88-9022704eb58f)








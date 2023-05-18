#include "./include/main.h"

int main()
{
    int num_apex = 4;
    cv::namedWindow("Test", cv::WINDOW_NORMAL);
    // cv::namedWindow("Test2", cv::WINDOW_NORMAL);
    // cv::namedWindow("Test3", cv::WINDOW_NORMAL);
    // cv::namedWindow("Test4", cv::WINDOW_NORMAL);
    TRTInferV1::TRTInfer myInfer(0);
    // nvinfer1::IHostMemory *data = myInfer.createEngine("/home/ninefish/nine-fish/TRTInferenceForYolo/sample/build/yolox.onnx", 8, 416, 416);
    // myInfer.saveEngineFile(data, "/home/ninefish/nine-fish/TRTInferenceForYolo/sample/engines/model_trt.engine");
    myInfer.initModule("/home/ninefish/nine-fish/TRTInferenceForYolo/sample/engines/model_trt.engine", 4, num_apex, 8, 8, 128);

    cv::VideoCapture cap(0);
    std::vector<cv::Mat> frames;

    // cv::Mat src = cv::imread("/home/ninefish/nine-fish/TRTInferenceForYoloX/sample/46.jpg");
    // cv::Mat src2 = cv::imread("/home/ninefish/nine-fish/TRTInferenceForYoloX/sample/SAU0076.jpg");
    // cv::Mat src3 = cv::imread("/home/ninefish/nine-fish/TRTInferenceForYoloX/sample/1674.jpg");
    cv::Mat src4 = cv::imread("/home/ninefish/nine-fish/TRTInferenceForYolo/sample/SAU0830.jpg");

    myInfer.calculate_inter_frame_compensation(120);

    while (true)
    {
        frames.clear();
        // cv::Mat img = src.clone();
        // cv::Mat img2 = src2.clone();
        // cv::Mat img3 = src3.clone();
        // cv::Mat img4 = src4.clone();
        // if (!cap.isOpened())
        // {
        //     continue;
        // }
        // cv::Mat img, img2, img3, img4;
        // cap.read(img);
        // img2 = img.clone();
        // img3 = img.clone();
        // img4 = img.clone();
        // assert(!img.empty());
        frames.emplace_back(src4);
        // frames.emplace_back(img2);
        // frames.emplace_back(img3);
        // frames.emplace_back(img4);
        auto start_t = std::chrono::system_clock::now().time_since_epoch();
        std::vector<std::vector<TRTInferV1::DetectObject>> result = myInfer.doInferenceLimitFPS(frames, 0.9, 0.5, 120);
        auto end_t = std::chrono::system_clock::now().time_since_epoch();
        char ch[255];
        for (int i(0); i < int(frames.size()); ++i)
        {
            for (int j(0); j < int(result[i].size()); ++j)
            {
                for (int k(0); k < num_apex; ++k)
                {
                    if (k < num_apex - 1)
                        cv::line(frames[i], result[i][j].pts[k], result[i][j].pts[k + 1], cv::Scalar(255, 255, 255), 1);
                    else
                        cv::line(frames[i], result[i][j].pts[k], result[i][j].pts[0], cv::Scalar(255, 255, 255), 1);
                }
            }

            sprintf(ch, "FPS %d", int(std::chrono::nanoseconds(1000000000).count() / (end_t - start_t).count()));
            std::string fps_str = ch;
            cv::putText(frames[i], fps_str, {10, 25}, cv::FONT_HERSHEY_SIMPLEX, 1, {0, 255, 0});
        }
        std::cout << ch << std::endl;
        cv::imshow("Test", frames[0]);
        // cv::imshow("Test2", frames[1]);
        // cv::imshow("Test3", frames[2]);
        // cv::imshow("Test4", frames[3]);
        cv::waitKey(1);
    }

    return 0;
}
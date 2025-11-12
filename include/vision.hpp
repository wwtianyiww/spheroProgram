#ifndef PROGRAMSPHERO_VISION_HPP
#define PROGRAMSPHERO_VISION_HPP
 #include <server.hpp>
#include <opencv2/opencv.hpp>


class Vision {
    public:
    struct DetectionResult {
        std::string objectType;
        cv::Rect boundingBox;
        float confidence; //how certain the object is what it is detected as
        cv::Point2f centerPoint;
    };

//constructor/destructor
    Vision();
    ~Vision();

    void initialize();
    void shutdown(cv::VideoCapture camera);
    void segment(cv::Mat& frame);

    void processFrame(const cv::Mat& frame);

    std::vector<DetectionResult> getDetections() const;

    // Callbacks for real-time processing
    using FrameCallback = std::function<void(const cv::Mat&)>;
    using DetectionCallback = std::function<void(const std::vector<DetectionResult>&)>;

    void setFrameCallback(FrameCallback callback);
    void setDetectionCallback(DetectionCallback callback);




    private:
    //void objectDetection();
    //void objectTracking();
    void thresholding();
    cv::Mat preprocessImage(const cv::Mat& image);
    cv::Mat currentFrame;
    cv::Mat processedFrame;
    std::vector<DetectionResult> currentDetection;

    FrameCallback frameCallback;
    DetectionCallback detectionCallback;

};



#endif //PROGRAMSPHERO_VISION_HPP
#ifndef IMAGEDENOISER_H
#define IMAGEDENOISER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <string>
#include <map>

struct DenoisingMetrics {
    std::string methodName;
    double mse;
    double psnr;
    double ssim;
    
    DenoisingMetrics(const std::string& name, double m, double p, double s) 
        : methodName(name), mse(m), psnr(p), ssim(s) {}
};

class ImageDenoiser {
private:
    cv::Mat originalImage;
    cv::Mat noisyImage;
    std::map<std::string, cv::Mat> denoisedImages;
    
public:
    ImageDenoiser();
    bool loadImage(const std::string& imagePath);
    cv::Mat addGaussianNoise(double mean = 0, double sigma = 25);
    cv::Mat addSaltPepperNoise(double saltProb = 0.01, double pepperProb = 0.01);
    cv::Mat applyGaussianFilter(int kernelSize = 5, double sigma = 1.0);
    cv::Mat applyMedianFilter(int kernelSize = 5);
    cv::Mat applyBilateralFilter(int d = 9, double sigmaColor = 75, double sigmaSpace = 75);
    cv::Mat applyNLMFilter(double h = 10, int templateWindowSize = 7, int searchWindowSize = 21);
    DenoisingMetrics calculateMetrics(const cv::Mat& denoisedImage, const std::string& methodName);
    std::vector<DenoisingMetrics> compareAllMethods();
    cv::Mat getOriginalImage() const { return originalImage; }
    cv::Mat getNoisyImage() const { return noisyImage; }
    std::map<std::string, cv::Mat> getDenoisedImages() const { return denoisedImages; }
    void saveResults(const std::string& outputDir);
    
private:
    double calculateMSE(const cv::Mat& image1, const cv::Mat& image2);
    double calculatePSNR(const cv::Mat& image1, const cv::Mat& image2);
    double calculateSSIM(const cv::Mat& image1, const cv::Mat& image2);
};

#endif
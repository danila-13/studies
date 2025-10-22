#include "ImageDenoiser.h"
#include <opencv2/opencv.hpp>
#include <random>
#include <iostream>
#include <filesystem>

ImageDenoiser::ImageDenoiser() {}

bool ImageDenoiser::loadImage(const std::string& imagePath) {
    originalImage = cv::imread(imagePath);
    if (originalImage.empty()) {
        std::cerr << "Ошибка: Не удалось загрузить изображение " << imagePath << std::endl;
        return false;
    }
    cv::cvtColor(originalImage, originalImage, cv::COLOR_BGR2RGB);
    return true;
}

cv::Mat ImageDenoiser::addGaussianNoise(double mean, double sigma) {
    if (originalImage.empty()) return cv::Mat();
    
    cv::Mat noise = cv::Mat(originalImage.size(), originalImage.type());
    cv::randn(noise, mean, sigma);
    
    cv::Mat noisyImageFloat;
    originalImage.convertTo(noisyImageFloat, CV_32FC3);
    noise.convertTo(noise, CV_32FC3);
    
    noisyImageFloat += noise;
    cv::convertScaleAbs(noisyImageFloat, noisyImage);
    return noisyImage;
}

cv::Mat ImageDenoiser::addSaltPepperNoise(double saltProb, double pepperProb) {
    if (originalImage.empty()) return cv::Mat();
    
    noisyImage = originalImage.clone();
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0.0, 1.0);
    
    for (int i = 0; i < noisyImage.rows; i++) {
        for (int j = 0; j < noisyImage.cols; j++) {
            double random_value = dis(gen);
            if (random_value < saltProb) {
                noisyImage.at<cv::Vec3b>(i, j) = cv::Vec3b(255, 255, 255);
            } else if (random_value < saltProb + pepperProb) {
                noisyImage.at<cv::Vec3b>(i, j) = cv::Vec3b(0, 0, 0);
            }
        }
    }
    return noisyImage;
}

cv::Mat ImageDenoiser::applyGaussianFilter(int kernelSize, double sigma) {
    if (noisyImage.empty()) return cv::Mat();
    cv::Mat denoised;
    cv::GaussianBlur(noisyImage, denoised, cv::Size(kernelSize, kernelSize), sigma);
    denoisedImages["Gaussian"] = denoised;
    return denoised;
}

cv::Mat ImageDenoiser::applyMedianFilter(int kernelSize) {
    if (noisyImage.empty()) return cv::Mat();
    cv::Mat denoised;
    cv::medianBlur(noisyImage, denoised, kernelSize);
    denoisedImages["Median"] = denoised;
    return denoised;
}

cv::Mat ImageDenoiser::applyBilateralFilter(int d, double sigmaColor, double sigmaSpace) {
    if (noisyImage.empty()) return cv::Mat();
    cv::Mat denoised;
    cv::bilateralFilter(noisyImage, denoised, d, sigmaColor, sigmaSpace);
    denoisedImages["Bilateral"] = denoised;
    return denoised;
}

cv::Mat ImageDenoiser::applyNLMFilter(double h, int templateWindowSize, int searchWindowSize) {
    if (noisyImage.empty()) return cv::Mat();
    cv::Mat denoised;
    cv::fastNlMeansDenoisingColored(noisyImage, denoised, h, h, templateWindowSize, searchWindowSize);
    denoisedImages["NLM"] = denoised;
    return denoised;
}

double ImageDenoiser::calculateMSE(const cv::Mat& image1, const cv::Mat& image2) {
    if (image1.size() != image2.size() || image1.type() != image2.type()) return -1;
    cv::Mat diff;
    cv::absdiff(image1, image2, diff);
    diff.convertTo(diff, CV_32F);
    diff = diff.mul(diff);
    cv::Scalar mse = cv::mean(diff);
    return (mse[0] + mse[1] + mse[2]) / 3.0;
}

double ImageDenoiser::calculatePSNR(const cv::Mat& image1, const cv::Mat& image2) {
    double mse = calculateMSE(image1, image2);
    if (mse <= 0) return 0;
    return 10.0 * log10((255.0 * 255.0) / mse);
}

double ImageDenoiser::calculateSSIM(const cv::Mat& image1, const cv::Mat& image2) {
    if (image1.size() != image2.size() || image1.type() != image2.type()) return -1;
    
    const double C1 = 6.5025, C2 = 58.5225;
    cv::Mat I1, I2;
    image1.convertTo(I1, CV_32F);
    image2.convertTo(I2, CV_32F);
    
    cv::Mat mu1, mu2;
    cv::GaussianBlur(I1, mu1, cv::Size(11, 11), 1.5);
    cv::GaussianBlur(I2, mu2, cv::Size(11, 11), 1.5);
    
    cv::Mat mu1_2 = mu1.mul(mu1);
    cv::Mat mu2_2 = mu2.mul(mu2);
    cv::Mat mu1_mu2 = mu1.mul(mu2);
    
    cv::Mat sigma1_2, sigma2_2, sigma12;
    cv::GaussianBlur(I1.mul(I1), sigma1_2, cv::Size(11, 11), 1.5);
    cv::GaussianBlur(I2.mul(I2), sigma2_2, cv::Size(11, 11), 1.5);
    cv::GaussianBlur(I1.mul(I2), sigma12, cv::Size(11, 11), 1.5);
    
    sigma1_2 -= mu1_2;
    sigma2_2 -= mu2_2;
    sigma12 -= mu1_mu2;
    
    cv::Mat t1 = 2 * mu1_mu2 + C1;
    cv::Mat t2 = 2 * sigma12 + C2;
    cv::Mat t3 = t1.mul(t2);
    
    t1 = mu1_2 + mu2_2 + C1;
    t2 = sigma1_2 + sigma2_2 + C2;
    t1 = t1.mul(t2);
    
    cv::Mat ssim_map;
    cv::divide(t3, t1, ssim_map);
    return cv::mean(ssim_map)[0];
}

DenoisingMetrics ImageDenoiser::calculateMetrics(const cv::Mat& denoisedImage, const std::string& methodName) {
    if (originalImage.empty() || denoisedImage.empty()) {
        return DenoisingMetrics(methodName, -1, -1, -1);
    }
    double mse = calculateMSE(originalImage, denoisedImage);
    double psnr = calculatePSNR(originalImage, denoisedImage);
    double ssim = calculateSSIM(originalImage, denoisedImage);
    return DenoisingMetrics(methodName, mse, psnr, ssim);
}

std::vector<DenoisingMetrics> ImageDenoiser::compareAllMethods() {
    std::vector<DenoisingMetrics> results;
    if (noisyImage.empty()) return results;
    
    applyGaussianFilter();
    applyMedianFilter();
    applyBilateralFilter();
    applyNLMFilter();
    
    for (const auto& pair : denoisedImages) {
        DenoisingMetrics metrics = calculateMetrics(pair.second, pair.first);
        results.push_back(metrics);
    }
    return results;
}

void ImageDenoiser::saveResults(const std::string& outputDir) {
    std::filesystem::create_directories(outputDir);
    
    cv::Mat originalBGR;
    cv::cvtColor(originalImage, originalBGR, cv::COLOR_RGB2BGR);
    cv::imwrite(outputDir + "/original.jpg", originalBGR);
    
    if (!noisyImage.empty()) {
        cv::Mat noisyBGR;
        cv::cvtColor(noisyImage, noisyBGR, cv::COLOR_RGB2BGR);
        cv::imwrite(outputDir + "/noisy.jpg", noisyBGR);
    }
    
    for (const auto& pair : denoisedImages) {
        cv::Mat denoisedBGR;
        cv::cvtColor(pair.second, denoisedBGR, cv::COLOR_RGB2BGR);
        cv::imwrite(outputDir + "/denoised_" + pair.first + ".jpg", denoisedBGR);
    }
}
#include <iostream>
#include <iomanip>
#include "ImageDenoiser.h"

void printMetrics(const std::vector<DenoisingMetrics>& metrics) {
    std::cout << "\n" << std::string(80, '=') << std::endl;
    std::cout << "РЕЗУЛЬТАТЫ ТЕСТИРОВАНИЯ МЕТОДОВ УДАЛЕНИЯ ШУМОВ" << std::endl;
    std::cout << std::string(80, '=') << std::endl;
    
    std::cout << std::left << std::setw(15) << "Метод" 
              << std::setw(15) << "MSE" 
              << std::setw(15) << "PSNR (dB)" 
              << std::setw(15) << "SSIM" << std::endl;
    std::cout << std::string(80, '-') << std::endl;
    
    for (const auto& metric : metrics) {
        std::cout << std::left << std::setw(15) << metric.methodName
                  << std::setw(15) << std::fixed << std::setprecision(2) << metric.mse
                  << std::setw(15) << std::fixed << std::setprecision(2) << metric.psnr
                  << std::setw(15) << std::fixed << std::setprecision(4) << metric.ssim << std::endl;
    }
    
    auto bestPsnr = std::max_element(metrics.begin(), metrics.end(),
        [](const DenoisingMetrics& a, const DenoisingMetrics& b) {
            return a.psnr < b.psnr;
        });
    
    auto bestSsim = std::max_element(metrics.begin(), metrics.end(),
        [](const DenoisingMetrics& a, const DenoisingMetrics& b) {
            return a.ssim < b.ssim;
        });
    
    std::cout << std::string(80, '=') << std::endl;
    std::cout << "ЛУЧШИЕ РЕЗУЛЬТАТЫ:" << std::endl;
    std::cout << "По PSNR: " << bestPsnr->methodName << " (" << bestPsnr->psnr << " dB)" << std::endl;
    std::cout << "По SSIM: " << bestSsim->methodName << " (" << bestSsim->ssim << ")" << std::endl;
}

int main() {
    std::cout << "ПРОГРАММА ДЛЯ УДАЛЕНИЯ ШУМОВ С ИЗОБРАЖЕНИЙ" << std::endl;
    std::cout << "==========================================" << std::endl;
    
    ImageDenoiser denoiser;
    
    std::string imagePath;
    std::cout << "Введите путь к изображению: ";
    std::cin >> imagePath;
    
    if (!denoiser.loadImage(imagePath)) {
        std::cerr << "Не удалось загрузить изображение!" << std::endl;
        return -1;
    }
    
    std::cout << "Изображение успешно загружено!" << std::endl;
    
    int noiseType;
    std::cout << "\nВыберите тип шума:" << std::endl;
    std::cout << "1 - Гауссовский шум" << std::endl;
    std::cout << "2 - Солевой-перечный шум" << std::endl;
    std::cout << "Ваш выбор: ";
    std::cin >> noiseType;
    
    switch (noiseType) {
        case 1:
            denoiser.addGaussianNoise(0, 25);
            std::cout << "Добавлен гауссовский шум" << std::endl;
            break;
        case 2:
            denoiser.addSaltPepperNoise(0.01, 0.01);
            std::cout << "Добавлен солевой-перечный шум" << std::endl;
            break;
        default:
            denoiser.addGaussianNoise();
            break;
    }
    
    std::cout << "\nЗапуск тестирования методов..." << std::endl;
    std::vector<DenoisingMetrics> results = denoiser.compareAllMethods();
    printMetrics(results);
    
    std::string outputDir = "./denoising_results";
    denoiser.saveResults(outputDir);
    std::cout << "\nРезультаты сохранены в папке: " << outputDir << std::endl;
    
    return 0;
}
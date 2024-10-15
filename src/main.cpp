#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>



int main ()
{
    setlocale (0, "Russian");
    cv:: Mat img1 = cv::imread("test1_1.jpg", cv:: IMREAD_GRAYSCALE);
    cv:: Mat img2 = cv::imread("test1_2.jpg", cv:: IMREAD_GRAYSCALE);
    // img1.convertTo(img1, CV_8U);
    // img2.convertTo(img2, CV_8U);
    if (img1.empty() || img2.empty()){
        std::cerr << "Не удалось загрузить изображение";
        return -1;
    }

    //cv:: Ptr<cv::ORB> orb = cv::ORB::create();
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv:: Mat descriptors1, descriptors2;

    cv::Ptr<cv::SIFT> sift = cv::SIFT::create();
    sift->detectAndCompute(img1, cv::Mat(), keypoints1, descriptors1);
    sift->detectAndCompute(img2, cv::Mat(), keypoints2, descriptors2);

    cv::FlannBasedMatcher matcher(new cv::flann::KDTreeIndexParams(), new cv::flann::SearchParams());
    descriptors1.convertTo(descriptors1, CV_32F);
    descriptors2.convertTo(descriptors2, CV_32F);
    std::vector<std::vector<cv::DMatch>> knn_matches;
    matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);

    const float ratio_thresh = 0.75f;
    std:: vector<cv::DMatch> good_matches;
    
    for (const auto& match : knn_matches) {
        if (match[0].distance < ratio_thresh * match[1].distance) {
            good_matches.push_back(match[0]);
        }
    }
    if (good_matches.size() < 7) {

        std:: cerr << "Недостаточно совпадений";
        return -1;

    }

    std::vector<cv::Point2f> points1, points2;

    for (size_t i =0; i < good_matches.size(); i++){

        points1.push_back(keypoints1[good_matches[i].queryIdx].pt);
        points2.push_back(keypoints2[good_matches[i].trainIdx].pt);
    }


   //Метод афинного преобразования
    cv::Mat affine_matrix = cv::estimateAffinePartial2D(points2, points1);
  

    

    std::vector<cv::Point2f> img2_corners(3);

    img2_corners[0] = cv::Point2f(0,0);
    img2_corners[1] = cv::Point2f(static_cast<float>(img2.cols), 0);
    img2_corners[2] = cv::Point2f(0, static_cast<float>(img2.rows));
    std::vector<cv::Point2f> img2_transformed_corners(3);
    cv::transform(img2_corners, img2_transformed_corners, affine_matrix);

    float min_x = std::min(0.0f, std::min({img2_transformed_corners[0].x, img2_transformed_corners[1].x, img2_transformed_corners[2].x }));
    float min_y = std::min(0.0f, std::min({img2_transformed_corners[0].y, img2_transformed_corners[1].y, img2_transformed_corners[2].y}));
    float max_x = std::max(static_cast<float>(img1.cols), std::max({img2_transformed_corners[0].x, img2_transformed_corners[1].x, img2_transformed_corners[2].x}));
    float max_y = std::max(static_cast<float>(img1.rows), std::max({img2_transformed_corners[0].y, img2_transformed_corners[1].y, img2_transformed_corners[2].y}));
    
    cv::Size result_size(static_cast<int>(std::ceil(max_x - min_x)), static_cast<int>(std::ceil(max_y - min_y)));

    cv::Mat result(result_size, img1.type(), cv::Scalar::all(255));

    cv:: Mat translation_matrix = (cv::Mat_<double>(2, 3) << 1, 0, -min_x, 0 , 1, -min_y);
    
    cv::warpAffine(img1, result, translation_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    // cv::imshow("img1", img1);
    // cv::imshow("img2", img2);
    //cv::drawKeypoints(img1, keypoints1, result);
    //cv::drawKeypoints(img2, keypoints2, result);
    
    cv::imshow("result", result);
    cv::waitKey(0);

    cv::Mat img2_transformed;

    cv::Mat img2_shifted;
    cv::warpAffine(img2, img2_shifted, translation_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);

// Теперь применяем affine_matrix после сдвига
    cv::warpAffine(img2_shifted, img2_transformed, affine_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
   

    cv::imshow("result", result);
    cv::waitKey(0);

    cv::imshow("img2_transformed", img2_transformed);
    cv::waitKey(0);

    
    cv::Mat mask = cv::Mat::zeros(result.size(), CV_32F);

    int blendWidth = 50; // Ширина области смешивания (можно подстроить)
    
    cv::Rect blendRect(static_cast<int>(img2_transformed_corners[0].x), 
                   static_cast<int>(img2_transformed_corners[0].y), 
                   img2.cols + blendWidth, img2.rows + blendWidth);
    

    
    for (int y = blendRect.y; y < blendRect.y + blendRect.height && y < mask.rows; ++y) {
        for (int x = blendRect.x; x < blendRect.x + blendRect.width && x < mask.cols; ++x) {
            float alpha = static_cast<float>(x - blendRect.x) / blendWidth;
            alpha = std::min(std::max(alpha, 0.0f), 1.0f); // Ограничиваем значение в диапазоне [0, 1]
            mask.at<float>(y, x) = alpha;
        }
    }
    

// Преобразуем изображения в 32-битный формат, чтобы выполнить смешивание
    result.convertTo(result, CV_32F);
    img2_transformed.convertTo(img2_transformed, CV_32F);

// Плавное смешивание изображений в области перекрытия
    cv::Mat blended;
    blended = result.mul(1.0 - mask) + img2_transformed.mul(mask);
    std::cout << "ошибка не тут5" << std::endl;
    
// Приводим результат обратно в 8-битный формат для отображения
    blended.convertTo(blended, CV_8U);

// Отображаем сшитое изображение
    cv::imshow("Сшитое изображение", blended);
    cv::waitKey(0);

    return 0;
}
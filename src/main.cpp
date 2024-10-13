#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>



int main ()
{
    setlocale (0, "Russian");
    cv:: Mat img1 = cv::imread("image1.jpg", cv:: IMREAD_GRAYSCALE);
    cv:: Mat img2 = cv::imread("image2.jpg", cv:: IMREAD_GRAYSCALE);
    
    if (img1.empty() || img2.empty()){
        std::cerr << "Не удалось загрузить изображение";
        return -1;
    }

    cv:: Ptr<cv::ORB> orb = cv::ORB::create();
    std::vector<cv::KeyPoint> keypoints1, keypoints2;
    cv:: Mat descriptors1, descriptors2;

    orb->detectAndCompute(img1, cv::Mat(), keypoints1, descriptors1);
    orb->detectAndCompute(img2, cv::Mat(), keypoints2, descriptors2);

    cv::BFMatcher bf(cv::NORM_HAMMING);
    std::vector<std::vector<cv::DMatch>> knn_matches;
    bf.knnMatch(descriptors1, descriptors2, knn_matches, 2);

    const float ratio_thresh = 0.75f;
    std:: vector<cv::DMatch> good_matches;
    
   for (size_t i =0; i < knn_matches.size(); i++){
        if (knn_matches[i][0].distance < ratio_thresh*knn_matches[i][1].distance){
            good_matches.push_back(knn_matches[i][0]);
        }
   }
    if (good_matches.size() < 20) {

        std:: cerr << "Недостаточно совпадений";
        return -1;

    }

    std::vector<cv::Point2f> points1, points2;

    for (size_t i =0; i < good_matches.size(); i++){

        points1.push_back(keypoints1[good_matches[i].queryIdx].pt);
        points2.push_back(keypoints2[good_matches[i].trainIdx].pt);
    }


   // Метод афинного преобразования
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

    cv::Mat result(result_size, img1.type(), cv::Scalar::all(0));

    cv:: Mat translation_matrix = (cv::Mat_<double>(2, 3) << 1, 0, -min_x, 0 , 1, -min_y);
    
    cv::warpAffine(img1, result, translation_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);
    
   
    cv::Mat combined_matrix = cv::Mat::zeros(2, 3, CV_64F);


    affine_matrix(cv::Rect(0, 0, 2, 2)).copyTo(combined_matrix(cv::Rect(0, 0, 2, 2)));  

    // Обновляем трансляцию
    combined_matrix.at<double>(0, 2) = affine_matrix.at<double>(0, 2) + translation_matrix.at<double>(0, 2);
    combined_matrix.at<double>(1, 2) = affine_matrix.at<double>(1, 2) + translation_matrix.at<double>(1, 2);

    cv::Mat img2_transformed;
    cv::warpAffine(img2, img2_transformed, combined_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_TRANSPARENT);

   
    cv::normalize(result, result, 0, 255, cv::NORM_MINMAX);
    cv::normalize(img2_transformed, img2_transformed, 1, 255, cv::NORM_MINMAX);
    cv::imshow("Сшитое изображение", result);
    cv::waitKey(0);

    cv::imshow("Сшитое изображение", img2_transformed);
    cv::waitKey(0);
    cv::addWeighted(result, 0.45, img2_transformed, 0.35, 0.2, result);

   // Гомографическое преобразование
    // cv:: Mat homography = cv::findHomography(points2, points1, cv::RANSAC);

    // cv:: Mat img2_transformed;
    // cv::warpPerspective(img2, img2_transformed, homography, img1.size());

    // cv:: Mat result;
    // cv::addWeighted(img1, 0.5, img2_transformed, 0.5, 0.0, result);

    cv::imshow("Сшитое изображение", result);
    cv::waitKey(0);

    return 0;
}
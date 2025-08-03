//#include <onnxruntime/core/session/onnxruntime_cxx_api.h>
#include <windows.h>
#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <iostream>
#include <string>
#include "opencv2/opencv.hpp"
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>
#include <chrono>


// int main ()
// {
//     setlocale (0, "Russian");
//     int n = 20;
//     int k = 0;
//     std::string name_image2;
//     std::string name_image3;
//     std::string name_image1;
//     cv:: Mat img2;
//     cv:: Mat img1;
//     std::vector<cv::KeyPoint> kpoints;
//     cv::Mat dpoints;
//     // ushort ** dataImage;
//     // dataImage = new ushort*[20];

//     auto start = std::chrono::high_resolution_clock::now();
//     cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(4, 4));

//     for (int i = 1; i < n; i++) {
//         if(i % 5 == 0){
//             k++;
//             continue;
        
//         }
//         name_image1 = "testing\\wholeImage_" + std::to_string(i+1)+ "_20.tiff";
//         img1 = cv::imread(name_image1, cv:: IMREAD_GRAYSCALE);
        
//         std:: cout << "i: "<< i << std:: endl;
//         std:: cout << "k: "<< k << std:: endl;
//         name_image3 = "stitching_image" + std::to_string(k)+ ".tiff";
//         if(i%5 == 1) {
            
//             name_image2 = "testing\\wholeImage_" + std::to_string(i)+ "_20.tiff";
//             img2 = cv::imread(name_image2, cv:: IMREAD_GRAYSCALE);
//             cv::normalize(img2, img2, 0, 255, cv::NORM_MINMAX);
//             clahe->apply(img2, img2);
           
            
//         }
//         else{
//             img2 = cv::imread(name_image3, cv:: IMREAD_GRAYSCALE);
//         }
        
        
//         cv::normalize(img1, img1, 0, 255, cv::NORM_MINMAX);
//         clahe->apply(img1, img1);
//         img1.convertTo(img1, CV_8U);
//         img2.convertTo(img2, CV_8U);

        

//         if (img1.empty() || img2.empty()){
//             std::cerr << "image read error";
//             return -1;
//         }

//         std::vector<cv::KeyPoint> keypoints1, keypoints2;
//         cv:: Mat descriptors1, descriptors2;
//         std:: cout << "checkpointSIFT1" << std::endl;    

//         cv::Mat mask_roi = cv::Mat::zeros(img2.size(), CV_8U);
//         int rect_x = 0; // Начало прямоугольника по x
//         int rect_y = 0;                        // Начало прямоугольника по y
//         int rect_width = img2.cols; // Ширина прямоугольника
//         int rect_height = img1.rows;   
//         cv::Rect roi(rect_x, rect_y, rect_width, rect_height);                // Прямоугольник (x, y, width, height)
//         cv::rectangle(mask_roi, roi, cv::Scalar(255), cv::FILLED);
//         cv::imwrite("mask1.png", mask_roi);


        
//         cv::Ptr<cv::SIFT> sift= cv::SIFT::create(0, 5);
//         sift->detectAndCompute(img1, cv::Mat(), keypoints1, descriptors1);
//         std:: cout << "SIFT2" << std::endl;
//         sift->detectAndCompute(img2, cv::Mat(), keypoints2, descriptors2);
        
//         cv::FlannBasedMatcher matcher(new cv::flann::KDTreeIndexParams(), new cv::flann::SearchParams());
//         descriptors1.convertTo(descriptors1, CV_32F);
//         descriptors2.convertTo(descriptors2, CV_32F);
//         std::vector<std::vector<cv::DMatch>> knn_matches;
//         matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);

//         std:: vector<cv::DMatch> good_matches;
        

//         const float ratio_thresh = 0.65f;
//         for (const auto& match : knn_matches) {
//             if (match[0].distance < ratio_thresh * match[1].distance) {
//                good_matches.push_back(match[0]);
//             }
//         }
        
//         std::cout << "Number of good matches SIFT1: " << good_matches.size() << std::endl;

//         std:: cout << "checkpoint2" << std::endl;

        

//         if (good_matches.size() < 20) {

//                 std::cout << "SIFT no matches" << std::endl;
//                 cv::Mat res;
//                 cv::Mat croppedImg = img1(cv::Rect(0, 0, img1.cols, img1.rows - 540));

//                 if (img1.empty() || img2.empty()) {
//                     std::cerr << "Error: One or more images could not be loaded!" << std::endl;
//                     return -1;
//                 }

//                 // Проверка размеров и типов
//                 if (img1.cols != img2.cols) {
//                     std::cout << "weight:" << img1.cols << " 1 2 " << img2.cols << std::endl;
//                     std::cerr << "Error: Image widths do not match!" << std::endl;
//                     return -1;
//                 }
//                 if (img1.type() != img2.type()) {
//                     std::cerr << "Error: Image types do not match!" << std::endl;
//                     return -1;
//                 }

//                 cv::vconcat(croppedImg, img2, res);
//                 cv::imwrite(name_image3, res); 
//                 continue;
//         }



//         std:: cout << "checkpoint3" << std::endl;
//         std::vector<cv::Point2f> points1, points2;
//         for (size_t i =0; i < good_matches.size(); i++){
//             points1.push_back(keypoints1[good_matches[i].queryIdx].pt);
//             points2.push_back(keypoints2[good_matches[i].trainIdx].pt);
//         }
    
//         cv::Mat affine_matrix = cv::estimateAffinePartial2D(points2, points1);
//         std::vector<cv::Point2f> img2_corners(4);
//         img2_corners[0] = cv::Point2f(0,0);
//         img2_corners[1] = cv::Point2f(static_cast<float>(img2.cols), 0);
//         img2_corners[2] = cv::Point2f(0, static_cast<float>(img2.rows));
//         img2_corners[3] = cv::Point2f(static_cast<float>(img2.cols), static_cast<float>(img2.rows));
//         std::vector<cv::Point2f> img2_transformed_corners(4);

//         std::cout << "img2_corners size: " << img2_corners.size() << std::endl;
//         std::cout << "affine_matrix size: " << affine_matrix.size() << std::endl;
//         std::cout << "affine_matrix type: " << affine_matrix.type() << std::endl;
//         cv::transform(img2_corners, img2_transformed_corners, affine_matrix);
//         std:: cout << "checkpoint5" << std::endl;

//         float min_x = std::min(0.0f, std::min({img2_transformed_corners[0].x, img2_transformed_corners[1].x, img2_transformed_corners[2].x }));
//         float min_y = std::min(0.0f, std::min({img2_transformed_corners[0].y, img2_transformed_corners[1].y, img2_transformed_corners[2].y}));
//         float max_x = std::max(static_cast<float>(img1.cols), std::max({img2_transformed_corners[0].x, img2_transformed_corners[1].x, img2_transformed_corners[2].x}));
//         float max_y = std::max(static_cast<float>(img1.rows), std::max({img2_transformed_corners[0].y, img2_transformed_corners[1].y, img2_transformed_corners[2].y})); 
//         cv::Size result_size(img1.cols, static_cast<int>(std::ceil(max_y - min_y)));

//         cv::Mat result(result_size, img1.type(), cv::Scalar::all(255));
//         cv::Mat translation_matrix = (cv::Mat_<double>(2, 3) << 1, 0, -min_x, 0 , 1, -min_y);
//         cv::warpAffine(img1, result, translation_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
       
//         cv::Mat img2_transformed;
//         cv::Mat img2_shifted;
//         cv::warpAffine(img2, img2_shifted, translation_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
//         cv::warpAffine(img2_shifted, img2_transformed, affine_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    
//         std:: cout << "checkpoint7" << std::endl;
//         cv::Mat mask = cv::Mat::zeros(result.size(), CV_32F);
//         int blendWidth = 100;
//         cv::Rect blendRect(static_cast<int>(img2_transformed_corners[0].x), 
//                     static_cast<int>(img2_transformed_corners[0].y), 
//                     img2.cols + blendWidth, img2.rows + blendWidth);

//         blendRect.x = std::max(blendRect.x, 0);
//         blendRect.y = std::max(blendRect.y, 0);
//         blendRect.width = std::min(blendRect.width + blendWidth, mask.cols - blendRect.x);
//         blendRect.height = std::min(blendRect.height + blendWidth, mask.rows - blendRect.y);

//         for (int y = blendRect.y; y < blendRect.y + blendRect.height && y < mask.rows; ++y) {
//             for (int x = blendRect.x; x < blendRect.x + blendRect.width && x < mask.cols; ++x) {
//                 float alpha = static_cast<float>(y - blendRect.y) / blendWidth;
//                 alpha = std::min(std::max(alpha, 0.0f), 1.0f); 
                
//                     mask.at<float>(y, x) = alpha;
                
            
//             }
//         }
//         std:: cout << "checkpoint8" << std::endl;
//         result.convertTo(result, CV_32F);
//         img2_transformed.convertTo(img2_transformed, CV_32F);

//         std:: cout << "checkpoint9" << std::endl;

//         cv::Mat blended;
//         blended = result.mul(1.0 - mask) + img2_transformed.mul(mask);
//         blended.convertTo(blended, CV_16F);
//         cv::imwrite(name_image3, blended);
//         blended.convertTo(blended, CV_8U);
      

//         img1.release();
//         img2.release();
//         affine_matrix.release();
//         translation_matrix.release();
//         result.release();
//         img2_shifted.release();
//         img2_transformed.release();
//         mask.release();
//         blended.release();
  
//         points1.clear();
//         points2.clear();
//         descriptors1.release();
//         descriptors2.release();
//         keypoints1.clear();
//         keypoints2.clear();
//         good_matches.clear();
        
//     }






//     //Combining vertical stripes    
//     //Horizontal stitching

//      for (int i = 0; i < 3; i++) {
        
//         if (i == 0){
//             name_image1 = "stitching_image" + std::to_string(i)+ ".tiff";
//         }
//         else {
//             name_image1 = "final_image.tiff";
//         }
//         name_image2 = "stitching_image" + std::to_string(i+1)+ ".tiff";

//         img2 = cv::imread(name_image2, cv:: IMREAD_GRAYSCALE);
        
//         std:: cout << "i: "<< i << std:: endl;

//         img1= cv::imread(name_image1, cv:: IMREAD_GRAYSCALE);
//         img1.convertTo(img1, CV_8U);
//         img2.convertTo(img2, CV_8U);


//         if (img1.empty() || img2.empty()){
//             std::cerr << "?? ??????? ????????? ???????????";
//             return -1;
//         }

//         // cv::cuda::GpuMat img1_gpu, img2_gpu;
//         // img1_gpu.upload(img1);
//         // img2_gpu.upload(img2);
//         // cv::Ptr<cv::cuda::SIFT> sift_gpu = cv::cuda::SIFT::create();
//         // cv::cuda::GpuMat descriptors1_gpu, descriptors2_gpu;
//         // std::vector<cv::KeyPoint> keypoints1, keypoints2;
//         // sift_gpu->detectAndComputeAsync(img1_gpu, cv::noArray(), keypoints1, descriptors1_gpu);
//         // sift_gpu->detectAndComputeAsync(img2_gpu, cv::noArray(), keypoints2, descriptors2_gpu);
//         //
//         // cv::Mat descriptors1, descriptors2;
//         // descriptors1_gpu.download(descriptors1);
//         // descriptors2_gpu.download(descriptors2);
//         // for (const auto& kp : keypoints1) {
//         //     keypoints1.push_back(kp);
//         // }
//         // for (const auto& kp : keypoints2) {
//         //     keypoints2.push_back(kp);
//         // }

//         // // Подготавливаем дескрипторы для FLANN
//         // descriptors1.convertTo(descriptors1, CV_32F);
//         // descriptors2.convertTo(descriptors2, CV_32F);

//         // // Сопоставляем дескрипторы на CPU
//         // cv::FlannBasedMatcher matcher(new cv::flann::KDTreeIndexParams(), new cv::flann::SearchParams());
//         // std::vector<std::vector<cv::DMatch>> knn_matches;
//         // matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);

//         // // Применяем фильтр Lowe
//         // const float ratio_thresh = 0.75f;
//         // std::vector<cv::DMatch> good_matches;
//         // for (const auto& match : knn_matches) {
//         //     if (match[0].distance < ratio_thresh * match[1].distance) {
//         //         good_matches.push_back(match[0]);
//         //     }
//         // }    
//         std::vector<cv::KeyPoint> keypoints1, keypoints2;
//         cv:: Mat descriptors1, descriptors2;
//         std:: cout << "checkpointSIFT1" << std::endl;    

//         cv::Mat mask_roi = cv::Mat::zeros(img1.size(), CV_8U);
//         int rect_x = std::max(0, img1.cols - img2.cols); // Начало прямоугольника по x
//         int rect_y = 0;                        // Начало прямоугольника по y
//         int rect_width = std::min(img2.cols, img1.cols); // Ширина прямоугольника
//         int rect_height = img1.rows;   
//         cv::Rect roi(rect_x, rect_y, rect_width, rect_height);                // Прямоугольник (x, y, width, height)
//         cv::rectangle(mask_roi, roi, cv::Scalar(255), cv::FILLED);
//         cv::imwrite("mask.png", mask_roi);
//         cv::Ptr<cv::SIFT> sift = cv::SIFT::create();

//         sift->detectAndCompute(img1, mask_roi, keypoints1, descriptors1);
//         std:: cout << "SIFT2" << std::endl;
//         sift->detectAndCompute(img2, cv::Mat(), keypoints2, descriptors2); 
       
        

//         std:: cout << "checkpointSIFT2" << std::endl;
    
//         std::cout << "Тип дескрипторов: " << descriptors1.type() << std::endl;
//         std::cout << "Тип дескрипторов: " << descriptors2.type() << std::endl;
        
//         cv::FlannBasedMatcher matcher(new cv::flann::KDTreeIndexParams(), new cv::flann::SearchParams());
//         descriptors1.convertTo(descriptors1, CV_32F);
//         descriptors2.convertTo(descriptors2, CV_32F);
//         std::vector<std::vector<cv::DMatch>> knn_matches;
//         matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);


//         std:: cout << "checkpoint2" << std::endl;

//         const float ratio_thresh = 0.65f;
//         std:: vector<cv::DMatch> good_matches;
        
//         for (const auto& match : knn_matches) {
//             if (match[0].distance < ratio_thresh * match[1].distance) {
//                 good_matches.push_back(match[0]);
//             }
//         }

//         std::cout << "Number of good matches SIFT1: " << good_matches.size() << std::endl;

//         if (good_matches.size() < 10) {

//             std::cout << "SIFT no matches" << std::endl;
//                 cv::Mat res;
//                 cv::Mat croppedImg = img1(cv::Rect(0, 0, img1.cols-580, img1.rows));

//                 if (img1.empty() || img2.empty()) {
//                     std::cerr << "Error: One or more images could not be loaded!" << std::endl;
//                     return -1;
//                 }

//                 // Проверка размеров и типов
//                 if (img1.rows != img2.rows) {
//                     std::cout << "weight:" << img1.rows << " 1 2 " << img2.rows << std::endl;
//                     std::cerr << "Error: Image widths do not match!" << std::endl;
//                     return -1;
//                 }
//                 if (img1.type() != img2.type()) {
//                     std::cerr << "Error: Image types do not match!" << std::endl;
//                     return -1;
//                 }

//                 cv::hconcat(croppedImg, img2, res);
//                 cv::imwrite("final_image.tiff", res); 
//                 continue;
//         }



        


//         std::vector<cv::Point2f> points1, points2;

//         for (size_t i =0; i < good_matches.size(); i++){

//             if (good_matches[i].queryIdx >= keypoints1.size() || good_matches[i].trainIdx >= keypoints2.size()) {
//             std::cerr << "Error: Index out of range at match " << i << std::endl;
//             }

//             points1.push_back(keypoints1[good_matches[i].queryIdx].pt);
//             points2.push_back(keypoints2[good_matches[i].trainIdx].pt);
//         }
    
//         cv::Mat affine_matrix = cv::estimateAffinePartial2D(points2, points1);

//         std::vector<cv::Point2f> img2_corners(4);

//         img2_corners[0] = cv::Point2f(0,0);
//         img2_corners[1] = cv::Point2f(static_cast<float>(img2.cols), 0);
//         img2_corners[2] = cv::Point2f(0, static_cast<float>(img2.rows));
//         img2_corners[3] = cv::Point2f(static_cast<float>(img2.cols), static_cast<float>(img2.rows));
//         std::vector<cv::Point2f> img2_transformed_corners(4);

//         std::cout << "img2_corners size: " << img2_corners.size() << std::endl;
//         std::cout << "affine_matrix size: " << affine_matrix.size() << std::endl;
//         std::cout << "affine_matrix type: " << affine_matrix.type() << std::endl;
//         cv::transform(img2_corners, img2_transformed_corners, affine_matrix);


//         std:: cout << "checkpoint5" << std::endl;

//         float min_x = std::min(0.0f, std::min({img2_transformed_corners[0].x, img2_transformed_corners[1].x, img2_transformed_corners[2].x }));
//         float min_y = std::min(0.0f, std::min({img2_transformed_corners[0].y, img2_transformed_corners[1].y, img2_transformed_corners[2].y}));
//         float max_x = std::max(static_cast<float>(img1.cols), std::max({img2_transformed_corners[0].x, img2_transformed_corners[1].x, img2_transformed_corners[2].x}));
//         float max_y = std::max(static_cast<float>(img1.rows), std::max({img2_transformed_corners[0].y, img2_transformed_corners[1].y, img2_transformed_corners[2].y}));
         
         
//         cv::Size result_size(static_cast<int>(std::ceil(max_x - min_x)), img1.rows);
//         cv::Mat result(result_size, img1.type(), cv::Scalar::all(255));
//         cv:: Mat translation_matrix = (cv::Mat_<double>(2, 3) << 1, 0, -min_x, 0 , 1, -min_y);
//         cv::warpAffine(img1, result, translation_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
        
//         std:: cout << "checkpoint6" << std::endl;
//         cv::Mat img2_shifted;
//         cv::warpAffine(img2, img2_shifted, translation_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);

//         cv::Mat img2_transformed;
//         cv::warpAffine(img2_shifted, img2_transformed, affine_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    
//         std:: cout << "checkpoint7" << std::endl;
//         cv::Mat mask = cv::Mat::zeros(result.size(), CV_32F);
//         int blendWidth = 400;
        
//         cv::Rect blendRect(static_cast<int>(img2_transformed_corners[0].x), 
//                     static_cast<int>(img2_transformed_corners[0].y), 
//                     img2.cols + blendWidth, img2.rows);

//         blendRect.x = std::max(blendRect.x, 0);
//         blendRect.y = std::max(blendRect.y, 0);
//         blendRect.width = std::min(blendRect.width, mask.cols - blendRect.x);
//         blendRect.height = std::min(blendRect.height, mask.rows - blendRect.y);

//         for (int y = blendRect.y; y < blendRect.y + blendRect.height && y < mask.rows; ++y) {
//             for (int x = blendRect.x; x < blendRect.x + blendRect.width && x < mask.cols; ++x) {
//                 float alpha = static_cast<float>(x - blendRect.x) / blendWidth;
//                 alpha = std::min(std::max(alpha, 0.0f), 1.0f); // Îãðàíè÷èâàåì çíà÷åíèå â äèàïàçîíå [0, 1]
                
//                     mask.at<float>(y, x) = alpha;
                
            
//             }
//         }
   
//         std:: cout << "checkpoint8" << std::endl;

//         result.convertTo(result, CV_32F);
//         img2_transformed.convertTo(img2_transformed, CV_32F);

//         std:: cout << "checkpoint9" << std::endl;
    

//         cv::Mat blended;

//         blended = result.mul(1.0 - mask) + img2_transformed.mul(mask);
        
//         blended.convertTo(blended, CV_16F);

    
        
//         cv::imwrite("final_image.tiff", blended);
        


//         img1.release();
//         img2.release();
        
//         affine_matrix.release();
//         translation_matrix.release();
//         result.release();
//         img2_shifted.release();
//         img2_transformed.release();
//         mask.release();
//         blended.release();
    

      
//         points1.clear();
//         points2.clear();

//         descriptors1.release();
//         descriptors2.release();
//         keypoints1.clear();
//         keypoints2.clear();
//         good_matches.clear();
//         knn_matches.clear();
//     }


//     auto end = std::chrono::high_resolution_clock::now();
//     std::chrono::duration<double> elapsed = end - start;
//     std::cout << "times: " << elapsed.count() << " s." << std::endl;
//     return 0;
// }

// #include <iostream>
// #include <string>
// //#include "opencv2/opencv.hpp"
// #include <opencv2/features2d.hpp>
// #include <opencv2/highgui.hpp>
// #include <opencv2/imgproc.hpp>
// #include <opencv2/calib3d.hpp>
// #include <chrono>

int main ()
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);  
    //setlocale (0, "Russian");
    int n = 20;
    int k = 0;
    double minVal, maxVal;
    std::string name_image2;
    std::string name_image3;
    std::string name_image1;
    cv:: Mat img2;
    cv:: Mat img1;
    std::vector<cv::Mat> intermediate_results;
    std::vector<cv::Mat> quarter_results;

    auto start = std::chrono::high_resolution_clock::now();
    std::cout << "Vertical stitching:" << std::endl;
    for (int i = 1; i < n; i++) {
        if(i%5 == 0){
            k++;
            continue;
        }
        name_image1 = "testing1\\wholeImage_" + std::to_string(i+1)+ "_20.tiff";
        img1 = cv::imread(name_image1, cv:: IMREAD_GRAYSCALE | cv:: IMREAD_ANYDEPTH);
        
        std:: cout << "i: "<< i << std:: endl;
        std:: cout << "k: "<< k << std:: endl;
        name_image3 = "stitching_image" + std::to_string(k)+ ".tiff";
        if(i%5 == 1) {
            
            name_image2 = "testing1\\wholeImage_" + std::to_string(i)+ "_20.tiff";
            img2 = cv::imread(name_image2, cv:: IMREAD_GRAYSCALE | cv:: IMREAD_ANYDEPTH);
           
            
        }
        else{
            //img2 = cv::imread(name_image3, cv:: IMREAD_GRAYSCALE | cv:: IMREAD_ANYDEPTH);
            img2 = intermediate_results.back();
        }

        if (img1.empty() || img2.empty()) {
            std::cout << "Error image -> empty" << std::endl;
            return -1;
        }
        std::cout << "Тип изображения: " << img1.type() << std::endl;
        int overlap_height = 670;

        // Create a new image for the result
        cv::Mat result(img1.rows + img2.rows - overlap_height, img1.cols, img1.type(), cv::Scalar(0));

        // Copy first image in two
        img1.copyTo(result(cv::Rect(0, 0, img1.cols, img1.rows)));

        // Copy two image in result, starting from the overlap area
        img2.copyTo(result(cv::Rect(0, img1.rows - overlap_height, img2.cols, img2.rows)));

        for (int y = img1.rows - overlap_height; y < img1.rows; y++) {
            for (int x = 0; x < result.cols; x++) {
                // Mixing weight (linear change from 0 to 1)
                double alpha = static_cast<double>(y - (img1.rows - overlap_height)) / overlap_height;

                // Mixing pixels
                result.at<ushort>(y, x) = static_cast<ushort>(alpha * img2.at<ushort>(y - (img1.rows - overlap_height), x) + (1 - alpha) * img1.at<ushort>(y, x));
            }
        }

        // Saving the result
        
        intermediate_results.push_back(result);
        if (i%5 == 4){
            quarter_results.push_back(result);
            minMaxLoc(result, &minVal, &maxVal);
            result.convertTo(result, CV_16UC1, 65535.0/(maxVal - minVal), -minVal * 65535.0/(maxVal - minVal));
            cv::imwrite(name_image3, result);
        }
        std::cout << "Тип изображения: " << result.type() << std::endl;
    }

    img1.release();
    img2.release();
    intermediate_results.clear();
    //cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE();
    
    std::cout << "Horizontal stitching" << std::endl;
    for(int i = 0; i < 3; i++) {
        std::cout << "k: " << i << std:: endl;
        //name_image2 = "stitching_image" + std::to_string(i+1)+ ".tiff";
        if (i == 0) {
            img1 = quarter_results[0]; //cv::imread("stitching_image0.tiff", cv::IMREAD_GRAYSCALE | cv:: IMREAD_ANYDEPTH);
            // clahe -> apply(img1, img1);
             
            minMaxLoc(img1, &minVal, &maxVal);
            img1.convertTo(img1, CV_16UC1, 65535.0/(maxVal - minVal), -minVal * 65535.0/(maxVal - minVal));
            
        }
        else {
            img1 = intermediate_results.back(); //cv::imread("result_stitching.tiff", cv::IMREAD_GRAYSCALE | cv:: IMREAD_ANYDEPTH);
            // clahe -> apply(img1, img1);
            minMaxLoc(img1, &minVal, &maxVal);
            img1.convertTo(img1, CV_16UC1, 65535.0/(maxVal - minVal), -minVal * 65535.0/(maxVal - minVal));
            
        }  
        
        cv::Mat img2 = quarter_results[i+1]; //cv::imread(name_image2, cv::IMREAD_GRAYSCALE | cv:: IMREAD_ANYDEPTH);
        minMaxLoc(img2, &minVal, &maxVal);
        img2.convertTo(img2, CV_16UC1, 65535.0/(maxVal - minVal), -minVal * 65535.0/(maxVal - minVal));
        //clahe -> apply(img2, img2);

        if (img1.empty() || img2.empty()) {
           std::cout << "Ошибка загрузки изображений!" << std::endl;
            return -1;
        }

        // Overlapping area
        int overlap_width = 750;

        // Create a new image for the result
        cv::Mat result(img1.rows, img1.cols + img2.cols - overlap_width, img1.type(), cv::Scalar(0));

        //Copy the first image to the left side of the result
        img1.copyTo(result(cv::Rect(0, 0, img1.cols, img1.rows)));

        // Copy the second image to the right side of the result, starting from the overlap area
        img2.copyTo(result(cv::Rect(img1.cols - overlap_width, 0, img2.cols, img2.rows)));

        // Blending in the overlap area
        for (int y = 0; y < result.rows; y++) {
            for (int x = img1.cols - overlap_width; x < img1.cols; x++) {
                // Mixing weight (linear change from 0 to 1)
                double alpha = static_cast<double>(x - (img1.cols - overlap_width)) / overlap_width;

                // Pixel blending
                result.at<ushort>(y, x) = static_cast<ushort>(
                    alpha * img2.at<ushort>(y, x - (img1.cols - overlap_width)) +
                    (1 - alpha) * img1.at<ushort>(y, x)
                );
            }
        }

        // Saving the result
        intermediate_results.push_back(result);
        
    }
    cv::imwrite("result_stitching.tiff", intermediate_results[2]);

    intermediate_results.clear();
    intermediate_results.shrink_to_fit();
    quarter_results.clear();
    quarter_results.shrink_to_fit();

    img1.release();
    img2.release();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "times: " << elapsed.count() << " s." << std::endl;
    return 0;
}
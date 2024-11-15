// #include <iostream>
// #include <string>
// #include "opencv2/opencv.hpp"
// #include <opencv2/features2d.hpp>
// #include <opencv2/highgui.hpp>
// #include <opencv2/imgproc.hpp>
// #include <opencv2/calib3d.hpp>



// int main ()
// {
//     setlocale (0, "Russian");
//     int n = 20;
//     int k = 0;
//     std::string name_image2;
//     std::string name_image3;
//     cv:: Mat img2;
//     cv:: Mat img1;
    
    
//     //std::string name_image3 = "stitching_image" + std::to_string(0)+ ".tiff";
//     //std::string write_name;

//     for (int i = 1; i < n; i++) {
//         if(i%5 == 0){
//             continue;
//         }
//         std::string name_image1 = "testing\\wholeImage_" + std::to_string(i+1)+ "_20.tiff";
//         img1 = cv::imread(name_image1, cv:: IMREAD_GRAYSCALE);
        
//         std:: cout << "i: "<< i << std:: endl;
//         std:: cout << "k: "<< k << std:: endl;
//         name_image3 = "stitching_image" + std::to_string(k)+ ".tiff";
//         if(i%5 == 1) {
            
//             name_image2 = "testing\\wholeImage_" + std::to_string(i)+ "_20.tiff";
//             img2 = cv::imread(name_image2, cv:: IMREAD_GRAYSCALE);
            
//         }
//         else{
//             img2 = cv::imread(name_image3, cv:: IMREAD_GRAYSCALE);
//         }
            
        
//         //std:: cout << "Сшивка изображения" << std::to_string(i+1) << std:: endl;

//         img1.convertTo(img1, CV_8U);
//         img2.convertTo(img2, CV_8U);


//         // cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
//         // clahe->apply(img1, img1);
//         // clahe->apply(img2, img2);


//         if (img1.empty() || img2.empty()){
//             std::cerr << "?? ??????? ????????? ???????????";
//             return -1;
//         }

        
//         std::vector<cv::KeyPoint> keypoints1, keypoints2;
//         cv:: Mat descriptors1, descriptors2;
       
//         cv::Ptr<cv::AKAZE> akaze = cv::AKAZE::create();//cv::AKAZE::DESCRIPTOR_KAZE, 3, 0.001f, 4);

//         std:: cout << "checkpointSIFT1" << std::endl;
//         akaze->detectAndCompute(img1, cv::Mat(), keypoints1, descriptors1);

//         std:: cout << "checkpointSIFT2" << std::endl;
//         akaze->detectAndCompute(img2, cv::Mat(), keypoints2, descriptors2);
        
        
//         //cv::BFMatcher matcher(cv::NORM_HAMMING, true); 
//         cv::FlannBasedMatcher matcher(new cv::flann::KDTreeIndexParams(), new cv::flann::SearchParams());
//         descriptors1.convertTo(descriptors1, CV_32F);
//         descriptors2.convertTo(descriptors2, CV_32F);
//         std::vector<std::vector<cv::DMatch>> knn_matches;
//         matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);


//         std:: cout << "checkpoint2" << std::endl;

//         const float ratio_thresh = 0.75f;
//         std:: vector<cv::DMatch> good_matches;
        
//         for (const auto& match : knn_matches) {
//             if (match[0].distance < ratio_thresh * match[1].distance) {
//                 good_matches.push_back(match[0]);
//             }
//         }

//         if (good_matches.size() < 4) {

//             descriptors1.release();
//             descriptors2.release();
//             keypoints1.clear();
//             keypoints2.clear();
//             good_matches.clear();
//             knn_matches.clear();

//             std:: cout << "??? 4, ?????? SIFT1" << std::endl;

//             cv::Ptr<cv::SIFT> sift = cv::SIFT::create();

//             sift->detectAndCompute(img1, cv::Mat(), keypoints1, descriptors1);

//             std:: cout << "SIFT2" << std::endl;
//             sift->detectAndCompute(img2, cv::Mat(), keypoints2, descriptors2);
            
            
//             //cv::BFMatcher matcher(cv::NORM_HAMMING, true); 
//             cv::FlannBasedMatcher matcher(new cv::flann::KDTreeIndexParams(), new cv::flann::SearchParams());
//             descriptors1.convertTo(descriptors1, CV_32F);
//             descriptors2.convertTo(descriptors2, CV_32F);
//             matcher.knnMatch(descriptors1, descriptors2, knn_matches, 2);


//             const float ratio_thresh = 0.75f;
//             std:: vector<cv::DMatch> good_matches;
            
//             for (const auto& match : knn_matches) {
//                 if (match[0].distance < ratio_thresh * match[1].distance) {
//                     good_matches.push_back(match[0]);
//                 }
//             }
//             if (good_matches.size() < 4){
//                 std::cout << "SIFT не проканал" << std::endl;
//                 continue;
//             }
//         }



//         std:: cout << "checkpoint3" << std::endl;


//         std::vector<cv::Point2f> points1, points2;

//         for (size_t i =0; i < good_matches.size(); i++){

//             points1.push_back(keypoints1[good_matches[i].queryIdx].pt);
//             points2.push_back(keypoints2[good_matches[i].trainIdx].pt);
//         }


//         std:: cout << "checkpoint4" << std::endl;

//     //Метод афинного преобразования
//         cv::Mat affine_matrix = cv::estimateAffinePartial2D(points2, points1);

//         std::vector<cv::Point2f> img2_corners(3);

//         img2_corners[0] = cv::Point2f(0,0);
//         img2_corners[1] = cv::Point2f(static_cast<float>(img2.cols), 0);
//         img2_corners[2] = cv::Point2f(0, static_cast<float>(img2.rows));
//         std::vector<cv::Point2f> img2_transformed_corners(3);

//         std::cout << "img2_corners size: " << img2_corners.size() << std::endl;
//         std::cout << "affine_matrix size: " << affine_matrix.size() << std::endl;
//         std::cout << "affine_matrix type: " << affine_matrix.type() << std::endl;
//         cv::transform(img2_corners, img2_transformed_corners, affine_matrix);


//         std:: cout << "checkpoint5" << std::endl;

//         float min_x = std::min(0.0f, std::min({img2_transformed_corners[0].x, img2_transformed_corners[1].x, img2_transformed_corners[2].x }));
//         float min_y = std::min(0.0f, std::min({img2_transformed_corners[0].y, img2_transformed_corners[1].y, img2_transformed_corners[2].y}));
//         float max_x = std::max(static_cast<float>(img1.cols), std::max({img2_transformed_corners[0].x, img2_transformed_corners[1].x, img2_transformed_corners[2].x}));
//         float max_y = std::max(static_cast<float>(img1.rows), std::max({img2_transformed_corners[0].y, img2_transformed_corners[1].y, img2_transformed_corners[2].y}));
        
//         cv::Size result_size(static_cast<int>(std::ceil(max_x - min_x)), static_cast<int>(std::ceil(max_y - min_y)));

//         cv::Mat result(result_size, img1.type(), cv::Scalar::all(255));

//         cv:: Mat translation_matrix = (cv::Mat_<double>(2, 3) << 1, 0, -min_x, 0 , 1, -min_y);
        
//         cv::warpAffine(img1, result, translation_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
        
        
//         std:: cout << "checkpoint6" << std::endl;
        
//         // cv::imshow("img1", img1);
//         // cv::imshow("img2", img2);
//         //cv::drawKeypoints(img1, keypoints1, result);
//         //cv::drawKeypoints(img2, keypoints2, result);
        
//         //cv::imshow("result", result);
//         //cv::waitKey(0);

//         cv::Mat img2_transformed;

//         cv::Mat img2_shifted;
//         cv::warpAffine(img2, img2_shifted, translation_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);

//         // Теперь применяем affine_matrix после сдвига
//         cv::warpAffine(img2_shifted, img2_transformed, affine_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    
//         std:: cout << "checkpoint7" << std::endl;
//         cv::Mat mask = cv::Mat::zeros(result.size(), CV_32F);
//         int blendWidth = 100;
        
//         // вариант1
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
//                 alpha = std::min(std::max(alpha, 0.0f), 1.0f); // Ограничиваем значение в диапазоне [0, 1]
                
//                     mask.at<float>(y, x) = alpha;
                
            
//             }
//         }
//         // ввариант1
//         std:: cout << "checkpoint8" << std::endl;




//        // cv::imwrite("Mask2.tiff", mask);
    
//     // Преобразуем изображения в 32-битный формат, чтобы выполнить смешивание
//         result.convertTo(result, CV_32F);
//         img2_transformed.convertTo(img2_transformed, CV_32F);

//         std:: cout << "checkpoint9" << std::endl;
//     // Плавное смешивание изображений в области перекрытия

//         cv::Mat blended;

//         // std:: cout << mask << std:: endl;

//         blended = result.mul(1.0 - mask) + img2_transformed.mul(mask);
        
        
        
//     // Приводим результат обратно в 8-битный формат для отображения
//         blended.convertTo(blended, CV_16F);

//     // Отображаем сшитое изображение
//         // if(i%6 == 5 || i == 1) {
//         //     write_name = "stitching_image"+ std::to_string((i+1)/6) + ".tiff";

//         // }
        
//         cv::imwrite(name_image3, blended);
//         blended.convertTo(blended, CV_8U);
//         //cv::imshow("Сшитое изображение", blended);
//         //cv::waitKey(0);
//         if( (i+2)%5 == 0) k = (i+2)/5;


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
//     return 0;
//}

// version 2.0

#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/features2d.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/calib3d.hpp>



int main ()
{
    setlocale (0, "Russian");
    cv:: Mat img2;
    std::string name_image;
    for (int i = 2; i <=5; i++) {

        std::string name_image = "wholeImage_" + std::to_string(i) + "_20.tiff";
        cv:: Mat img1 = cv::imread(name_image, cv:: IMREAD_GRAYSCALE);

        if  (i == 2) {
            img2 = cv::imread("wholeImage_1_20.tiff", cv:: IMREAD_GRAYSCALE);
        } else  img2 = cv::imread("img_stitch1.tiff", cv:: IMREAD_GRAYSCALE);

        img1.convertTo(img1, CV_8U);
        img2.convertTo(img2, CV_8U);

        if (img1.empty() || img2.empty()){
            std::cerr << "?? ??????? ????????? ???????????";
            return -1;
        }

        cv::Ptr<cv::CLAHE> clahe = cv::createCLAHE(2.0, cv::Size(8, 8));
        clahe->apply(img1, img1);
        clahe->apply(img2, img2);

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
        if (good_matches.size() < 4) {

            good_matches.clear();  // Очищаем совпадения
            keypoints1.clear();
            keypoints2.clear();

            cv::Ptr<cv::AKAZE> akaze = cv::AKAZE::create();
            akaze->detectAndCompute(img1, cv::Mat(), keypoints1, descriptors1);
            akaze->detectAndCompute(img2, cv::Mat(), keypoints2, descriptors2);

            cv::BFMatcher bfMatcher(cv::NORM_HAMMING, true);  // true для поиска лучших совпадений          
            bfMatcher.match(descriptors1, descriptors2, good_matches);
        
        // Проверяем снова количество совпадений
            if (good_matches.size() < 7) {
                std::cerr << "Недостаточно совпадений для ORB." << std::endl;
                return -1;
            }


        }

        std::vector<cv::Point2f> points1, points2;

        for (size_t i =0; i < good_matches.size(); i++){

            points1.push_back(keypoints1[good_matches[i].queryIdx].pt);
            points2.push_back(keypoints2[good_matches[i].trainIdx].pt);
        }


    
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
        
    
        cv::Mat img2_transformed;

        cv::Mat img2_shifted;
        cv::warpAffine(img2, img2_shifted, translation_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);


        cv::warpAffine(img2_shifted, img2_transformed, affine_matrix, result_size, cv::INTER_LINEAR, cv::BORDER_REPLICATE);
    

    
        
        cv::Mat mask = cv::Mat::zeros(result.size(), CV_32F);

        int blendWidth = 50; 
        
        cv::Rect blendRect(static_cast<int>(img2_transformed_corners[0].x), 
                    static_cast<int>(img2_transformed_corners[0].y), 
                    img2.cols + blendWidth, img2.rows + blendWidth);
        

        
        for (int y = blendRect.y; y < blendRect.y + blendRect.height && y < mask.rows; ++y) {
            for (int x = blendRect.x; x < blendRect.x + blendRect.width && x < mask.cols; ++x) {
                float alpha = static_cast<float>(x - blendRect.x) / blendWidth;
                alpha = std::min(std::max(alpha, 0.0f), 1.0f); 
                mask.at<float>(y, x) = alpha;
            }
        }
        


        result.convertTo(result, CV_32F);
        img2_transformed.convertTo(img2_transformed, CV_32F);


        cv::Mat blended;
        blended = result.mul(1.0 - mask) + img2_transformed.mul(mask);
    
        

        blended.convertTo(blended, CV_16F);


        cv::imwrite("img_stitch1.tiff", blended);
    
    }

    return 0;
}
#include <opencv2/opencv.hpp>

int main() {
  cv::Mat image1 = cv::imread("Empty_event_C.jpg");         // Noise-only image
  cv::Mat image2 = cv::imread("Muon_with_shower_C.jpg");    // Noise+shower image

  if (image1.empty() || image2.empty()) {
    std::cout << "Can not read the image files!" << std::endl;
    return -1;
  }

  cv::resize(image1, image1, cv::Size(image2.cols, image2.rows));

  cv::Mat diff, gray_diff;            // diff is to store the difference of image1 and image2. gray_diff is gray scaled version of diff.
  cv::absdiff(image1, image2, diff);  // subtract image1 from image2. result stored in diff.
  cv::cvtColor(diff, gray_diff, cv::COLOR_BGR2GRAY);  // convert diff to grayscale.

  // morphology operation to remove remained noise
  cv::Mat thresholded;
  cv::threshold(gray_diff, thresholded, 30, 255, cv::THRESH_BINARY);
  cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));
  cv::morphologyEx(thresholded, thresholded, cv::MORPH_OPEN, kernel);

  // edge detection to detect line elements (shower)
  cv::Mat edges;
  cv::Canny(thresholded, edges, 50, 200);
  std::vector<cv::Vec4i> lines;
  cv::HoughLinesP(edges, lines, 1, CV_PI / 180, 10, 10, 25);

  // draw lines on the image
  for (const auto &line : lines) {
    cv::line(image2, cv::Point(line[0], line[1]), cv::Point(line[2], line[3]), cv::Scalar(0, 0, 255), 2);
  }

  // save images
  cv::imwrite("imgcomp_img1.jpg", image1);
  cv::imwrite("imgcomp_img2.jpg", image2);
  cv::imwrite("imgcomp_diff.jpg", diff);
  cv::imwrite("imgcomp_thresholded.jpg", thresholded);

  /*  draw images
  cv::imshow("Image 1", image1);
  cv::imshow("Image 2", image2);
  cv::imshow("Difference", diff);
  cv::imshow("Thresholded", thresholded);

  cv::waitKey(0);
  cv::destroyAllWindows();
  */

  return 0;
}


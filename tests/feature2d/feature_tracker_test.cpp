#include "gvio/munit.hpp"
#include "gvio/feature2d/feature_tracker.hpp"

namespace gvio {

static const std::string TEST_IMAGE_TOP = "test_data/apriltag/top.png";
static const std::string TEST_IMAGE_BOTTOM = "test_data/apriltag/bottom.png";

int test_FeatureTracker_constructor() {
  FeatureTracker tracker;

  MU_CHECK(tracker.show_matches == false);

  MU_CHECK_EQ(-1, (int) tracker.counter_frame_id);

  return 0;
}

// int test_FeatureTracker_detect() {
//   FeatureTracker tracker;
//
//   cv::VideoCapture capture(0);
//   cv::Mat frame;
//
//   double time_prev = time_now();
//   int frame_counter = 0;
//
//   std::vector<Feature> features;
//
//   while (cv::waitKey(1) != 113) {
//     capture >> frame;
//
//     tracker.detect(frame, features);
//     cv::imshow("Image", frame);
//
//     frame_counter++;
//     if (frame_counter % 10 == 0) {
//       std::cout << 10.0 / (time_now() - time_prev) << std::endl;
//       time_prev = time_now();
//       frame_counter = 0;
//     }
//   }
//
//   return 0;
// }

int test_FeatureTracker_conversions() {
  FeatureTracker tracker;
  const cv::Mat img = cv::imread(TEST_IMAGE_TOP, CV_LOAD_IMAGE_COLOR);

  // Detect keypoints and descriptors
  cv::Mat mask;
  std::vector<cv::KeyPoint> k0;
  cv::Mat d0;
  cv::Ptr<cv::ORB> orb = cv::ORB::create();
  orb->detectAndCompute(img, mask, k0, d0);

  // Convert keypoints and descriptors to features
  std::vector<Feature> f0;
  tracker.getFeatures(k0, d0, f0);

  // Convert features to keypoints and descriptors
  std::vector<cv::KeyPoint> k1;
  cv::Mat d1;
  tracker.getKeyPointsAndDescriptors(f0, k1, d1);

  // Assert
  int index = 0;
  for (auto &f : f0) {
    MU_CHECK(f.kp.pt.x == k0[index].pt.x);
    MU_CHECK(f.kp.pt.y == k0[index].pt.y);

    MU_CHECK(f.kp.pt.x == k1[index].pt.x);
    MU_CHECK(f.kp.pt.y == k1[index].pt.y);

    MU_CHECK(is_equal(f.desc, d0.row(index)));
    MU_CHECK(is_equal(f.desc, d1.row(index)));

    MU_CHECK_EQ(f.desc.size(), cv::Size(32, 1));
    index++;
  }

  return 0;
}

int test_FeatureTracker_match() {
  FeatureTracker tracker;
  const cv::Mat img0 = cv::imread(TEST_IMAGE_TOP, CV_LOAD_IMAGE_COLOR);
  const cv::Mat img1 = cv::imread(TEST_IMAGE_BOTTOM, CV_LOAD_IMAGE_COLOR);
  // cv::imshow("Image0", img0);
  // cv::imshow("Image1", img1);
  // cv::waitKey();

  // Detect keypoints and descriptors
  cv::Mat mask;
  cv::Ptr<cv::ORB> orb = cv::ORB::create();

  // Detect features in image 0
  std::vector<cv::KeyPoint> k0;
  cv::Mat d0;
  std::vector<Feature> f0;
  orb->detectAndCompute(img0, mask, k0, d0);
  tracker.getFeatures(k0, d0, f0);
  tracker.fea_ref = f0;

  // Detect features in image 1
  std::vector<cv::KeyPoint> k1;
  cv::Mat d1;
  std::vector<Feature> f1;
  orb->detectAndCompute(img1, mask, k1, d1);
  tracker.getFeatures(k1, d1, f1);

  // Perform matching
  std::vector<cv::DMatch> matches;
  tracker.img_size = img0.size();
  tracker.match(f1, matches);

  // Draw inliers
  cv::Mat matches_img = draw_inliers(img0, img1, k0, k1, matches, 1);
  // cv::imshow("Matches", matches_img);
  // cv::waitKey();

  // Asserts
  MU_CHECK(matches.size() > 0);

  return 0;
}

void test_suite() {
  MU_ADD_TEST(test_FeatureTracker_constructor);
  // MU_ADD_TEST(test_FeatureTracker_detect);
  MU_ADD_TEST(test_FeatureTracker_conversions);
  MU_ADD_TEST(test_FeatureTracker_match);
}

} // namespace gvio

MU_RUN_TESTS(gvio::test_suite);

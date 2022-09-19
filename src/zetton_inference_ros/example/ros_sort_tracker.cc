#include <cv_bridge/cv_bridge.h>
#include <image_transport/image_transport.h>
#include <image_transport/publisher.h>
#include <ros/package.h>
#include <ros/ros.h>
#include <ros/time.h>
#include <sensor_msgs/image_encodings.h>

#include <csignal>
#include <opencv2/opencv.hpp>
#include <string>

#include "zetton_common_ros/util/ros_util.h"
#include "zetton_inference/detector/yolo_object_detector.h"
#include "zetton_inference/tracker/sort_tracker.h"
#include "zetton_inference/util/viz_util.h"

void signalHandler(int sig) {
  AWARN_F("Trying to exit!");
  ros::shutdown();
}

class RosMotTracker {
 private:
  inline void RosImageCallback(const sensor_msgs::ImageConstPtr& msg) {
    // convert image msg to cv::Mat
    cv_bridge::CvImagePtr cv_ptr;
    try {
      cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    } catch (cv_bridge::Exception& e) {
      AERROR_F("cv_bridge exception: {}", e.what());
      return;
    }

    // do detection
    std::vector<zetton::inference::ObjectPtr> detections;
    detector_.Detect(cv_ptr->image, detections);

    // do tracking
    tracker_.Track(cv_ptr->image, ros::Time::now().toSec(), detections);

    // print detections and tracks
    AINFO_F("Detections:");
    for (auto& detection : detections) {
      AINFO << detection;
      zetton::inference::DrawBoundingBoxOnCvImage(cv_ptr->image, detection);
    }
    AINFO_F("Trackings:");
    for (auto& track : tracker_.tracks()) {
      // if (track.tracking_fail_count <= 3) {
      AINFO << track;
      track.Draw(cv_ptr->image);
      // }
    }

    // publish results
    image_pub_.publish(
        cv_bridge::CvImage(std_msgs::Header(), "bgr8", cv_ptr->image)
            .toImageMsg());
    AINFO_F("---");
  }

  ros::NodeHandle* nh_;

  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

  zetton::inference::YoloObjectDetector detector_;
  // zetton::inference::MotTracker tracker_;
  zetton::inference::SortTracker tracker_;

 public:
  RosMotTracker(ros::NodeHandle* nh) : nh_(nh), it_(*nh_) {
    // load params
    // hardcoded or using GPARAM
    // std::string image_topic_sub =
    // "/uvds_communication/image_streaming/mavic_0";
    std::string image_topic_sub = "/camera/image";

    // subscribe to input video feed
    image_sub_ = it_.subscribe(image_topic_sub, 1,
                               &RosMotTracker::RosImageCallback, this);
    // publish images
    image_pub_ = it_.advertise("/camera/result", 1);

    // prepare yolo config
    yolo_trt::Config config_v4;
    std::string package_path = ros::package::getPath("zetton_inference");
    // config_v4.net_type = yolo_trt::ModelType::YOLOV4_TINY;
    // config_v4.file_model_cfg = package_path + "/asset/yolov4-tiny-uav.cfg";
    // config_v4.file_model_weights =
    //     package_path + "/asset/yolov4-tiny-uav_best.weights";
    // config_v4.net_type = yolo_trt::ModelType::YOLOV4;
    // config_v4.file_model_cfg = package_path + "/asset/yolov4-608.cfg";
    // config_v4.file_model_weights =
    //     package_path + "/asset/yolov4-608.weights";
    config_v4.net_type = yolo_trt::ModelType::YOLOV4;
    config_v4.file_model_cfg = package_path + "/asset/yolov4-visdrone.cfg";
    config_v4.file_model_weights =
        package_path + "/asset/yolov4-visdrone-best.weights";
    config_v4.inference_precision = yolo_trt::Precision::FP32;
    config_v4.detect_thresh = 0.4;

    // initialize detector
    detector_.Init(config_v4);
    detector_.SetWidthLimitation(50, 1920);
    detector_.SetHeightLimitation(50, 1920);

    // initialize tracker
    tracker_.Init();
  }

  ~RosMotTracker() {
    if (nh_) delete nh_;
  }
};

int main(int argc, char** argv) {
  // init node
  ros::init(argc, argv, "example_ros_mot_tracker");
  auto nh = new ros::NodeHandle("~");

  // catch external interrupt initiated by the user and exit program
  signal(SIGINT, signalHandler);

  // init instance
  RosMotTracker pipeline(nh);

  ros::spin();
  return 0;
}
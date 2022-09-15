# zetton-inference-ros

ROS wrapper of [zetton-inference](https://github.com/project-zetton/zetton-inference).

## Usage

### Object Detection

#### YOLO

Object detection powered by YOLO-family algorithms.

- Receive image form ROS topic and do detection (in `zetton_ros_vendor` repo):

   ```bash
   rosrun zetton_inference example_ros_yolo_object_detector
   ```

- Receive image form RTSP stream and do detection (in `zetton_ros_vendor` repo):

   ```bash
   rosrun zetton_inference example_rtsp_yolo_object_detector
   ```

### Object Tracking

#### SORT

Object tracking powered by SORT algorithms.

- Receive image form ROS topic, and then do detection & tracking (in `zetton_ros_vendor` repo):

   ```bash
   rosrun zetton_inference example_ros_mot_tracker
   ```

#### MOT

Object tracking powered by Optical Flow & ReID.

(W.I.P)

## License

- For academic use, this project is licensed under the 2-clause BSD License - see the [LICENSE file](LICENSE) for details.
- For commercial use, please contact [Yusu Pan](mailto:xxdsox@gmail.com).

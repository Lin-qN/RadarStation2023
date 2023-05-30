/*************************************************************************
  > File Name: video_pub.cpp
  > Author: cyz
  > Mail:
  > Created Time: Sun 24 Feb 2019 05:08:34 PM
  >it situmalated a video file to normal video capture, thus you can use it to debug
 ************************************************************************/


#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <sstream>
#include <iostream>
#include "opencv2/opencv.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <cv_bridge/cv_bridge.h>
#include <std_msgs/Bool.h>
//#include "preprocess.h"

using namespace std;
using namespace cv;
Size large_size = Size(640, 512);
Size small_size = Size(640, 480);
//Size dist_size = large_size;
Size dist_size = small_size;
string video_source_far = "/media/lin/T7 Shield2/视频/0001.哔哩哔哩-第20场 西安交通大学 笃行战队 vs厦门理工学院 PFA战队 RoboMaster 2023 超级对抗赛·区域赛（南部赛区）.mp4";
string video_source_close = "/media/lin/Windows-SSD1/Users/inshu/Desktop/视频/0001.哔哩哔哩-第69场  西安交通大学 笃行战队 vs 深圳大学 RobotPilots战队 RoboMaster 2023 超级对抗赛·区域赛（南部赛区）.mp4";
Mat img_show;



///
/// \brief get_is_large
/// if cam is 1280x1024, we resize it to 640x512
/// \param is_large_resolution
///
void get_is_large(const std_msgs::BoolConstPtr &is_large_resolution)
{
  if(is_large_resolution->data==true)
  {
    dist_size=large_size;
  }else
  {
    dist_size=small_size;
  }
}

int main(int argc, char **argv)
{
  ros::init(argc,argv,"video_publisher");
  ros::NodeHandle nh;
  image_transport::ImageTransport it_far(nh);// useful when publish imgs
  image_transport::ImageTransport it_close(nh);
  image_transport::Publisher pub_far = it_far.advertise("/sensor_far/image_raw", 1);
  image_transport::Publisher pub_close = it_close.advertise("/sensor_close/image_raw", 1);
  ros::Subscriber is_large_sub=nh.subscribe("/mv_param/is_large",1,get_is_large);

  // nh.getParam("/video_source",video_source);

  ros::Rate loop_rate(30);
  Mat fileimg;
  

  VideoCapture cap_far(video_source_far);//open video in the path
  if(!cap_far.isOpened())
  {
    std::cout<<"open video failed!"<<std::endl;
    return -1;
  }
  else
  {
      std::cout<<"open video success!"<<std::endl;
  }

  VideoCapture cap_close(video_source_close);//open video in the path
  if(!cap_close.isOpened())
  {
      std::cout<<"open video failed!"<<std::endl;
      return -1;
  }
  else
  {
      std::cout<<"open video success!"<<std::endl;
  }


  Mat frame_far,frame_close,img_show;
  bool isSuccessf = true;
  bool isSuccessc = true;

  while(nh.ok())
  {
    isSuccessf = cap_far.read(frame_far);
    isSuccessc = cap_close.read(frame_close);
    if(!isSuccessf && !isSuccessc)//if the video ends, then break
    {
      std::cout<<"video ends"<<std::endl;
      break;
    }

  // fileimg=cv::imread("/home/chris/radar_station2022/src/sjtu_battlefield.jpg");
  // fileimg.copyTo(frame);

    /// preprocess: cuda version


    //        processor.proc_update(frame);
    //        if(!processor.compImg.empty())
    //            img_show=processor.compImg;
    //        else
    //            img_show=frame;


    ///preprocess: opencv version
    Size src_size_far = Size(frame_far.cols,frame_far.rows);
    Size src_size_close = Size(frame_close.cols,frame_close.rows);
//    if(src_size!=dist_size)   // resize to 640x512
//      resize(frame, frame, dist_size);


//    sensor_msgs::ImagePtr msg = cv_bridge::CvImage(std_msgs::Header(), "mono8", img_show).toImageMsg();
    std_msgs::Header header;
    header.stamp=ros::Time::now();
    sensor_msgs::ImagePtr msg_far = cv_bridge::CvImage(header, "bgr8", frame_far).toImageMsg();
    sensor_msgs::ImagePtr msg_close = cv_bridge::CvImage(header, "bgr8", frame_close).toImageMsg();
    pub_far.publish(msg_far);
    pub_close.publish(msg_close);

    ros::spinOnce();
    loop_rate.sleep();
  }
  return 0;
}

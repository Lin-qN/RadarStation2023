#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace cv;
bool if_shot = true;
char ad[100] = { 0 };
void imgCallback(const sensor_msgs::ImageConstPtr& msg)
{
    if (if_shot)
    {
        Mat img = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image.clone();
        static int d = 0;
        imshow("shot", img);
        int k = cv::waitKey(0);
        if(!img.empty())
        {
            sprintf(ad, "/home/lin/pictures/%d.jpg", d++);
            cv::imwrite(ad, img);
            ROS_INFO("Get %d pictures!", d);
        }
    }
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "getPictures_node");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("/sensor_far/image_raw", 1, &imgCallback);
    std::cout<<"ROS start!"<<std::endl;
    ros::spin();
}

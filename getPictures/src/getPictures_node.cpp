#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace cv;
bool if_shot = true;
char ad[100] = { 0 };
void imgCallback(const sensor_msgs::ImageConstPtr& msg)
{
    std::cout<<111<<std::endl;
    if (if_shot)
    {
        cv::Mat img = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image;
        static int d = 0;
        cv::imshow("shot", img);
        int k = cv::waitKey(0);
        if(k == 'p' && !img.empty())
        {
            cv::imwrite("/home/lin/pictures/%d.jpg", img);
        }
    }
}


int main(int argc, char **argv)
{
    ros::init(argc, argv, "getPictures_node");
    ros::NodeHandle n;
    ros::Subscriber sub = n.subscribe("/sensor_far/image_raw", 10, &imgCallback);
    std::cout<<"ROS start!"<<std::endl;
    ros::spin();
}

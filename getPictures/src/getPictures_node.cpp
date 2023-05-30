#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace cv;
bool if_shot_far = false;
bool if_shot_close = false;
char ad[100] = { 0 };
static long f = 9844;
static long c = 7399;
void farCallback(const sensor_msgs::ImageConstPtr& msg)
{
    if (if_shot_far)
    {
        Mat img = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image.clone();
        //imshow("shot", img);
        //int k = cv::waitKey(0);
        if(!img.empty())
        {
            sprintf(ad, "/home/lin/pictures/farall/%ld.jpg", f+=2);
            cv::imwrite(ad, img);
            ROS_INFO("Get %ld pictures!", f);
            ros::Rate loop_rate_far(5);
            loop_rate_far.sleep();
        }
    }
}
void closeCallback(const sensor_msgs::ImageConstPtr& msg)
{
    if (if_shot_close)
    {
        Mat img = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image.clone();
        //imshow("shot", img);
        //int k = cv::waitKey(0);
        if(!img.empty())
        {
            sprintf(ad, "/home/lin/pictures/closeall/%ld.jpg", c+=2);
            cv::imwrite(ad, img);
            ROS_INFO("Get %ld pictures!", c);
            ros::Rate loop_rate_close(5);
            loop_rate_close.sleep();
        }
    }
}

int main(int argc, char **argv)
{
    ros::init(argc, argv, "getPictures_node");
    ros::NodeHandle n;
    ros::Subscriber sub_far = n.subscribe("/sensor_far/image_raw", 1, &farCallback);
    ros::Subscriber sub_close = n.subscribe("/sensor_close/image_raw", 1, &closeCallback);
    std::cout<<"ROS start!"<<std::endl;
    ros::spin();
}

#include <ros/ros.h>
#include <opencv2/opencv.hpp>
#include <cv_bridge/cv_bridge.h>

using namespace cv;
bool if_shot = true;
char ad[100] = { 0 };
void farCallback(const sensor_msgs::ImageConstPtr& msg)
{
    if (if_shot)
    {
        Mat img = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image.clone();
        static int f = 0;
        //imshow("shot", img);
        //int k = cv::waitKey(0);
        if(!img.empty())
        {
            sprintf(ad, "/home/lin/pictures/farall/%d.jpg", f++);
            cv::imwrite(ad, img);
            ROS_INFO("Get %d pictures!", f);
            ros::Rate loop_rate_far(1);
            loop_rate_far.sleep();
        }
    }
}
void closeCallback(const sensor_msgs::ImageConstPtr& msg)
{
    if (if_shot)
    {
        Mat img = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8)->image.clone();
        static int c = 0;
        //imshow("shot", img);
        //int k = cv::waitKey(0);
        if(!img.empty())
        {
            sprintf(ad, "/home/lin/pictures/closeall/%d.jpg", c++);
            cv::imwrite(ad, img);
            ROS_INFO("Get %d pictures!", c);
            ros::Rate loop_rate_close(1);
            loop_rate_close.sleep();
        }
    }
}

int main(int argc, char **argv)
{
    ros::param::get("/yolo/if_shot", if_shot);
    ros::init(argc, argv, "getPictures_node");
    ros::NodeHandle n;
    ros::Subscriber sub_far = n.subscribe("/sensor_far/image_raw", 1, &farCallback);
    ros::Subscriber sub_close = n.subscribe("/sensor_close/image_raw", 1, &closeCallback);
    std::cout<<"ROS start!"<<std::endl;
    ros::spin();
}

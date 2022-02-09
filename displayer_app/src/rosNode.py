#!/usr/bin/env python3
# -*- encoding: utf-8 -*-
# @Author : Bismarckkk
# @Site   : https://github.com/bismarck
# @File   : rosNode.py

import sys

import numpy as np
import rospy
from sensor_msgs.msg import Image

from base import BaseNode, BaseImageSubscriber
import config

"""
@warning: 当在配置文件中打开cvBridge选项时，将会引入cv_bridge包替代自编image消息解析程序，请自行为python3编译cv_bridge包
"""
if config.isCvBridge:
    import cv_bridge
    bridge = cv_bridge.CvBridge()


class RosNode(BaseNode):
    """
    @brief: RosNode类，继承自BaseNode类
    @details: 在启动后初始化ros句柄，使用spin()方法运行ros后端
    """
    def __init__(self):
        super().__init__()
        rospy.init_node('displayer')

    def run(self):
        rospy.spin()


class RosImageSubscriber(BaseImageSubscriber):
    """
    @brief: ros相机订阅者类，继承自BaseImageSubscriber类
    @fn self.img_to_cv2: 通过cv_bridge或者自编解析image消息获取cv2图像
    """
    def __init__(self, topic):
        super().__init__()
        self.topic = topic
        self.sub = rospy.Subscriber(self.topic, Image, self.callback, 1)

    @staticmethod
    def img_to_cv2(img_msg: Image):
        """
        @param img_msg: ros sensor_msgs/Image消息
        @return: cv2图像
        @warning: 自编解析代码仅支持bgr8和rgb8编码
        """
        if config.isCvBridge:
            return bridge.imgmsg_to_cv2(img_msg, 'bgr8')
        else:
            dtype = np.dtype("uint8")
            dtype = dtype.newbyteorder('>' if img_msg.is_bigendian else '<')
            image_opencv = np.ndarray(shape=(img_msg.height, img_msg.width, 3),
                                      dtype=dtype, buffer=img_msg.data)
            if img_msg.is_bigendian == (sys.byteorder == 'little'):
                image_opencv = image_opencv.byteswap().newbyteorder()
            if img_msg.encoding == 'bgr8':
                return image_opencv
            elif img_msg.encoding == 'rgb8':
                return image_opencv[:, :, [2, 1, 0]]
            else:
                raise ValueError('Unsupported encoding: ' + img_msg.encoding)

    def callback(self, data, _):
        self.queue.put(self.img_to_cv2(data))


imageSubscribers = {}
for cfg in config.cameraConfig:
    imageSubscribers[cfg['topic']] = RosImageSubscriber(cfg['topic'])

#!/usr/bin/env python
import rospy
import numpy as np
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import cv2


# Initialize depth interceptor node
rospy.init_node('depth_extractor',anonymous=False)

# raw data image converter
bridge = CvBridge()


# Create depth subscriber
def depth_callback(depth_msg):
	depthImg = bridge.imgmsg_to_cv2(depth_msg)
	
	print depthImg[240,320] * 3.28
	cv2.circle(depthImg,(320,240), 5, (0,0,255), -1)
	cv2.imshow("depth image", depthImg)
	cv2.waitKey(3)
	
depth_subscriber = rospy.Subscriber('/camera/depth/image',Image, depth_callback)

rate = rospy.Rate(10)

def depth_extractor():
	while not rospy.is_shutdown():
		rate.sleep()

if __name__ == '__main__':
	try:
		depth_extractor()
	except rospy.ROSInterruptException:
		pass

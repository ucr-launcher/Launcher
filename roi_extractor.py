#!/usr/bin/env python
import rospy
import numpy as np
import cv2
from std_msgs.msg import UInt16
from sensor_msgs.msg import Image
from senior_design.msg import roi_image
from cv_bridge import CvBridge, CvBridgeError

# Ros Image data converter
bridge = CvBridge()

global rgbImg
rgbImg = np.zeros(shape=(480,640,3))

#--------------------- ROS NODE CONFIGURATION -------------------------------
rospy.init_node('object_detector', anonymous=False)

# Set up Publishers
roi = roi_image()
roi.x = 0
roi.y = 0

roi_publisher = rospy.Publisher('/roi_location', roi_image, queue_size=1)

# Set up Subscribers
def image_callback(image_msg):
	global rgbImg
	# Convert ros image to cv image	
	rgbImg = bridge.imgmsg_to_cv2(image_msg,"bgr8")
	
	
image_subscriber = rospy.Subscriber('/camera/rgb/image_color', Image, image_callback)

# Set loop rate in Hz  
rate = rospy.Rate(20)
#----------------------- END CONFIGURATION ----------------------------------


#-------------------- OBJECT DETECTION PIPELINE -----------------------------
def object_detector():

	def nothing(x):
		pass

	cv2.namedWindow('result')

	# h  16 s  153 v  142
	# h1 45 s1 255 v1 255
	
	h = 100
	s = 055
	v = 106

	h1 = 166
	s1 = 178
	v1 = 255
 
	# Creating track bar
	cv2.createTrackbar('h', 'result',0,179, nothing)
	cv2.createTrackbar('s', 'result',0,255, nothing)
	cv2.createTrackbar('v', 'result',0,255, nothing)
	cv2.createTrackbar('h1', 'result',0,179, nothing)
	cv2.createTrackbar('s1', 'result',35,255, nothing)
	cv2.createTrackbar('v1', 'result',0,255, nothing)

	cv2.setTrackbarPos('h','result',100)
	cv2.setTrackbarPos('s','result',055)
	cv2.setTrackbarPos('v','result',106)
	cv2.setTrackbarPos('h1','result',166)
	cv2.setTrackbarPos('s1','result',178)
	cv2.setTrackbarPos('v1','result',255)

	while not rospy.is_shutdown():
		try:
			# Convert to hsv color space
			hsvImg = cv2.cvtColor(rgbImg, cv2.COLOR_BGR2HSV)
		
			# get info from track bar and appy to result
			h = cv2.getTrackbarPos('h','result')
			s = cv2.getTrackbarPos('s','result')
			v = cv2.getTrackbarPos('v','result')

			h1 = cv2.getTrackbarPos('h1','result')
			s1 = cv2.getTrackbarPos('s1','result')
			v1 = cv2.getTrackbarPos('v1','result')


			# Color based Threshold
			lower_orange = np.array([h,s,v])
			upper_orange = np.array([h1,s1,v1])
		
			thresh_img = cv2.inRange(hsvImg, lower_orange, upper_orange)

			result = cv2.bitwise_and(rgbImg, rgbImg, mask = thresh_img)
			result = cv2.cvtColor(result, cv2.COLOR_BGR2GRAY)
			
			# Circle Detection
			circles = cv2.HoughCircles(result,cv2.HOUGH_GRADIENT,
				   1,400,param1=20,param2=50,
				   minRadius=10 ,maxRadius=300)
			
			# Publish ROI (x,y) location 
			x = 0
			y = 0

			if circles is not  None:
				circles = np.uint16(np.around(circles))
				for i in circles[0,:]:
					# Draw Outer circle
					# parameters: (frame, x, y, radius, color, thickness)
					cv2.circle(rgbImg,(i[0],i[1]),i[2],(0,255,0),2)
					# Draw Centroid
					cv2.circle(rgbImg,(i[0],i[1]),2,(0,0,255),3)
					x = i[0]
					y = i[1]
					text = '(' + str(i[0]) + ', ' + str(i[1]) + ')'
					cv2.putText(rgbImg,text,(i[0],i[1]),cv2.FONT_ITALIC,1, 255)
	
			print('Xpos: {}, Ypos: {} '.format(x, y))
			
			# Display result	
			cv2.imshow("object_detected", rgbImg)
			cv2.imshow("thresh_image", result)
			cv2.waitKey(3)

		except :
			pass	
		
		rate.sleep()
#-------------------- END DETECTION PIPELINE -------------------------------


#------------------------- MAIN LOOP ----------------------------------------
if __name__ == "__main__":
	try:
		object_detector()
	except rospy.ROSInterruptException:
		pass

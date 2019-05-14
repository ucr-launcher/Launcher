#!/usr/bin/env python
# coding: utf-8
import numpy as np
import os
import sys
import six.moves.urllib as urllib
# This is needed since the notebook is stored in the object_detection folder.
sys.path.append("..")
import tarfile
import tensorflow as tf
import time
import copy

from tensorflow.core.framework import graph_pb2
from utils import label_map_util
from utils import visualization_utils as vis_util
from object_detection.utils import ops as utils_ops
#from matplotlib import pyplot as plt
from PIL import Image
from io import StringIO
from collections import defaultdict
import cv2

import rospy
from std_msgs.msg import UInt16, Int16
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

print "completed imports"

#cap = cv2.VideoCapture(0)

# Ros Image data converter
bridge = CvBridge()

global rgbImg
rgbImg = np.zeros(shape=(480,640,3))

##-------------------------------------------------------------------------------------------------
##-------------------------------------------------------------------------------------------------

def _node_name(n):
  if n.startswith("^"):
    return n[1:]
  else:
    return n.split(":")[0]


input_graph = tf.Graph()
with tf.Session(graph=input_graph):
    score = tf.placeholder(tf.float32, shape=(None, 1917, 90), name="Postprocessor/convert_scores")
    expand = tf.placeholder(tf.float32, shape=(None, 1917, 1, 4), name="Postprocessor/ExpandDims_1")
    for node in input_graph.as_graph_def().node:
        if node.name == "Postprocessor/convert_scores":
            score_def = node
        if node.name == "Postprocessor/ExpandDims_1":
            expand_def = node


detection_graph = tf.Graph()
with detection_graph.as_default():
  od_graph_def = tf.GraphDef()
  with tf.gfile.GFile('./ssd_mobilenet_v1_coco_2017_11_17/frozen_inference_graph.pb', 'rb') as fid:
    serialized_graph = fid.read()
    od_graph_def.ParseFromString(serialized_graph)
    dest_nodes = ['Postprocessor/convert_scores','Postprocessor/ExpandDims_1']

    edges = {}
    name_to_node_map = {}
    node_seq = {}
    seq = 0
    for node in od_graph_def.node:
      n = _node_name(node.name)
      name_to_node_map[n] = node
      edges[n] = [_node_name(x) for x in node.input]
      node_seq[n] = seq
      seq += 1

    for d in dest_nodes:
      assert d in name_to_node_map, "%s is not in graph" % d

    nodes_to_keep = set()
    next_to_visit = dest_nodes[:]
    while next_to_visit:
      n = next_to_visit[0]
      del next_to_visit[0]
      if n in nodes_to_keep:
        continue
      nodes_to_keep.add(n)
      next_to_visit += edges[n]

    nodes_to_keep_list = sorted(list(nodes_to_keep), key=lambda n: node_seq[n])

    nodes_to_remove = set()
    for n in node_seq:
      if n in nodes_to_keep_list: continue
      nodes_to_remove.add(n)
    nodes_to_remove_list = sorted(list(nodes_to_remove), key=lambda n: node_seq[n])

    keep = graph_pb2.GraphDef()
    for n in nodes_to_keep_list:
      keep.node.extend([copy.deepcopy(name_to_node_map[n])])

    remove = graph_pb2.GraphDef()
    remove.node.extend([score_def])
    remove.node.extend([expand_def])
    for n in nodes_to_remove_list:
      remove.node.extend([copy.deepcopy(name_to_node_map[n])])

    with tf.device('/gpu:0'):
      tf.import_graph_def(keep, name='')
    with tf.device('/cpu:0'):
      tf.import_graph_def(remove, name='')


NUM_CLASSES = 90
label_map = label_map_util.load_labelmap('data/mscoco_label_map.pbtxt')
categories = label_map_util.convert_label_map_to_categories(label_map, max_num_classes=NUM_CLASSES, use_display_name=True)
category_index = label_map_util.create_category_index(categories)


def load_image_into_numpy_array(image):
  (im_width, im_height) = image.size
  return np.array(image.getdata()).reshape(
      (im_height, im_width, 3)).astype(np.uint8)

# For the sake of simplicity we will use only 2 images:
# image1.jpg
# image2.jpg
# If you want to test the code with your images, just add path to the images to the TEST_IMAGE_PATHS.
PATH_TO_TEST_IMAGES_DIR = 'test_images'
TEST_IMAGE_PATHS = [ os.path.join(PATH_TO_TEST_IMAGES_DIR, 'image{}.jpg'.format(i)) for i in range(1, 3) ]

# Size, in inches, of the output images.
IMAGE_SIZE = (12, 8)

##-----------------------------------------------------------------------------------------------##
##-----------------------------------------------------------------------------------------------##

#--------------------- ROS NODE CONFIGURATION -------------------------------
rospy.init_node('object_detector', anonymous=False)

# Set up Publishers
x_loc = Int16()
x_loc.data = 0

xloc_publisher = rospy.Publisher('/xaxis_location', Int16, queue_size=1)


# Set up Subscribers
def image_callback(image_msg):
	global rgbImg
	# Convert ros image to cv image	
	rgbImg = bridge.imgmsg_to_cv2(image_msg,"bgr8")
	
	
image_subscriber = rospy.Subscriber('/camera/rgb/image_color', Image, image_callback)

# Set loop rate in Hz  
rate = rospy.Rate(20)

#----------------------- END CONFIGURATION ----------------------------------


with detection_graph.as_default():
  with tf.Session(graph=detection_graph,config=tf.ConfigProto(allow_soft_placement=True)) as sess:
    image_tensor = detection_graph.get_tensor_by_name('image_tensor:0')
    score_out = detection_graph.get_tensor_by_name('Postprocessor/convert_scores:0')
    expand_out = detection_graph.get_tensor_by_name('Postprocessor/ExpandDims_1:0')
    score_in = detection_graph.get_tensor_by_name('Postprocessor/convert_scores_1:0')
    expand_in = detection_graph.get_tensor_by_name('Postprocessor/ExpandDims_1_1:0')
    detection_boxes = detection_graph.get_tensor_by_name('detection_boxes:0')
    detection_scores = detection_graph.get_tensor_by_name('detection_scores:0')
    detection_classes = detection_graph.get_tensor_by_name('detection_classes:0')
    num_detections = detection_graph.get_tensor_by_name('num_detections:0')
    i = 0
    while not rospy.is_shutdown():
      #ret, image_np = cap.read()
      image_np = rgbImg
      
      image_np_expanded = np.expand_dims(image_np, axis=0)
    
      start_time = time.time()
      (score, expand) = sess.run([score_out, expand_out], feed_dict={image_tensor: image_np_expanded})
      (boxes, scores, classes, num) = sess.run(
            [detection_boxes, detection_scores, detection_classes, num_detections],
            feed_dict={score_in:score, expand_in: expand})
      ##print 'Iteration %d: %.3f sec'%(i, time.time()-start_time)
      i += 1
      vis_util.visualize_boxes_and_labels_on_image_array(
        image_np,
       np.squeeze(boxes),
      np.squeeze(classes).astype(np.int32),
      np.squeeze(scores),
      category_index,
      use_normalized_coordinates=True,
      line_thickness=8)
	
      # Iterate through boxes and look for sports ball!!!
      for i,b in enumerate(boxes[0]):
      	if classes[0][i] == 1 or classes[0][i] == 37:
	  mid_x = (boxes[0][i][1]+boxes[0][i][3])/2
          mid_y = (boxes[0][i][0]+boxes[0][i][2])/2
	  
	  if scores[0][i] >= 0.5: 
	    print "Person at: " + str(mid_x*800) + ", " + str(mid_y*600)
		
            # Publish x-axis
	    x_loc.data = mid_x*800
            xloc_publisher.publish(x_loc) 

      cv2.imshow('object detection', cv2.resize(image_np, (800,600)))
      if cv2.waitKey(25) & 0xFF == ord('q'):
	cv2.destroyAllWindows()
	break
    
    

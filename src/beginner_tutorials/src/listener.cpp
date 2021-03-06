   #include "ros/ros.h"
   #include "std_msgs/String.h"
   #include <image_transport/image_transport.h>
   #include <cv_bridge/cv_bridge.h>
   #include <sensor_msgs/image_encodings.h>
   #include <opencv2/imgproc/imgproc.hpp>
   #include <opencv2/highgui/highgui.hpp>
   using namespace cv;
   using namespace std;
   
   image_transport::Subscriber image_sub_;
   image_transport::Subscriber image_map_sub_;
   image_transport::Publisher image_pub_;

   Mat mapImage;

   /**
    * This tutorial demonstrates simple receipt of messages over the ROS system.
    */
   void chatterCallback(const std_msgs::String::ConstPtr& msg)
   {
     ROS_INFO("I heard: [%s]", msg->data.c_str());
   }


   void imageMapCallback(const sensor_msgs::ImageConstPtr& msg)
   {
	cv_bridge::CvImagePtr cv_ptr;
	try
	{
		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
	}
	catch (cv_bridge::Exception& e)
	{
		ROS_ERROR("cv_bridge exception: %s", e.what());
		return;
	}

        mapImage=cv_ptr->image;
        
        cvtColor(cv_ptr->image, mapImage, cv::COLOR_BGR2GRAY);
   }  


   int main(int argc, char **argv)
   {
     /**
      * The ros::init() function needs to see argc and argv so that it can perform
      * any ROS arguments and name remapping that were provided at the command line.
      * For programmatic remappings you can use a different version of init() which takes
      * remappings directly, but for most command-line programs, passing argc and argv is
      * the easiest way to do it.  The third argument to init() is the name of the node.
      *
      * You must call one of the versions of ros::init() before using any other
      * part of the ROS system.
      */
     //ros::init(argc, argv, "listener");
     ros::init(argc, argv, "navigator");
     /**
      * NodeHandle is the main access point to communications with the ROS system.
      * The first NodeHandle constructed will fully initialize this node, and the last
      * NodeHandle destructed will close down the node.
      */
     ros::NodeHandle nh;
     image_transport::ImageTransport it_(nh);

     image_map_sub_ = it_.subscribe( "/map_image", 1,imageMapCallback);
     
   
     /**
      * The subscribe() call is how you tell ROS that you want to receive messages
      * on a given topic.  This invokes a call to the ROS
      * master node, which keeps a registry of who is publishing and who
      * is subscribing.  Messages are passed to a callback function, here
      * called chatterCallback.  subscribe() returns a Subscriber object that you
      * must hold on to until you want to unsubscribe.  When all copies of the Subscriber
      * object go out of scope, this callback will automatically be unsubscribed from
      * this topic.
      *
      * The second parameter to the subscribe() function is the size of the message
      * queue.  If messages are arriving faster than they are being processed, this
      * is the number of messages that will be buffered up before beginning to throw
      * away the oldest ones.
      */
     ros::Subscriber sub = nh.subscribe("chatter", 1000, chatterCallback);

   
     /**
      * ros::spin() will enter a loop, pumping callbacks.  With this version, all
      * callbacks will be called from within this thread (the main one).  ros::spin()
      * will exit when Ctrl-C is pressed, or the node is shutdown by the master.
      */
     ros::spin();
   
     return 0;
  }

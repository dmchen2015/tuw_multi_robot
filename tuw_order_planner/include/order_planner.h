#ifndef __TUW_ORDERMANAGER_H
#define __TUW_ORDERMANAGER_H

#include <stdio.h>

#include <ros/ros.h>
#include <tuw_multi_robot_msgs/OrderArray.h>
#include <tuw_multi_robot_msgs/GoodPosition.h>
#include <tuw_multi_robot_msgs/Pickup.h>
#include <tuw_multi_robot_msgs/RobotInfo.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose.h>
#include <map>

#include "abstractsolver.h"
#include "simplesolver.h"

namespace tuw_order_planner
{
enum Modes
{
  MODE_INIT,
  MODE_PROGRESS
};

class OrderPlanner
{
public:
  OrderPlanner(int, char**);
  void run();

private:
  void robotInfoCallback(const tuw_multi_robot_msgs::RobotInfo::ConstPtr& robotInfo);
  void ordersCallback(const tuw_multi_robot_msgs::OrderArray::ConstPtr& orders);
  void odomCallback(const nav_msgs::Odometry& odom);
  int incrementRobotProgress(std::string robot_name);
  void subscribeRobotOdom();
  void route();
  void reset();

  tuw_multi_robot_msgs::Order* findOrderByRobotName(std::string robot_name);
  tuw_multi_robot_msgs::Order* findOrderByGoodId(int id);
  int findGoodIdByRobotName(std::string robot_name);

  void publishGoodPosition(int good_id, geometry_msgs::Pose pose);
  void publishPickup(std::string robot_name, int good_id);


  ros::Publisher pub_robot_goals_;
  ros::Publisher pub_pickup_;
  ros::Publisher pub_good_position_;
  std::vector<ros::Subscriber> subscribers_;
  ros::NodeHandle* nodeHandle_;

  std::map<std::string, geometry_msgs::Pose*> subscribed_robots_;
  std::map<std::string, int> robots_status_;
  std::map<std::string, int> robots_progress_;
  std::vector<TransportPair> transport_pairs_;
  std::map<std::string, int> attached_goods_;

  std::vector<tuw_multi_robot_msgs::Order> orders_;

  int mode_;
};

}  // end namespace tuw_order_planner

#endif

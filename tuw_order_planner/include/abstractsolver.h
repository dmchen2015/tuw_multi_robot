#ifndef __TUW_ORDERMANAGER_ABSTRACTSOLVER_H
#define __TUW_ORDERMANAGER_ABSTRACTSOLVER_H

#include <ros/ros.h>
#include <tuw_multi_robot_msgs/Order.h>
#include <nav_msgs/Odometry.h>
#include <geometry_msgs/Pose.h>
#include <map>
#include <string>

namespace tuw_order_planner
{
struct TransportPair
{
  std::string robot_name;
  int good_id;
};

class AbstractSolver
{
public:
  explicit AbstractSolver(
      std::map<std::string, geometry_msgs::Pose*> robots,
      std::vector<tuw_multi_robot_msgs::Order> orders)
    : robots_(robots), orders_(orders){};

  virtual std::vector<TransportPair> solve() = 0;

protected:
  std::map<std::string, geometry_msgs::Pose*> robots_;
  std::vector<tuw_multi_robot_msgs::Order> orders_;
};

}  // end namespace tuw_order_planner

#endif

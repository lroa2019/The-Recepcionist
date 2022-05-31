// Copyright 2022 los ultramarinos
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <string>

#include "behavior_tree/NavegarTR.h"

namespace behavior_trees
{
Navegar::Navegar(const std::string& name, const BT::NodeConfiguration & config):
BT::ActionNodeBase(name, config), nh_(), feedBack(" ")
{
  activador = nh_.advertise<geometry_msgs::PoseStamped>("move_base_simple/goal", 10);
  poseSub = nh_.subscribe("/navegar_arbitro", 10, &Navegar::posicionNav, this);
  sub = nh_.subscribe("/move_base/result", 10, &Navegar::messageCallback, this);
}

void Navegar::messageCallback(const move_base_msgs::MoveBaseActionResult::ConstPtr& msg)
{
  feedBack = msg->status.text;
  std::cout << "Resultado Navegacion : " << feedBack << "\n";
}

void Navegar::posicionNav(const geometry_msgs::PoseStamped::ConstPtr& pose){
  result = *pose;
}

void Navegar::halt()
{
  ROS_INFO("Seguir halt");
}

BT::NodeStatus Navegar::tick()
{
  if (a == 5)
  {
    std::cout << a << "\n";

    activador.publish(result);
  }
  a++;

  if (feedBack != "")
  {
    if (feedBack == "Goal reached.")
    {
      return BT::NodeStatus::SUCCESS;
    }
    else
    {
      return BT::NodeStatus::FAILURE;
    }
  }
  return BT::NodeStatus::RUNNING;
}
}  // namespace behavior_trees

BT_REGISTER_NODES(factory)
{
  factory.registerNodeType<behavior_trees::Navegar>("Navegar");
}
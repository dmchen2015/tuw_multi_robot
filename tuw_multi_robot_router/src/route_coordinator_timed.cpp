/*
 * Copyright (c) 2017, <copyright holder> <email>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *     * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *     * Neither the name of the <organization> nor the
 *     names of its contributors may be used to endorse or promote products
 *     derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY <copyright holder> <email> ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <copyright holder> <email> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 */

#include <tuw_global_planner/route_coordinator_timed.h>

#define TIME_INFINITY   (-1)
//#define DEBUG

RouteCoordinatorTimed::RouteCoordinatorTimed() : RouteCoordinator()
{
}


bool RouteCoordinatorTimed::addRoute(const std::vector< RouteVertex > &_path, const uint32_t _diameterPixel)
{
    for(uint32_t i = 0; i < _path.size(); i++)
    {
        uint32_t begin = 0;
        int32_t end = TIME_INFINITY;

        if(i != 0)
        {
            begin = _path[i - 1].potential;
        }

        if(i != _path.size() - 1)
        {
            end = _path[i].potential;
        }

        
        if(!timeline_.addSegment(begin, end, _path[i].getSegment().getSegmentId(), activeRobot_, _diameterPixel, true))
        {
            return false;
        }

        std::vector<uint32_t> pred = _path[i].getSegment().getPredecessors();

        if(pred.size() > 1)
        {
            //for(auto it = pred.cbegin(); it != pred.cend(); it++)
            for(const uint32_t  idx : pred)
            {
                if(!timeline_.addCrossingSegment(begin, end, idx, activeRobot_, _diameterPixel, false))
                {
                    return false;
                }

            }
        }

        std::vector<uint32_t>  succ = _path[i].getSegment().getSuccessors();

        if(succ.size() > 1)
        {
            //for(auto it = succ.cbegin(); it != succ.cend(); it++)
            for(const uint32_t  idx : succ)
            {
                if(!timeline_.addCrossingSegment(begin, end, idx, activeRobot_, _diameterPixel, false))
                {
                    return false;
                }

            }
        }

    }

    return true;
}

bool RouteCoordinatorTimed::isGoal(const Vertex  &_seg) const
{
    return  _seg.getSegment().getSegmentId() == goalSegments_[activeRobot_];
}

const uint32_t RouteCoordinatorTimed::getEnd() const
{
    return goalSegments_[activeRobot_];
}

const uint32_t RouteCoordinatorTimed::getStart() const
{
    return startSegments_[activeRobot_];
}





bool RouteCoordinatorTimed::checkSegment(const Vertex &_next, const uint32_t _startTime, const int32_t _endTime, const uint32_t _diameterPixel, int32_t &_collisionRobot, bool _ignoreGoal) const
{
    if(!checkSegmentSingle(_next, _startTime, _endTime, _diameterPixel, _collisionRobot, _ignoreGoal))
    {
        return false;
    }

//     const std::vector<uint32_t> pred = _next.getSegment().getPredecessors();
// 
//     if(pred.size() > 1)
//     {
//         //for(auto it = pred.cbegin(); it != pred.cend(); it++)
//         for(const uint32_t idx : pred)
//         {
//             if(!timeline_.checkCrossingSegment(_startTime, _endTime, idx, activeRobot_, _diameterPixel, _collisionRobot))
//             {
//                 return false;
//             }
// 
//         }
//     }
// 
//     std::vector<uint32_t> succ = _next.getSegment().getSuccessors();
// 
//     if(succ.size() > 1)
//     {
//         //for(auto it = succ.cbegin(); it != succ.cend(); it++)
//         for(const uint32_t idx : succ)
//         {
//             if(!timeline_.checkCrossingSegment(_startTime, _endTime, idx, activeRobot_, _diameterPixel, _collisionRobot))
//             {
//                 return false;
//             }
// 
//         }
//     }


    return true;
}


bool RouteCoordinatorTimed::checkSegmentSingle(const Vertex &_next, const uint32_t _startTime, const int32_t _endTime, const uint32_t _diameterPixel, int32_t &_collisionRobot, const bool &_ignoreGoal) const
{
    if(isGoal(_next) && !_ignoreGoal)
    {
        return timeline_.checkSegment(_startTime, TIME_INFINITY, _next.getSegment().getSegmentId(), activeRobot_, _diameterPixel, _collisionRobot);
    }
    else
    {
        return timeline_.checkSegment(_startTime, _endTime, _next.getSegment().getSegmentId(), activeRobot_, _diameterPixel, _collisionRobot);
    }
}


void RouteCoordinatorTimed::reset(const std::vector<Segment>  &_graph, const uint32_t _nrRobots)
{
    timeline_.reset(_graph);

    goalSegments_.clear();
    startSegments_.clear();
}

void RouteCoordinatorTimed::setActive(const uint32_t _robotNr)
{
    activeRobot_ = _robotNr;
}

bool RouteCoordinatorTimed::setGoalSegments(const std::vector<uint32_t> &_goalSegments)
{
    goalSegments_.clear();

    for(int i = 0; i < _goalSegments.size(); i++)
    {
        goalSegments_.emplace_back(_goalSegments[i]);
    }
    return true;
}

bool RouteCoordinatorTimed::setStartSegments(const std::vector<uint32_t> &_startSegments)
{
    startSegments_.clear();

    for(int i = 0; i < _startSegments.size(); i++)
    {
        startSegments_.emplace_back(_startSegments[i]);
    }
    return true;
}


int32_t RouteCoordinatorTimed::findSegNr(const uint32_t _robot, const  uint32_t _potential) const
{
    int32_t segId = timeline_.findSegId(_robot, _potential);

    return segId;
}


int32_t RouteCoordinatorTimed::findPotentialUntilRobotOnSegment(const uint32_t _robot, const uint32_t _segId, const int32_t _potential) const
{
    return timeline_.getTimeUntilRobotOnSegment(_robot, _segId);
}


std::vector< std::pair<uint32_t, float> > RouteCoordinatorTimed::getListOfRobotsHigherPrioritizedRobots(const uint32_t _robot, const uint32_t _segId, const int32_t _potential) const
{
    return timeline_.getListOfRobotsHigherPrioritizedRobots(_robot, _segId, _potential);
}





































RouteCoordinatorTimed::Timeline::Timeline()
{
}

void RouteCoordinatorTimed::Timeline::reset(const std::vector< Segment > &_graph)
{
    timeline_.clear();
    segmentSpace_.clear();
    maxTime_ = 0;

    for(int i = 0; i < _graph.size(); i++)
    {
        timeline_.emplace_back();
        segmentSpace_.push_back(_graph[i].width());
    }
}

bool RouteCoordinatorTimed::Timeline::addSegment(const uint32_t _startTime, const int32_t _endTime, const uint32_t _segId, const uint32_t _robotNr, const uint32_t _robotSize, bool _mainSeg)
{
    int collision;

    if(!checkSegment(_startTime, _endTime, _segId, _robotNr, _robotSize, collision))
    {
        return false;
    }

    int freeSpace = segmentSpace_[_segId];

    if(_endTime > maxTime_)
    {
        maxTime_ = _endTime;
    }

    //Vertex has enough space
    if(robotSegments_.size() <= _robotNr)
    {
        robotSegments_.resize(_robotNr + 1);
    }

    robotSegments_[_robotNr].push_back(_segId);
    
    bool found = false;
    for(seg_occupation &element : timeline_[_segId])
    {
        if(element.robot == _robotNr)
        {
            if(_startTime <= element.endTime && _startTime >= element.startTime)
            {
                //Segment is after existing one
                found = true;
                if(_endTime > element.endTime)
                    element.endTime == _endTime;
            }
            else if(_endTime <= element.endTime && _endTime >= element.startTime)
            {
                //Segment is after existing one
                found = true;
                if(_startTime < element.startTime)
                    element.startTime == _startTime;
            }
            else if(_endTime > element.endTime && _startTime < element.startTime)
            {
                found = true;
                element.startTime = _startTime;
                element.endTime = _endTime;
            }
        }
    }
    
    if(!found)
        timeline_[_segId].emplace_back(_robotNr, (float) _robotSize, _startTime, _endTime, _mainSeg);

    return true;
}

bool RouteCoordinatorTimed::Timeline::addCrossingSegment(const uint32_t _startTime, const int32_t _endTime, const uint32_t _segId, const uint32_t _robotNr, const uint32_t _robotSize, const bool &_mainSeg)
{
    if(_robotSize > segmentSpace_[_segId])
    {
        return addSegment(_startTime, _endTime, _segId, _robotNr, segmentSpace_[_segId], _mainSeg); 
    }

    return addSegment(_startTime, _endTime, _segId, _robotNr, _robotSize, _mainSeg);
}

bool RouteCoordinatorTimed::Timeline::checkCrossingSegment(const uint32_t _startTime, const int32_t _endTime, const uint32_t _segId, const uint32_t _robotNr, const uint32_t _robotSize, int32_t &_lastCollisionRobot) const
{
    if(_robotSize > segmentSpace_[_segId])
    {
        return checkSegment(_startTime, _endTime, _segId, _robotNr, segmentSpace_[_segId], _lastCollisionRobot);   
    }

    return checkSegment(_startTime, _endTime, _segId, _robotNr, _robotSize, _lastCollisionRobot);

}



bool RouteCoordinatorTimed::Timeline::checkSegment(const uint32_t _startTime, const int32_t _endTime, const uint32_t _segId, const uint32_t _robotNr, const uint32_t _robotSize, int32_t &_lastCollisionRobot) const
{
    _lastCollisionRobot = -1;

    //Return if to less space
    int freeSpace = segmentSpace_[_segId];

    if(freeSpace < _robotSize)
    {
        return false;
    }

    for(const seg_occupation & occupation : timeline_[_segId])
    {
        if(occupation.robot != _robotNr)
        {
            if(_endTime == TIME_INFINITY || occupation.endTime == TIME_INFINITY)
            {
                if(_endTime == TIME_INFINITY && occupation.endTime == TIME_INFINITY)
                {
                    freeSpace -= occupation.spaceOccupied;
                    _lastCollisionRobot = occupation.robot;

                    if(freeSpace < _robotSize)
                    {
                        return false;
                    }
                }
                else if(_endTime == TIME_INFINITY && _startTime <= occupation.endTime)
                {
                    freeSpace -= occupation.spaceOccupied;
                    _lastCollisionRobot = occupation.robot;

                    if(freeSpace < _robotSize)
                    {
                        return false;
                    }
                }
                else if(occupation.endTime == TIME_INFINITY && _endTime >= occupation.startTime)
                {
                    freeSpace -= occupation.spaceOccupied;
                    _lastCollisionRobot = occupation.robot;

                    if(freeSpace < _robotSize)
                    {
                        return false;
                    }
                }
            }
            else
            {

                if((_startTime <= occupation.startTime && _endTime >= occupation.startTime) || (_startTime <= occupation.endTime && _endTime >= occupation.endTime))
                {
                    freeSpace -= occupation.spaceOccupied;
                    _lastCollisionRobot = occupation.robot;

                    if(freeSpace < _robotSize)
                    {
                        return false;
                    }
                }
                else if((_startTime >= occupation.startTime && _endTime <= occupation.endTime))
                {
                    freeSpace -= occupation.spaceOccupied;
                    _lastCollisionRobot = occupation.robot;

                    if(freeSpace < _robotSize)
                    {
                        return false;
                    }
                }
            }
        }
    }

    //No collision
    _lastCollisionRobot = -1;
    return true;
}

uint32_t RouteCoordinatorTimed::Timeline::getSize() const
{
    return maxTime_;
}


int32_t RouteCoordinatorTimed::Timeline::findSegId(const int32_t _robot, const uint32_t _timestep) const
{
    if(_robot < 0)
    {
        return -1;
    }

    for(const int & s : robotSegments_[_robot])
    {
        for(const seg_occupation & occupation : timeline_[s])
        {
            if(occupation.robot == _robot && _timestep >= occupation.startTime && _timestep <= occupation.endTime && occupation.mainSeg)
            {
                return s;
            }
        }
    }

    return -1;
}

int32_t RouteCoordinatorTimed::Timeline::getTimeUntilRobotOnSegment(const int32_t _robotNr, const uint32_t _segId) const
{
    int32_t ret = -2;

    for(const auto & occupation : timeline_[_segId])
    {
        if(occupation.robot == _robotNr)
        {
            ret = occupation.endTime;
        }
    }

    return ret;
}

std::vector< std::pair<uint32_t, float> > RouteCoordinatorTimed::Timeline::getListOfRobotsHigherPrioritizedRobots(const uint32_t _robot, const uint32_t _segId, const int32_t _potential) const
{
    std::vector<std::pair<uint32_t, float>> robots;

    seg_occupation occupationRobot(-1, -1, -1, -1);

    for(const auto & occupation : timeline_[_segId])
    {
        if(occupation.robot == _robot)
        {
            occupationRobot = occupation;
            break;
        }
    }

    if(occupationRobot.robot != _robot)
        return robots;    //TODO Error Should not happen


    for(const auto & occupation : timeline_[_segId])
    {
        if(occupation.robot != _robot && occupation.startTime < _potential)
        {
            if(segmentSpace_[_segId] - occupation.spaceOccupied < occupationRobot.spaceOccupied)
            {
                //Sort only if they would block each other
                robots.push_back( {occupation.robot, occupation.endTime});
            }
        }
    }

    return robots;
}


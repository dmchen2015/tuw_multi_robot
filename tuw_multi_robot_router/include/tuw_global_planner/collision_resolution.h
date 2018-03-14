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

#ifndef COLLISION_RESOLUTION_H
#define COLLISION_RESOLUTION_H

#include <tuw_global_planner/srr_utils.h>
#include <tuw_global_planner/route_coordinator.h>
#include <tuw_global_planner/potential_calculator.h>

class CollisionResolution
{
public:     CollisionResolution(std::shared_ptr<RouteCoordinator> _path_querry, std::shared_ptr<PotentialCalculator> _pCalc, int _timeoverlap) : path_querry_(_path_querry) , timeoverlap_(_timeoverlap), pCalc_(_pCalc) { }
public:     virtual std::vector<std::shared_ptr<Segment>> resolve(std::shared_ptr< Segment > _current, std::shared_ptr< Segment > _next, std::shared_ptr< Segment > _end, int _collision, int _robot_radius)=0;
public:		virtual void reset()=0; 

protected:   std::shared_ptr<RouteCoordinator> path_querry_;
protected:	 std::shared_ptr<PotentialCalculator> pCalc_;
protected:   int timeoverlap_;
};

#endif // HEURISTIC_H

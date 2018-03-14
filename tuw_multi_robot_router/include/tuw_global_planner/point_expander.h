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

#ifndef POINT_EXPANDER_H
#define POINT_EXPANDER_H

#define POT_HIGH 1.0e10
#include <algorithm>
#include <memory>
#include <queue>
#include <opencv/cv.h>
#include <unordered_set>
#include <map>
#include <eigen3/Eigen/Dense>

class PointExpander
{
    public:
        void initialize(const cv::Mat &_map);
        bool findGoalOnMap(const Eigen::Vector2d &_start, const uint32_t &_cycles, float* _potential, const std::map<uint32_t, Eigen::Vector2d> &_goals, const uint32_t &_optimizationSteps, Eigen::Vector2d &_foundPoint, int32_t &_segIdx, const uint32_t &_radius);
        float getDistanceToObstacle(const Eigen::Vector2d &vec);
    private:
        template <class T, class S, class C>
        void clearpq(std::priority_queue<T, S, C>& q)
        {
            q = std::priority_queue<T, S, C>();
        }
        class Index
        {
            public:

                Index(int index, float c, float d, float p)
                {
                    i = index;
                    weight = c;
                    dist = d;
                    potential = p;
                }

                Index(int x_val, int y_val, int nx, float c, float d, float p)
                {
                    i = x_val + y_val * nx;
                    weight = c;
                    dist = d;
                    potential = p;
                }

                Index offsetDist(int dist_x, int dist_y, int nx, int ny)
                {
                    int x_val = (i % nx) + dist_x;
                    int y_val = (i / nx) + dist_y;

                    if(x_val < 0 || x_val > nx || y_val < 0 || y_val > ny)
                        return Index(-1, -1, -1, -1);

                    return Index(x_val, y_val, nx, 0, 0, 0);
                }

                int getX(int nx)
                {
                    return (i % nx);
                }

                int getY(int nx)
                {
                    return (i / nx);
                }

                float distance(Index _p, int _nx)
                {
                    float dx = abs(getX(_nx) - _p.getX(_nx));
                    float dy = abs(getY(_nx) - _p.getY(_nx));

                    return std::sqrt(dx * dx + dy * dy);
                }

                int i;
                float weight;
                float dist;
                float cost;
                float potential;
        };

        struct greater1
        {
            bool operator()(const Index& a, const Index& b) const
            {
                return a.weight > b.weight;
            }
        };
        std::priority_queue<Index, std::vector<Index>, greater1> queue_;
        int nx_, ny_, ns_;
        Index findGoal(const Index &_start, const uint32_t &_cycles, float* _potential, const std::map<uint32_t, Index> &_goals, const uint32_t &_optimizationSteps, int32_t &segIdx, const uint32_t &_radius);
        void addPotentialExpansionCandidate(Index _current, int32_t _next_x, int32_t _next_y, float* _potential, uint32_t _distToObstacle);
        bool isGoal(Index _p, const std::map<uint32_t, Index> &_goals, int32_t &segIdx);

        cv::Mat distance_field_;
        float neutral_cost_ = 1;

};

#endif // VORONOI_EXPANDER_H


#ifndef pointgroup_H
#define pointgroup_H


#include <vector>
#include <map>
#include <string>


struct Point {

    std::string code;
    std::map<std::string, double> outlets;
    double dollars;
    double x{0}, y{0};
    double cluster = 0;

    void addOutlet(std::string name_, double dollars_) {
        if ( outlets.find(name_) == outlets.end() ) {
            outlets.insert({name_, dollars_});
        } 
        outlets.at(name_) += dollars_;
        dollars += dollars_;
    };

};


class PointGroup {

    public:

        double square(double value) {
            return value * value;
        }

        double squared_l2_distance(Point first, Point second) {
            return square(first.x - second.x) + square(first.y - second.y);
        }

        std::string name;
        std::map<std::string, Point> points;
        double max_share;
        int num_retailers;

        bool isReleasable(std::vector<Point> points_) {

             // find the total sales for all outlets
            double dollars = 0.0;
            for (auto point: points_) {
                dollars += point.dollars;
            }

            std::map<std::string, double> outletTotalsMap;

            // need to loop through the point objects
            // combine outlets with the same name
            for (auto point : points_) {
                for (auto const& x : point.outlets) {
                    if ( outletTotalsMap.find(x.first) == outletTotalsMap.end() ) {
                        outletTotalsMap.insert({x.first, x.second});
                    }
                    outletTotalsMap.at(x.first) += x.second;
                }
            }

            // compute the share within each outlet
            max_share = 0.0;
            num_retailers = 0;
            for (auto const& x : outletTotalsMap) {
                double outletShare = x.second/dollars;
                if(outletShare>max_share) {
                    max_share = outletShare;
                };
                num_retailers += 1;
            }

            return max_share<.75 && num_retailers>3 ? true: false;
        };

        void addData(std::string code_, std::string outlet_, double dollars_, double x_, double y_) {
           
           // is it a new point?
            if ( points.find(code_) == points.end() ) {

                Point point;
                point.x = x_;
                point.y = y_;
                point.code = code_;
                point.addOutlet(outlet_, dollars_);
                points.insert({code_, point});
            }

            // an existing point?
            else {

                points.at(code_).addOutlet(outlet_, dollars_);
            }

        }

        std::vector<Point> cluster(const std::vector<Point>& data, size_t k, size_t number_of_iterations) {

            static std::random_device seed;
            static std::mt19937 random_number_generator(seed());
            std::uniform_int_distribution<size_t> indices(0, data.size() - 1);

            std::vector<Point> newPoints;
            for (auto point : data) {
                newPoints.push_back(point);
            }
            
            std::vector<Point> means(k);
            for (auto& cluster : means) {
                cluster = data[indices(random_number_generator)];
            }

            std::vector<size_t> assignments(data.size());
            for (size_t iteration = 0; iteration < number_of_iterations; ++iteration) {
                for (size_t point = 0; point < data.size(); ++point) {
                double best_distance = std::numeric_limits<double>::max();
                    size_t best_cluster = 0;
                    for (size_t cluster = 0; cluster < k; ++cluster) {
                        const double distance = squared_l2_distance(data[point], means[cluster]);
                        if (distance < best_distance) {
                            best_distance = distance;
                            best_cluster = cluster;
                        }
                    }
                    assignments[point] = best_cluster;
                    newPoints[point].cluster = best_cluster;
                }
            };

            return newPoints;
        }

        std::vector<std::vector<Point>> split(std::vector<Point> pointsVec) {
            
             // a vector of point groups
            std::vector<std::vector<Point>> results;

            if(pointsVec.size()>1) {
                // create a new group with the clustered points
                std::vector<Point> newPoints;
                newPoints = cluster(pointsVec, 2, 5);

                // objects for each of the split groups
                std::vector<Point> points0;
                std::vector<Point> points1;

                // create two new groups from the clustered points
                for (auto point : newPoints) {
                    if(point.cluster==0) {
                        points0.push_back(point);
                    }
                    else {
                        points1.push_back(point);
                    }
                }

                // check if new groups are releasable
                if(isReleasable(points0) && isReleasable(points1)) {
                    results.push_back(points0);
                    results.push_back(points1);
                }
                else {
                    results.push_back(pointsVec);
                }
            }

            else {
                results.push_back(pointsVec);
            }

            return results;

        }


        std::vector<std::vector<Point>> split_groups(std::vector<std::vector<Point>> pointgroups) {

            std::vector<std::vector<Point>> finalgroups;
            std::vector<std::vector<Point>> groups;
            
            for (auto group : pointgroups) {
                groups = split(group);
                for (auto temp_group : groups) {
                    finalgroups.push_back(temp_group);
                }
            }
            return finalgroups;
        }


        std::vector<std::vector<Point>> optimize() {

            // convert points into a vector
            std::vector<Point> pointsVec;
            for (auto const& x : points) {    
                pointsVec.push_back(x.second);
            }

            std::vector<std::vector<Point>> pointGroups;
            pointGroups = split(pointsVec);

            int t1=pointGroups.size();
            int t2=pointGroups.size()+1;

            while(t2>t1) {
                t1 = pointGroups.size();
                pointGroups = split_groups(pointGroups);
                t2 = pointGroups.size();
                // std::cout<<t2<<","<<t1<<"\n";
            };

            return pointGroups;

        }

};


#endif
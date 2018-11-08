
#ifndef point_H
#define point_H

#include <string>
#include <map>


class Point {

    public:

        std::string code;
        std::map<std::string, double> outlets;
        double dollars;
        double x{0}, y{0};
        double cluster = 0;

        void addOutlet(std::string name_, double dollars_) {
            if ( outlets.find(name_) == outlets.end() ) {
                outlets.insert({name_, dollars_});
            } else {
                outlets.at(name_) += dollars_;
            }

            dollars += dollars_;
        
        };

};

#endif
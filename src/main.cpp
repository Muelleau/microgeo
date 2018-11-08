
#include <algorithm> 
#include <iostream>
#include <chrono>
#include <vector>
#include <functional>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <iterator>
#include <string>
#include <map>
#include <cstdlib>
#include "./pointgroup/pointgroup.hpp"


std::vector<std::string> line_split(const std::string& s, char delimiter){
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter)) {
      tokens.push_back(token);
   }
   return tokens;
};


std::map<std::string, PointGroup> initGroups(std::string datapath_) {

    std::map<std::string, PointGroup> groups;

    // map zipcode names to the zip code object
    std::map<std::string, Point> zipMap;

    std::string line;
    std::string dataFile = datapath_;
    std::ifstream inputData(dataFile);
    getline(inputData, line);
    for(std::string line; getline(inputData, line);) {

        // split the line into a vector
        std::vector<std::string> split_line = line_split(line, ',');

        // read the data
        std::string code = split_line[0];
        double dollars = std::stod(split_line[1]);
        std::string dma_name = split_line[2];
        std::string outlet_name = split_line[3];
        double latitude = std::stod(split_line[4]);
        double longitude = std::stod(split_line[5]);

        // if it is a new dma name, add it to the group map
        if ( groups.find(dma_name) == groups.end() ) { 
            PointGroup newGroup;
            newGroup.name = dma_name;
            groups.insert({dma_name, newGroup});
        }

        groups.at(dma_name).addData(code, outlet_name, dollars, latitude, longitude);

    };


    return groups;
};


int main(int argc, char **argv ) {

    std::string dataFile=argv[1];
    std::string outputFile=argv[2];

    std::map<std::string, PointGroup> groupMap = initGroups(dataFile);

    std::ofstream myfile;
    myfile.open (outputFile);
    myfile << "zip,microgeo,dma\n";

    int groupNum = 0;
    for (auto const& x : groupMap) {   
        std::vector<std::vector<Point>> pointGroups = groupMap.at(x.first).optimize();
        for (auto group : pointGroups) {
            for (auto point : group) {
                myfile << point.code << ", "<<groupNum << "," << x.first <<"\n";
            }
            groupNum += 1;
        }
    };


}

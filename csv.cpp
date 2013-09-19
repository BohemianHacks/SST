#include <iostream>
#include <string>
#include <vector>
#include <sstream>

std::vector <std::vector <std::string> > csvStringVector(std::string csv){
    std::stringstream csvStream(csv);
    std::string line;
    std::vector <std::vector <std::string> > csvVector;
    while(std::getline(csvStream, line)){
        std::vector <std::string> lineVector;
        lineVector.clear();
        while(line.find(',') != -1){
            std::string value;
            size_t qPos1 = line.find('"');
            size_t qPos2 = line.find('"', qPos1+1);
            size_t cPos1 = line.find(',');
            size_t cPos2 = line.find(',',cPos1+1);


            if ((qPos1 < cPos1) && (cPos1 < qPos2) && (qPos2 != -1)){
                value = line.substr(qPos1, qPos2+1);
                if (cPos2 != -1){
                    line.erase(0,qPos2+2);
                }
                else{
                    line.erase(0, line.length());
                }
            }
            else{
                value = line.substr(0, cPos1);
                line.erase(0, cPos1+1);
            }
            lineVector.push_back(value);
        }
        lineVector.push_back(line);
        csvVector.push_back(lineVector);
    }
    return(csvVector);
}

int main(){
    std::string testString = "\"a,s,s\",bat,cow,dog\nevil,\"f,ish\",goat,hawk";
    std::vector <std::vector <std::string> > response;
    std::cout << testString << std::endl;
    response = csvStringVector(testString);
    for (size_t i = 0; i < response.size(); i++){
        for (size_t j = 0; j < response[i].size(); j++){
            std::cout << response[i][j] << " ";
        }
        std::cout << std::endl;
    }
}


            

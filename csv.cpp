#include <vector>
#include <string>
#include <sstream>
#include <iostream>

class csvReader{
    private:
        std::vector <std::string> csvVector;
        size_t rows;
        size_t cols;
        size_t size;
    public:
        csvReader(std::string csvString);
        std::string cell(size_t row, size_t col){return(csvVector[col+row*cols]);};
        size_t getRows(){return(rows);};
        size_t getCols(){return(cols);};
        size_t getSize(){return(size);};
};


csvReader::csvReader(std::string csvString){
    std::stringstream csvStream(csvString);
    std::string line;
    size_t colTracker = 0;
    rows = 0;
    cols = 0;
    while(std::getline(csvStream, line)){
        rows++;
        while(line.find(',') != -1){
            colTracker++;
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
            csvVector.push_back(value);
        }
        csvVector.push_back(line);
        colTracker++;
        if (cols == 0){
           cols = colTracker;
        }
        colTracker = 0;
    }
    size = rows*cols;
}

int main(){
    csvReader csvTest("1,\"2,0\",3\n4,5,6\n\"7,9\",,9");
    for (size_t r = 0; r < csvTest.getRows(); r++){
        for (size_t c = 0; c < csvTest.getCols(); c++){
            std::cout << csvTest.cell(r,c);
            if (c < csvTest.getCols()-1){
                std::cout << ",";
            }
        }
        std::cout << std::endl;
    }
    return(0);
}

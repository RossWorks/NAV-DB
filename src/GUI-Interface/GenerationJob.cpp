#include "./GenerationJob.hpp"

GenerationJob::GenerationJob(std::string JobName, std::string Dbname,
                             std::string srcDir){
    this->IcdVers = 5;
    this->dbname = Dbname;
    this->srcpath = srcDir;
    this->name = JobName;
    this->isBigEndian = false;
}

GenerationJob::~GenerationJob(){
}

std::string GenerationJob::Getname(){
    return this->name;
}

std::string GenerationJob::ToString(){
    std::string output = this->name;
    output += " ; ";
    output.append(this->dbname);
    output += " ; ";
    output.append((this->isBigEndian ? "Little Endian" : "Big Endian"));
    output += " ; ";
    output.append(this->airacCycle);
    return output;
}
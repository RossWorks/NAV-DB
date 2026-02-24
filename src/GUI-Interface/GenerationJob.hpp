#ifndef GENERATIONJOB
#define GENERATIONJOB

#include <string>
#include <list>
#include <stdint.h>

class GenerationJob{
private:
    std::string name, dbname, airacCycle;
    uint32_t IcdVers;
    bool isBigEndian;
    std::string srcpath;
    std::list<std::string> AdmittedCountries;
public:
    GenerationJob(std::string JobName, std::string Dbname,
                  std::string srcDir);
    ~GenerationJob();
    std::string Getname();
    std::string ToString();
};

#endif //GENERATIONJOB
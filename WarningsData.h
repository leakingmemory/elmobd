//
// Created by sigsegv on 5/18/23.
//

#ifndef ELMOBD_WARNINGSDATA_H
#define ELMOBD_WARNINGSDATA_H

#include <mutex>
#include <vector>
#include <string>
#include <memory>

struct WarningsList {
    std::mutex mtx{};
    std::vector<std::string> messages{};
};

class WarningsData {
private:
    std::mutex mtx{};
    std::vector<std::shared_ptr<WarningsList>> messages{};
public:
    std::shared_ptr<WarningsList> Create();
    void Remove(std::shared_ptr<WarningsList> &list);
    std::vector<std::string> GetMessages();
};


#endif //ELMOBD_WARNINGSDATA_H

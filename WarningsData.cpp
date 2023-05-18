//
// Created by sigsegv on 5/18/23.
//

#include "WarningsData.h"

std::shared_ptr<WarningsList> WarningsData::Create() {
    std::shared_ptr<WarningsList> warningsList = std::make_shared<WarningsList>();
    std::lock_guard lock{mtx};
    this->messages.emplace_back(warningsList);
    return warningsList;
}

void WarningsData::Remove(std::shared_ptr<WarningsList> &list) {
    std::lock_guard lock{mtx};
    auto iterator = messages.begin();
    while (iterator != messages.end()) {
        if (*iterator == list) {
            messages.erase(iterator);
            return;
        }
        ++iterator;
    }
}

std::vector<std::string> WarningsData::GetMessages() {
    std::vector<std::shared_ptr<WarningsList>> lists{};
    {
        std::lock_guard lock{mtx};
        lists = messages;
    }
    std::vector<std::string> messages{};
    for (auto &list : lists) {
        std::lock_guard lock{list->mtx};
        for (const auto &msg : list->messages) {
            messages.emplace_back(msg);
        }
    }
    return messages;
}

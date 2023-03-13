#include "account.hpp"

const std::vector<QLabel>& Account::Info::labels() const {
    return NO_LABELS;
}

std::vector<QLabel> Account::Info::NO_LABELS;
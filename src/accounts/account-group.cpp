#include "account-group.hpp"

const std::string& AccountGroup::name() const {
    return name_;
}

QWidget& AccountGroup::info() const {
    return accountGroupInfoWidget;
}

const std::vector<const Account*>& AccountGroup::accounts() const {
    return accounts_;
}
typename std::vector<const Account*>::const_iterator AccountGroup::begin() const {
    return accounts_.begin();
}
typename std::vector<const Account*>::const_iterator AccountGroup::end() const {
    return accounts_.end();
}
typename std::vector<const Account*>::const_iterator AccountGroup::cbegin() const {
    return accounts_.cbegin();
}
typename std::vector<const Account*>::const_iterator AccountGroup::cend() const {
    return accounts_.cend();
}
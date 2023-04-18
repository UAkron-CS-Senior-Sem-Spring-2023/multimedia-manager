#include "account-group.hpp"

const std::string& AccountGroup::name() const {
    return name_;
}

QWidget& AccountGroup::info() const {
    return accountGroupInfoWidget;
}

const SetList<Account*>& AccountGroup::accounts() const {
    return accounts_;
}
typename SetList<Account*>::const_iterator AccountGroup::begin() const {
    return accounts_.begin();
}
typename SetList<Account*>::const_iterator AccountGroup::end() const {
    return accounts_.end();
}
typename SetList<Account*>::const_iterator AccountGroup::cbegin() const {
    return accounts_.cbegin();
}
typename SetList<Account*>::const_iterator AccountGroup::cend() const {
    return accounts_.cend();
}

std::size_t AccountGroup::count(Account* account) const {
    return accounts_.count(account);
}

void AccountGroup::erase(Account* account) {
    accounts_.erase(account);
}
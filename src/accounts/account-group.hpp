#pragma once

#include <string>
#include <vector>

#include "account.hpp"
#include "set-list.hpp"

class AccountGroup {
    public:
        template <class AccountPtrIterator>
        AccountGroup(AccountPtrIterator begin, AccountPtrIterator end) {
            for (; begin != end; ++begin) {
                accounts_.push_back(*begin);
            }
            name_ = std::string("Unnamed group of ") + std::to_string(accounts_.size()) + " accounts";
        }
        template <class AccountPtrContainer>
        AccountGroup(const AccountPtrContainer& container)
            : AccountGroup(container.begin(), container.end())
        {}

        const std::string& name() const;

        QWidget& info() const;

        const SetList<Account*>& accounts() const;
        typename SetList<Account*>::const_iterator begin() const;
        typename SetList<Account*>::const_iterator end() const;
        typename SetList<Account*>::const_iterator cbegin() const;
        typename SetList<Account*>::const_iterator cend() const;

        std::size_t count(Account* account) const;
        void erase(Account* account);
    private:
        std::string name_;

        mutable QWidget accountGroupInfoWidget;

        SetList<Account*> accounts_;
};

#pragma once

#include <string>
#include <vector>

#include "account.hpp"

class AccountGroup {
    public:
        template <class ConstAccountPtrIterator>
        AccountGroup(ConstAccountPtrIterator begin, ConstAccountPtrIterator end) {
            for (; begin != end; ++begin) {
                accounts_.push_back(*begin);
            }
            name_ = std::string("Unnamed group of ") + std::to_string(accounts_.size()) + " accounts";
        }
        template <class ConstAccountPtrContainer>
        AccountGroup(const ConstAccountPtrContainer& container)
            : AccountGroup(container.begin(), container.end())
        {}

        const std::string& name() const;

        QWidget& info() const;

        const std::vector<const Account*>& accounts() const;
        typename std::vector<const Account*>::const_iterator begin() const;
        typename std::vector<const Account*>::const_iterator end() const;
        typename std::vector<const Account*>::const_iterator cbegin() const;
        typename std::vector<const Account*>::const_iterator cend() const;
    private:
        std::string name_;

        mutable QWidget accountGroupInfoWidget;

        std::vector<const Account*> accounts_;
};

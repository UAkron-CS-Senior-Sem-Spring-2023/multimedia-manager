#pragma once

#include <string>
#include <vector>

#include "account.hpp"

class AccountGroup {
    public:
        template <class ConstAccountPtrContainer>
        AccountGroup(ConstAccountPtrContainer&& container)
            : accounts_(std::move(container))
        {}

        template <class ConstAccountPtrIterator>
        AccountGroup(ConstAccountPtrIterator begin, ConstAccountPtrIterator end) {
            for (; begin != end; ++begin) {
                accounts_.push_back(*begin);
            }
        }

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

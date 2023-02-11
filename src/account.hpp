#pragma once

class Account {
    public:
        virtual ~Account() = default;
        virtual const AccountInfo& accountInfo() const; 
}
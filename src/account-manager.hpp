#pragma once

class AccountManager {
    public:
        enum ManagingType {
            ACCOUNTS,
            ACCOUNT_GROUPS
        };
        static ManagingType INITIAL_MANAGING_TYPE;
};
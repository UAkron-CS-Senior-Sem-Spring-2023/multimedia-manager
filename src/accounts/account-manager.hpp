#pragma once

#include <list>
#include <memory>
#include <unordered_set>

#include <QObject>

#include "account.hpp"
#include "account-group.hpp"

class AccountManager : public QObject {
    Q_OBJECT

    public:
        enum ManagingType {
            ACCOUNTS,
            ACCOUNT_GROUPS
        };
        static ManagingType INITIAL_MANAGING_TYPE;

        enum SourceRecipientType {
            SOURCE,
            RECIPIENT,
            DUAL,
            NONE
        };

        enum AccountVendor {
            GMAIL
        };

        void addSourceAccount(std::unique_ptr<SourceAccount> sourceAccount);
        void addRecipientAccount(std::unique_ptr<RecipientAccount> recipientAccount);
        void addDualAccount(std::unique_ptr<DualAccount> dualAccount);
        void addAccountGroup(std::unique_ptr<AccountGroup> accountGroup);

        SourceRecipientType accountSourceRecipientType(const Account* account) const;

    signals:
        void onAccountAdded(const Account* account);
        void onAccountsChanged(const std::unordered_set<Account*>& accounts);
        void onSourceAccountAdded(const SourceAccount* sourceAccount);
        void onSourceAccountsChanged(const std::unordered_set<SourceAccount*>& sourceAccounts);
        void onRecipientAccountAdded(const RecipientAccount* recipientAccount);
        void onRecipientAccountsChanged(const std::unordered_set<RecipientAccount*>& recipientAccounts);
        void onDualAccountAdded(const DualAccount* dualAccount);
        void onDualAccountsChanged(const std::unordered_set<DualAccount*>& dualAccounts);
        void onAccountGroupAdded(const AccountGroup* accountGroup);
        void onAccountGroupsChanged(const std::unordered_set<AccountGroup*>& accountGroups);
    private:
        std::list<std::unique_ptr<Account>> accountsOwned;
        std::unordered_map<const Account*, SourceRecipientType> accountSourceRecipientTypes_;
        std::unordered_set<Account*> accounts_;
        std::unordered_set<SourceAccount*> sourceAccounts_;
        std::unordered_set<RecipientAccount*> recipientAccounts_;
        std::unordered_set<DualAccount*> dualAccounts_;

        std::list<std::unique_ptr<AccountGroup>> accountGroupsOwned;
        std::unordered_set<AccountGroup*> accountGroups_;
        
};
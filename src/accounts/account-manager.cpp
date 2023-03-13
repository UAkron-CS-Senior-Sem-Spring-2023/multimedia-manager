#include "account-manager.hpp"

AccountManager::ManagingType AccountManager::INITIAL_MANAGING_TYPE = ACCOUNTS;

std::unique_ptr<AccountManager> AccountManager::singleton_;
AccountManager& AccountManager::singleton() {
    if (!singleton_) {
        singleton_ = std::make_unique<AccountManager>();
    }
    return *singleton_;
}

void AccountManager::addSourceAccount(std::unique_ptr<SourceAccount> sourceAccount) {
    SourceAccount* sourceAccountPtr = sourceAccount.get();
    accounts_.insert(sourceAccountPtr);
    accountSourceRecipientTypes_[sourceAccountPtr] = SOURCE;
    sourceAccounts_.insert(sourceAccountPtr);
    accountsOwned.push_back(std::unique_ptr<Account>(std::move(sourceAccount)));
    onAccountAdded(sourceAccountPtr);
    onSourceAccountAdded(sourceAccountPtr);
    onAccountsChanged(accounts_);
    onSourceAccountsChanged(sourceAccounts_);
}

void AccountManager::addRecipientAccount(std::unique_ptr<RecipientAccount> recipientAccount) {
    RecipientAccount* recipientAccountPtr = recipientAccount.get();
    accounts_.insert(recipientAccountPtr);
    accountSourceRecipientTypes_[recipientAccountPtr] = RECIPIENT;
    recipientAccounts_.insert(recipientAccountPtr);
    accountsOwned.push_back(std::unique_ptr<Account>(std::move(recipientAccount)));
    onAccountAdded(recipientAccountPtr);
    onRecipientAccountAdded(recipientAccountPtr);
    onAccountsChanged(accounts_);
    onRecipientAccountsChanged(recipientAccounts_);
}

void AccountManager::addDualAccount(std::unique_ptr<DualAccount> dualAccount) {
    DualAccount* dualAccountPtr = dualAccount.get();
    accounts_.insert(dualAccountPtr);
    accountSourceRecipientTypes_[dualAccountPtr] = DUAL;
    sourceAccounts_.insert(dualAccountPtr);
    recipientAccounts_.insert(dualAccountPtr);
    dualAccounts_.insert(dualAccountPtr);
    accountsOwned.push_back(std::unique_ptr<Account>(std::move(dualAccount)));
    onAccountAdded(dualAccountPtr);
    onSourceAccountAdded(dualAccountPtr);
    onRecipientAccountAdded(dualAccountPtr);
    onDualAccountAdded(dualAccountPtr);
    onAccountsChanged(accounts_);
    onSourceAccountsChanged(sourceAccounts_);
    onRecipientAccountsChanged(recipientAccounts_);
    onDualAccountsChanged(dualAccounts_);
}

void AccountManager::addAccountGroup(std::unique_ptr<AccountGroup> accountGroup) {
    AccountGroup* accountGroupPtr = accountGroup.get();
    accountGroups_.insert(accountGroupPtr);
    accountGroupsOwned.push_back(std::move(accountGroup));
    onAccountGroupAdded(accountGroupPtr);
    onAccountGroupsChanged(accountGroups_);
}

AccountManager::SourceRecipientType AccountManager::accountSourceRecipientType(const Account* account) const {
    return accountSourceRecipientTypes_.at(account);
}
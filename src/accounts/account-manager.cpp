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
    accounts_.push_back(sourceAccountPtr);
    accountSourceRecipientTypes_[sourceAccountPtr] = SOURCE;
    sourceAccounts_.push_back(sourceAccountPtr);
    accountsOwned.push_back(std::unique_ptr<Account>(std::move(sourceAccount)));
    emit onAccountAdded(sourceAccountPtr);
    emit onSourceAccountAdded(sourceAccountPtr);
    emit onAccountsChanged(accounts_);
    emit onSourceAccountsChanged(sourceAccounts_);
}

void AccountManager::addRecipientAccount(std::unique_ptr<RecipientAccount> recipientAccount) {
    RecipientAccount* recipientAccountPtr = recipientAccount.get();
    accounts_.push_back(recipientAccountPtr);
    accountSourceRecipientTypes_[recipientAccountPtr] = RECIPIENT;
    recipientAccounts_.push_back(recipientAccountPtr);
    accountsOwned.push_back(std::unique_ptr<Account>(std::move(recipientAccount)));
    emit onAccountAdded(recipientAccountPtr);
    emit onRecipientAccountAdded(recipientAccountPtr);
    emit onAccountsChanged(accounts_);
    emit onRecipientAccountsChanged(recipientAccounts_);
}

void AccountManager::addDualAccount(std::unique_ptr<DualAccount> dualAccount) {
    DualAccount* dualAccountPtr = dualAccount.get();
    accounts_.push_back(dualAccountPtr);
    accountSourceRecipientTypes_[dualAccountPtr] = DUAL;
    sourceAccounts_.push_back(dualAccountPtr);
    recipientAccounts_.push_back(dualAccountPtr);
    dualAccounts_.push_back(dualAccountPtr);
    accountsOwned.push_back(std::unique_ptr<Account>(std::move(dualAccount)));
    emit onAccountAdded(dualAccountPtr);
    emit onSourceAccountAdded(dualAccountPtr);
    emit onRecipientAccountAdded(dualAccountPtr);
    emit onDualAccountAdded(dualAccountPtr);
    emit onAccountsChanged(accounts_);
    emit onSourceAccountsChanged(sourceAccounts_);
    emit onRecipientAccountsChanged(recipientAccounts_);
    emit onDualAccountsChanged(dualAccounts_);
}

void AccountManager::addAccountGroup(std::unique_ptr<AccountGroup> accountGroup) {
    AccountGroup* accountGroupPtr = accountGroup.get();
    accountGroups_.push_back(accountGroupPtr);
    accountGroupsOwned.push_back(std::move(accountGroup));
    emit onAccountGroupAdded(accountGroupPtr);
    emit onAccountGroupsChanged(accountGroups_);
}

AccountManager::SourceRecipientType AccountManager::accountSourceRecipientType(const Account* account) const {
    return accountSourceRecipientTypes_.at(account);
}

const SetList<Account*>& AccountManager::accounts() const {
    return accounts_;
}
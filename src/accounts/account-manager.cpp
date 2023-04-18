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
    accountsOwnedLocations[accountsOwned.back().get()] = std::prev(accountsOwned.end());
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
    accountsOwnedLocations[accountsOwned.back().get()] = std::prev(accountsOwned.end());
    emit onAccountAdded(recipientAccountPtr);
    emit onRecipientAccountAdded(recipientAccountPtr);
    emit onAccountsChanged(accounts_);
    emit onRecipientAccountsChanged(recipientAccounts_);
}

void AccountManager::addDualAccount(std::unique_ptr<DualAccount> dualAccount) {
    DualAccount* dualAccountPtr = dualAccount.get();
    accounts_.push_back(dualAccountPtr);
    accountSourceRecipientTypes_[dualAccountPtr] = DUAL;
    dualAccounts_.push_back(dualAccountPtr);
    accountsOwned.push_back(std::unique_ptr<Account>(std::move(dualAccount)));
    accountsOwnedLocations[accountsOwned.back().get()] = std::prev(accountsOwned.end());
    emit onAccountAdded(dualAccountPtr);
    emit onDualAccountAdded(dualAccountPtr);
    emit onAccountsChanged(accounts_);
    emit onDualAccountsChanged(dualAccounts_);
}

void AccountManager::addAccountGroup(std::unique_ptr<AccountGroup> accountGroup) {
    AccountGroup* accountGroupPtr = accountGroup.get();
    accountGroups_.push_back(accountGroupPtr);
    accountGroupsOwned.push_back(std::move(accountGroup));
    accountGroupsOwnedLocations[accountGroupsOwned.back().get()] = std::prev(accountGroupsOwned.end());
    emit onAccountGroupAdded(accountGroupPtr);
    emit onAccountGroupsChanged(accountGroups_);
}

void AccountManager::removeAccount(Account* account) {
    for (auto* accountGroup : accountGroups_) {
        if (accountGroup->count(account) != 0) {
            accountGroup->erase(account);
        }
    }
    auto sourceRecipientType = accountSourceRecipientType(account);
    if (sourceRecipientType == DUAL) {
        dualAccounts_.erase(dynamic_cast<DualAccount*>(account));
        emit onDualAccountsChanged(dualAccounts_);
    }
    if (sourceRecipientType == SOURCE) {
        sourceAccounts_.erase(dynamic_cast<SourceAccount*>(account));
        emit onSourceAccountsChanged(sourceAccounts_);
    }
    if (sourceRecipientType == RECIPIENT) {
        recipientAccounts_.erase(dynamic_cast<RecipientAccount*>(account));
        emit onRecipientAccountsChanged(recipientAccounts_);
    }
    accounts_.erase(account);
    auto listIt = accountsOwnedLocations.at(account);
    accountsOwnedLocations.erase(account);
    accountsOwned.erase(listIt);
    emit onAccountsChanged(accounts_);
}

void AccountManager::removeAccountGroup(AccountGroup* accountGroup) {
    accountGroups_.erase(accountGroup);
    auto listIt = accountGroupsOwnedLocations.at(accountGroup);
    accountGroupsOwnedLocations.erase(accountGroup);
    accountGroupsOwned.erase(listIt);
    emit onAccountGroupsChanged(accountGroups_);
}

AccountManager::SourceRecipientType AccountManager::accountSourceRecipientType(const Account* account) const {
    return accountSourceRecipientTypes_.at(account);
}

const SetList<Account*>& AccountManager::accounts() const {
    return accounts_;
}
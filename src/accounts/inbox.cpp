#include "inbox.hpp"

Inbox::Inbox(QObject* parent)
    : QObject(parent)
{}

bool operator<(const Inbox::Info& infoLHS, const Inbox::Info& infoRHS) {
    if (infoLHS.time == infoRHS.time) {
        return infoLHS.uniqueId < infoRHS.uniqueId;
    }
    return infoLHS.time < infoRHS.time;
}
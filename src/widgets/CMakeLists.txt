target_sources(MultimediaManager
    PUBLIC main-window.cpp
    PUBLIC accounts-widget.cpp
    PUBLIC account-info-widget.cpp
    PUBLIC get-email-recipients-wizard.cpp
    PUBLIC send-message-wizard.cpp
    PUBLIC view-inbox-dialog.cpp

    PUBLIC add-account-group-wizard.cpp
)

add_subdirectory(add-account-wizard)
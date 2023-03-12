target_sources(MultimediaManager
    PUBLIC main.cpp
    PUBLIC dynamic-assert.cpp
    PUBLIC request.cpp
    PUBLIC q-request.cpp
    PUBLIC customizable-get-messaging-recipient.cpp
    PUBLIC customizable-post-messaging-recipient.cpp
)

add_subdirectory(accounts)
add_subdirectory(widgets)
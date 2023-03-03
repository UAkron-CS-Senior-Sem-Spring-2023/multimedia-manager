target_sources(MultimediaManager
    PUBLIC main.cpp
    PUBLIC dynamic-assert.cpp
    PUBLIC media.cpp
    PUBLIC message.cpp
    PUBLIC request.cpp
    PUBLIC messaging-recipient.cpp
    PUBLIC customizable-get-messaging-recipient.cpp
    PUBLIC customizable-post-messaging-recipient.cpp
    PUBLIC google-gateway.cpp
    
    PUBLIC main-window.cpp
)

QT6_WRAP_CPP(
    main-window.cpp
)
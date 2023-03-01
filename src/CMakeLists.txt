target_sources(MultimediaManager
    PUBLIC main.cpp
    PUBLIC dynamic-assert.cpp
    PUBLIC media.cpp
    PUBLIC message.cpp
    PUBLIC request.cpp
    
    PUBLIC main-window.cpp
)

QT6_WRAP_CPP(
    main-window.cpp
)
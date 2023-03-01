from alpine

CMD ./MultimediaManager

RUN apk add cmake curl-dev make g++ qt6-qtbase-dev font-misc-misc

RUN mkdir -p /app/build
WORKDIR /app/build
ADD . /app

RUN cmake .. && make
from alpine

CMD ./MultimediaManager

RUN apk add cmake make g++ qt6-qtbase-dev font-misc-misc
RUN mkdir build
WORKDIR build
ADD . /

RUN cmake .. && make
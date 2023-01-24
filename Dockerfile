from alpine

CMD ./MultimediaManager

RUN apk add cmake make g++
RUN mkdir build
WORKDIR build
ADD . /

RUN cmake .. && make
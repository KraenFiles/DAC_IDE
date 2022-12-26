FROM alpine
RUN apk update && apk add gcc g++ make
WORKDIR /home/

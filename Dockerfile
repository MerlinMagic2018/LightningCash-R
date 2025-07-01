FROM ubuntu:18.04

RUN apt-get update

RUN apt-get install -y git wget nano

RUN apt-get install -y software-properties-common

RUN apt-get install -y libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils python3

RUN apt-get install -y libboost-system-dev libboost-filesystem-dev libboost-chrono1.65-dev libboost-program-options-dev libboost-thread-dev

RUN add-apt-repository -y ppa:luke-jr/bitcoincore

RUN apt-get update

RUN apt-get install -y libdb4.8-dev libdb4.8++-dev

RUN apt-get install -y libminiupnpc-dev

RUN git clone https://github.com/MerlinMagic2018/LightningCash-R

WORKDIR /LightningCash-R
RUN ./autogen.sh

RUN ./configure --without-gui --disable-tests --disable-bench --disable-zmq

RUN make

RUN make install

RUN mkdir /root/.lightningcashr

RUN cp lightningcashr.conf /root/.lightningcashr/lightningcashr.conf

EXPOSE 4466 6644

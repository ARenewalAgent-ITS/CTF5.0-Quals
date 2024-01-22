# export DOCKER_DEFAULT_PLATFORM=linux/amd64
# docker build -t pwn-baby .
# docker run -d -it --rm -p 10023:1337 pwn-baby
FROM ubuntu:18.04
RUN apt-get update -y && apt-get install -y \
    lib32z1 xinetd openssl libgmp-dev libssl1.0-dev python3 perl \
 && rm -rf /var/lib/apt/lists/*
 
RUN useradd pwnuser
RUN mkdir /pwn

RUN echo "You've been blocked by our server,try again, and report if this repeats." > /etc/banner_fail

COPY ./ctf.xinetd /etc/xinetd.d/pwn
COPY ./start.sh /start.sh
COPY ./setup.sh /setup.sh
COPY ./chall /pwn/
COPY ./flag.txt /flag-b527042ee28f02aa753932.txt

RUN chown -R root:pwnuser /pwn && chmod -R 750 /pwn
RUN chmod +x /setup.sh
RUN chown root:pwnuser /start.sh && chmod 750 /start.sh

CMD ["/setup.sh"]

EXPOSE 1337
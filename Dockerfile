FROM lonkidely/hl-build-image

ADD . /app

COPY httptest /var/www/html/httptest

WORKDIR /app

EXPOSE 8089

RUN make build

CMD make run-docker

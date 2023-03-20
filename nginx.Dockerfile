FROM nginx

COPY configs/nginx.conf /etc/nginx/nginx.conf

COPY  httptest /var/www/html/httptest

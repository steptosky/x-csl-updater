FROM nginx:latest
LABEL maintainer="steptosky.com"

COPY nginx.conf /etc/nginx/nginx.conf
COPY site.conf /etc/nginx/sites/site.conf
COPY www/ /var/www/

# custom entrypoint
COPY my-entrypoint.sh /usr/local/bin/
RUN chmod +x /usr/local/bin/my-entrypoint.sh \
    && chown -R www-data:www-data /var/www
ENTRYPOINT ["/usr/local/bin/my-entrypoint.sh"]
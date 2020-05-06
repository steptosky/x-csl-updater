#!/bin/bash
chown -R www-data:www-data /var/www

exec nginx -g "daemon off;"

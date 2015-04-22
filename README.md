# rawRevelador
Raw to 16bits Histogram

### Instalation

Download the repository as .zip and extract in your Linux server.

Give the folder permissions to be executed as www-data:

```sh

chown www-data <folder> -R
chmod 775 <folder> -R

```

Edit php.ini to allow upload big filesize:

```sh

post_max_size= 200M
upload_max_size = 200M

```


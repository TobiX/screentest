#!/bin/dash
rsync --stats --progress --delete-after --recursive --exclude='.*' --exclude=_darcs --exclude=update.sh . tobig@shell.sf.net:/home/groups/s/sc/screentest/htdocs

#!/bin/dash
rsync --stats --progress --delete-after --recursive --exclude='.*' --exclude=_darcs --exclude=update.sh . tobig,screentest@web.sourceforge.net:htdocs/

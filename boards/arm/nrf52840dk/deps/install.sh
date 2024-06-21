#!/bin/env bash

SCRIPT_DIR=$(dirname "$(readlink -f "${BASH_SOURCE[0]}")")
URL=$(sed -n '1p' "${SCRIPT_DIR}/tools.dep")
TYPE=$(sed -n '2p' "${SCRIPT_DIR}/tools.dep")
CROSSCOMPILETOOL_URL="${URL}${TYPE}"
FILE_NAME=$(basename "${CROSSCOMPILETOOL_URL}")

sudo apt-get install wget
wget "${CROSSCOMPILETOOL_URL}"
#echo "$TYPE"

case "$TYPE" in
    ".tar.bz2")
        echo "Extracting $FILE_NAME using tar (bzip2)"
        tar -xjf "$FILE_NAME"
        ;;
    ".tar.gz")
        echo "Extracting $FILE_NAME using tar (gzip)"
        tar -xzf "$FILE_NAME"
        ;;
    ".zip")
        echo "Extracting $FILE_NAME using unzip"
        unzip "$FILE_NAME"
        ;;
    ".rar")
        echo "Extracting $FILE_NAME using unrar"
        unrar x "$FILE_NAME"
        ;;
    *)
        echo "Unsupported file type: $TYPE"
        ;;
esac


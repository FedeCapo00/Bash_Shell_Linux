#!/usr/bin/bash

set -x
# Sopra, enable the debugging mode

#echo "Today is " `date`

#echo -e "\nEnter the path to directory"
#read the_path

# Utilizzo delle variabili:
#echo -e "\n you path has the following files and folders: "
#ls $the_path

# Per convertire in caso che il file sia stato creato su windows:
# dos2unix path/to/script.sh

# cout/cin :
#echo "Insert the name of the file to be opened: "
#read file_name

# $1 per prendere il primo argomento

# Ciclo while:
#while IFS= read -r line
#do
#    echo "Line: $line"
#done < "$1"

# Se voglio eliminare qualcosa che ha permessi speciali: 
#sudo rm importante.txt
# Elimino i file oggetto e i vari eseguibili
rm -rf program

# -a AND
# -o OR
# -z NULL
# -d test se il nome fornito esiste e se e' una directory
# -e testa sia directory che file
# -n ritorna TRUE se la lunghezza di una stringa non e' 0
# if a > 60 && b < 100
# if [ $a -gt 60 -a $b -lt 100 ]

# >&1 output sullo stdout
# >&2 output sullo stderr

# If the command is: bash run.sh info.txt 3
# Compile with the input arguments
if [[ -z $3 ]]
then
  # Check if the $1 is a file
  if [ -e "$1" ]; then
    # Check if the $1 is a dir
    if [ -d "$1" ]; then
      rmdir "$1"  # Remove the directory if it exists
    else
      rm "$1"  # Remove the file if it exists
    fi
  else
    mkdir -p "$(dirname "$1")"  # Create the directory if it does not exist
  fi

  # Compile
  gcc -Wall -o program main.c

  # Execute
  ./program "$1" "$2"

  # Cout the target file
  cat "$1"
else
  # If the command is: bash run.sh prova info.txt 3

  # Check if the directory/file exists
  if [ -e "$2" ]; then
    if [ -d "$2" ]; then
      rmdir "$2"  # Remove the directory if it exists
    else
      rm "$2"  # Remove the file if it exists
    fi
  else
    mkdir -p "$(dirname "$2")"  # Create the directory if it does not exist
  fi

  # Compile
  gcc -Wall -o "$1" main.c

  # Execute
  ./"$1" "$2" "$3"

  # Cout the target file
  cat "$2"
fi
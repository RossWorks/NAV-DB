#!/bin/bash

MAKER=gprbuild
CLEANER=gprclean
MISSING_GPR="Missing $MAKER: install from AdaCore!"
PROJECT_FILE=NavDb.gpr
PDFLATEX=pdflatex
LOGS_DIR=./Logs/

if !(command -v $MAKER > /dev/null) then
  echo $MISSING_GPR
  exit 1
fi

echo Cleaning environment
$CLEANER

echo Building documentation
for doc in ./src/documentation/*.tex; do
  if [ ! -d "Docs" ]; then
    mkdir Docs
  fi
   $PDFLATEX -output-directory=Docs $doc > $LOGS_DIR`basename $doc`.log
done
rm ./Docs/*.aux; rm ./Docs/*.log; rm ./Docs/*.toc

echo Start compilation
$MAKER -d -p -q -P $PROJECT_FILE -XMode=debug -XInterface=GUI

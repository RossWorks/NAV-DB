#!/bin/bash

MAKER=gprbuild
CLEANER=gprclean
MISSING_GPR="Missing $MAKER: install from AdaCore!"
PROJECT_FILE=NavDb.gpr
PDFLATEX=pdflatex

if !(command -v $MAKER > /dev/null) then
  echo $MISSING_GPR
  exit 1
else
  echo $MAKER found. Begin building
fi

echo Cleaning environment
$CLEANER

echo Building documentation
for doc in ./src/documentation/*.tex; do
  if [ ! -d "Docs" ]; then
    mkdir Docs
  fi
   $PDFLATEX -output-directory=Docs $doc
done
rm ./Docs/*.aux; rm ./Docs/*.log; rm ./Docs/*.toc

exit 1
echo Start compilation
$MAKER -p -P $PROJECT_FILE -XMode=debug -XInterface=GUI

cd ~/Downloads
for FILE in `ls arinc*`;
do A424Region=`basename $FILE .zip`
echo "extracting "  $FILE;
7zz x $FILE -o./Arinc/; #Using 7-zip to extract the zip downloaded from selenium
mv -f ./Arinc/$A424Region/isolated/$A424Region.pc ~/Code/RegNav/NAV-DB/Data/$A424Region.pc; #moving the new A424 files onto Nav-Db
done
rm ./Arinc -R #deleting Arinc folder used as temp storage
rm -f ~/Downloads/arinc*.zip #delete downloaded arinc archives
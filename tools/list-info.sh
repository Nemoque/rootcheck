#!/bin/bash
system_apppath=$(pwd)/app
touch report_apk
output_file=`ls report_apk`

if [ -d $system_apppath ];then
    echo -e "The path needs to be recusive is:$system_apppath\n"
else
    echo -e "no such directory or files\n"
    exit 1
fi

let index=0
for apk in `ls $system_apppath|grep '[A-Z]*[a-z]*.apk'`;do
    apk_files[$index]=$apk
   # echo -e "The current apkname is:${apk_files[$index]} \n"
    let index++
done

if [ 0 -lt ${#apk_files[@]} ];then
   echo -e "got file in this directory\n"
else
   echo -e "no file in this directory\n"
   
fi

chmod u+rwx $output_file
if [ -n $output_file ];then
   echo -e "The report file is not clean\n"
   echo "">$output_file
   
fi

apk_num=$(ls -l $system_apppath|grep '^-'|wc -l)
echo -e "There are $apk_num apk files in this directory\n" >> $output_file

for element in ${apk_files[@]}
do
    echo -e "element in the array is:$element \n"
    if [ -f `ls ${system_apppath}/${element}` ];then
      temp=`echo -e $element`
      echo -e $temp>>$output_file
#      sed -i '/'$element'/N;s/\n//' $output_file 
#      echo -e '\t' >>$output_file 
      echo -e `openssl dgst -md5 ${system_apppath}/$element|sed s/"MD5.*\=\ "//g`>>$output_file
      echo -e '' >>$output_file
    fi
done
       
sed -i '/.*apk.*/N;s/\n/\t/g' $output_file 

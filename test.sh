
int=1
echo $(date "+%H:%M:%S")
str="";
while(( $int<10))
do
	
	str3="${str}a${int},b${int},c${int},d${int},e${int},f${int},g${int},h${int},i${int},j${int}\n";
	str=$str3;
    let "int++";
    
done
echo -e $str >>./test.csv
echo $(date "+%H:%M:%S")

str=$1
ans=`echo $str | wc -c ` 
echo $((ans -1))

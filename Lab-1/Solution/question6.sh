for i; do 
   if [ ! -f $i ]
   then 
   	echo "Filename $i doesn't exist"
   	exit 0
   fi
   
   tr '[a-z]' '[A-Z]' < $i
done

var=(`expr $3 + $2 - 1`)

cut -c $2-$var <<< $1

#accepting input of filename and reading it in variable file
#using replace command of sed s
#replace ^. to ' '(^. represents the first character after a new line)
#replace .$ to ' '(.$ represents the last character in a line)
sed 's/^.//;s/.$//' $1

	

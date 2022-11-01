double a[1000][1000];
void a_input_array(FILE *file){
    fscanf(file,"%d %d",&an,&am);
    int id=0;
    int i=1;
    char line[500];   


    fgets(line,sizeof(line),file) ;
    fgets(line,sizeof(line),file) ;
	
    while (fscanf(file,"%lf",&a[id/am][id%am])) {
        if(id==(an)*(am))
            break;

        id++;
        i++;
        if(i%((am)+1)==0){
            i++;
            fgets(line,sizeof(line),file) ;
            fgets(line,sizeof(line),file) ;
        }
    }
}
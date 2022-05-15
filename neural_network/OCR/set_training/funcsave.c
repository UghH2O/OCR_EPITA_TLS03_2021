void maketrain(double matrix[])
{
	char filename ='a';
	int ret = -1;
	while(ret <0 || filename == '\n')
		ret = scanf("%c",&filename);
	char c[5+14] = {filename, '.', 't', 'x', 't'}; 
	FILE *fichier = fopen(c,"w" );
    if (fichier == NULL) printf("Erreur lors de la création du fichier" );
    for (int x = 0; x < 30; x++)
    { 
        for (int y = 0; y < 30; y++)
        {
			fprintf(fichier, "%.0f", matrix[x * 30 + y]);
			fprintf(fichier, ",");
        } 
    }
	fclose(fichier);
}
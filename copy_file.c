void copyAll(FILE *inputFile, FILE *outputFile){
    int ch ;

    while ((ch = fgetc(inputFile)) != EOF) {
        fputc(ch, outputFile);
    }
    printf("Data successfully written.\n");
}

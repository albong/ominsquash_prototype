#ifndef FILE_READER_H
#define FILE_READER_H

char *readFileToString(char *filename);
unsigned char *readBinaryFileToCharStar(char *filename, unsigned long *length);

#endif

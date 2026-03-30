#include <iostream>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

typedef struct Reader_t {
  string gamePath;
} Reader;

typedef Reader *PReader;

PReader path_Create(string filePath) {
  FILE *fd = fopen(&filePath[0], "r");
  if(!fd) {
    return NULL;
  }
  string path = "";
  PReader response = (PReader)malloc(sizeof(Reader));
  char chr;
  while(fscanf(fd, "%c", &chr) != EOF) {
    path += chr;
  }
  response->gamePath = path;
  return response;
}
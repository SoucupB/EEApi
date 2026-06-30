#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <cstdint>

using namespace std;

typedef struct Reader_t {
  string *configPath;
  string *gamePath;
  string *dllPath;
  string *gameName;
  string *dllName;
} Reader;

typedef Reader *PReader;

uint8_t path_Parse(PReader reader);
void path_PrintComps(vector<string> &line);

PReader path_Create(string filePath) {
  FILE *fd = fopen(filePath.c_str(), "r");
  if(!fd) {
    return NULL;
  }
  string path = "";
  PReader response = (PReader)malloc(sizeof(Reader));
  memset(response, 0, sizeof(Reader));
  response->configPath = new string();
  char chr;
  while(fscanf(fd, "%c", &chr) != EOF) {
    response->configPath->push_back(chr);
  }
  fclose(fd);
  if(!path_Parse(response)) {
    return NULL;
  }
  return response;
}

vector<string> split(string &input, string temp) {
  vector<string> response;
  string current = "";
  if(!temp.size()) {
    response.push_back(input);
    return response;
  }
  for(int32_t i = 0, c = input.size(); i < c; i++) {
    uint8_t found = 1;
    if((int32_t)input.size() - (int32_t)i < (int32_t)temp.size()) {
      break;
    }
    for(int32_t j = i, p = 0, k = temp.size(); p < k; p++, j++) {
      if(j >= input.size()) {
        found = 0;
        break;
      }
      if(input[j] != temp[p]) {
        found = 0;
        break;
      }
    }
    if(found) {
      if(current.size()) {
        response.push_back(current);
      }
      current.clear();
      i += (int32_t)temp.size() - 1;
    } else {
      current += input[i];
    }
  }
  if(current.size()) {
    response.push_back(current);
  }
  return response;
}

void path_PrintComps(vector<string> &line) {
  for(size_t i = 0, c = line.size(); i < c; i++) {
    cout << "->" << line[i] << "<-\n";
  }
}

void path_ProcessLine(PReader reader, string &line) {
  vector<string> currentLine = split(line, "=");
  if(currentLine.size() != 2) {
    return ;
  }
  if(currentLine[0] == "game_path") {
    reader->gamePath = new string(currentLine[1]);
  } else if(currentLine[0] == "dll_path") {
    reader->dllPath = new string(currentLine[1]);
  } else if(currentLine[0] == "game_name") {
    reader->gameName = new string(currentLine[1]);
  } else if(currentLine[0] == "dll_name") {
    reader->dllName = new string(currentLine[1]);
  }
}

uint8_t path_Parse(PReader reader) {
  vector<string> line = split(*reader->configPath, "\n");
  if(line.size() < 2) {
    return 0;
  }
  for(int32_t i = 0, c = line.size(); i < c; i++) {
    path_ProcessLine(reader, line[i]);
  }
  if(!reader->dllPath || !reader->gamePath || !reader->gameName || !reader->dllName) {
    return 0;
  }
  return 1;
}
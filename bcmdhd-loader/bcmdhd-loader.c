/*
 * Copyright (C) 2016 The CyanogenMod Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

//#define LOG_NDEBUG 0
#define LOG_TAG "bcmdhd-loader"

#include <cutils/log.h>
#include <stdio.h>
#include <string.h>

#define DEFERRED_INITCALLS "/proc/deferred_initcalls"
#define MAX_FILE_NAME_CHARS 255

FILE *fp;
char file_name[MAX_FILE_NAME_CHARS];

void cat(FILE* f)
{
  ALOGI("%s", __FUNCTION__);
  int c;
  while(EOF != (c = getc(f))) {
    if (EOF == putchar(c)) {
      perror("cat");
      return;
    }
  }
  if (ferror(f)) {
    perror("cat");
    return;
  }
}

int main(int argc __unused, char *argv[] __unused)
{
    strcpy(file_name, DEFERRED_INITCALLS);

    fp = fopen(file_name, "r");

    if(fp == NULL) {
       printf("%s: No such file or directory\n", file_name);
       return 0;
    }
    cat(fp);

    fclose(fp);

    return 0;
}

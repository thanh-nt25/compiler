#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_WORDS 1000
#define MAX_STOPWORDS 100
#define MAX_LINE_LENGTH 1024

typedef struct {
    char word[50];
    char lines[100]; 
} WordIndex;

int isStopWord(char *word, char stopWords[MAX_STOPWORDS][50], int stopWordCount) {
    for (int i = 0; i < stopWordCount; i++) {
        if (strcmp(word, stopWords[i]) == 0) {
            return 1; 
        }
    }
    return 0;
}

void toLowerStr(char *str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}

int main() {
    FILE *textFile = fopen("vanban.txt", "r");
    FILE *stopWordFile = fopen("stopw.txt", "r");
    if (!textFile || !stopWordFile) {
        printf("Không thể mở tệp!\n");
        return 1;
    }

    char stopWords[MAX_STOPWORDS][50];
    int stopWordCount = 0;
    while (fgets(stopWords[stopWordCount], sizeof(stopWords[stopWordCount]), stopWordFile)) {
        stopWords[stopWordCount][strcspn(stopWords[stopWordCount], "\n")] = '\0';
        toLowerStr(stopWords[stopWordCount]); 
        stopWordCount++;
    }
    fclose(stopWordFile);

    WordIndex wordIndices[MAX_WORDS];
    int wordCount = 0;

    char line[MAX_LINE_LENGTH];
    int lineNumber = 0;

    while (fgets(line, sizeof(line), textFile)) {
        lineNumber++;
        char *token = strtok(line, " .,;:!?()\n");
        while (token) {
            char word[50];
            strcpy(word, token);
            toLowerStr(word); 

            int isProperNoun = isupper(token[0]) && (line[token - line - 1] != '.');
            if (!isStopWord(word, stopWords, stopWordCount) && !isProperNoun) {
                int found = 0;
                for (int i = 0; i < wordCount; i++) {
                    if (strcmp(wordIndices[i].word, word) == 0) {
                        char lineStr[10];
                        sprintf(lineStr, "%d", lineNumber);
                        if (!strstr(wordIndices[i].lines, lineStr)) {
                            strcat(wordIndices[i].lines, ",");
                            strcat(wordIndices[i].lines, lineStr);
                        }
                        found = 1;
                        break;
                    }
                }
                if (!found) {
                    strcpy(wordIndices[wordCount].word, word);
                    sprintf(wordIndices[wordCount].lines, "%d", lineNumber);
                    wordCount++;
                }
            }
            token = strtok(NULL, " .,;:!?()\n");
        }
    }
    fclose(textFile);

    for (int i = 0; i < wordCount - 1; i++) {
        for (int j = i + 1; j < wordCount; j++) {
            if (strcmp(wordIndices[i].word, wordIndices[j].word) > 0) {
                WordIndex temp = wordIndices[i];
                wordIndices[i] = wordIndices[j];
                wordIndices[j] = temp;
            }
        }
    }

    printf("Index table:\n");
    for (int i = 0; i < wordCount; i++) {
        printf("%s %s\n", wordIndices[i].word, wordIndices[i].lines);
    }

    return 0;
}

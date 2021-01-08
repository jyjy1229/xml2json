#include <stdio.h>
#include <string.h>
#include <iostream>
#include <string>
#include <stack>

using namespace std;

stack <string> tags;

int main(){
    FILE *xml = NULL, *json = NULL;
    xml = fopen("network.xml", "r");
    json = fopen("network.json", "w");

    if(xml == NULL){
        printf("Cannot open file");
        return 0;
    }

    // Read xml file
    char sentence[512], tag[100];
    bool first = true;
    int i = 0, j = 0, depth = 0;
    while(!feof(xml)){
        memset(tag, 0, sizeof(tag));
        memset(sentence, 0, sizeof(sentence));
        i = 0; j = 0;

        fgets(sentence, sizeof(sentence), xml);
        if(first){
            first = false;
            continue;
        }
        
        // Get tag
        while(sentence[i] == ' ' || sentence[i] == '<' || sentence[i] == '/'){
            i++;
        }
        while(sentence[i] != ' ' && sentence[i] != '>'){
            tag[j] = sentence[i];
            i++;
            j++;
        }

        if(!tags.empty() && tags.top().compare(string(tag))){
            fprintf(json, "}\n");
            tags.pop();
            depth--;
            continue;
        }
        else{
            fprintf(json, "{\n");
            tags.push(string(tag));
            depth++;
            for(int i = 0;i < depth;i++){
                fprintf(json, "\t");
            }
            fprintf(json, "\"%s\":", tag);
        }
        
        // Split space
        while(sentence[i] != '>'){
            i++;
        }
        if(sentence[i-1] == '/'){
            fprintf(json, "}\n");
            tags.pop();
            depth--;
        }
    }

    fclose(xml);
    fclose(json);
    return 0;
}
#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <stack>

using namespace std;

struct tag_info {
    string tag = string("");
    bool is_first_key = true;
    string current_key = string("");
};

stack <tag_info> tags;
FILE *xml = NULL, *json = NULL;
char sentence[512], tag[100], key[100], value[100];
bool first_line = true;
int i, j, depth;

int main(){
    ifstream xml;
    ofstream json;
    xml.open("network.xml");
    json.open("network.json");

    if(!xml.is_open()){
        printf("Cannot open file");
        return 0;
    }

    while(!xml.eof()){
        // initialize
        memset(tag, 0, sizeof(tag));
        memset(sentence, 0, sizeof(sentence));
        i = 0; j = 0;

        // get line
        xml.getline(sentence, 1024);

        // ignore first line and handle end of file
        if(first_line){
            first_line = false;
            continue;
        }
        if(strcmp(sentence, "") == 0)   continue;

        // Get tag
        while(sentence[i] == ' ' || sentence[i] == '<' || sentence[i] == '/'){
            i++;
        }
        while(sentence[i] != ' ' && sentence[i] != '>'){
            tag[j] = sentence[i];
            i++;
            j++;
        }

        // Check if it is close tag
        if(!tags.empty() && tags.top().tag.compare(string(tag)) == 0){
            json.write("\n", 1);
            depth--;
            for(int t = 0;t < depth;t++){
                json.write("\t", 1);
            }
            json.write("]}", 2);
            tags.pop();
            if(!tags.empty())   tags.top().current_key = string(tag);
            continue;
        }

        if(!tags.empty() && tags.top().is_first_key == true){
            tags.top().is_first_key = false;
            json.write("{\n", 2);
            depth++;
        }
        else if(tags.empty()){
            json.write("{\n", 2);
            depth++;
        }
        else{
            if(tags.top().current_key.compare(string(tag)) != 0 && tags.top().current_key.compare(string("")) != 0) json.write("],\n", 3);
            else    json.write(",\n", 2);
        }

        for(int t = 0;t < depth;t++){
            json.write("\t", 1);
        }

        if(tags.empty() || tags.top().current_key.compare(string(tag)) != 0){
            json.write("\"", 1);
            json.write(tag, strlen(tag));
            json.write("\":[", 3);
        }
        
        tags.push({string(tag), true, string("")});
        // Split
        while(sentence[i] != '>'){
            while(sentence[i] != ' ' && sentence[i] != '/' && sentence[i] != '>'){
                memset(key, 0, sizeof(key));
                memset(value, 0, sizeof(value));

                j = 0;
                while(sentence[i] != '=' && sentence[i] != ' '){
                    key[j] = sentence[i];
                    i++;
                    j++;
                }
                while(sentence[i] != '"'){
                    i++;
                }
                i++;
                j = 1;
                value[0] = '"';
                while(sentence[i] != '"'){
                    value[j] = sentence[i];
                    i++;
                    j++;
                }
                i++;
                value[j] = '"';

                if(!tags.empty() && tags.top().is_first_key == true){
                    json.write("{\n", 2);
                    tags.top().is_first_key = false;
                    depth++;
                }
                else if(tags.empty()){
                    json.write("{\n", 2);
                    depth++;
                }
                else{
                    json.write(",\n", 2);
                }

                for(int t = 0;t < depth;t++){
                    json.write("\t", 1);
                }
                json.write("\"", 1);
                json.write(key, strlen(key));
                json.write("\": ", 3);
                json.write(value, strlen(value));
            }
            if(sentence[i] == '>')  break;
            if(sentence[i] == '/'){
                i++;
                break;
            }
            i++;
        }

        if(sentence[i-1] == '/'){
            json.write("\n", 1);
            depth--;
            for(int t = 0;t < depth;t++){
                json.write("\t", 1);
            }
            json.write("}", 1);
            tags.pop();
            if(!tags.empty())   tags.top().current_key = string(tag);
        }
    }
    json.write("\n]}", 2);
    xml.close();
    json.close();
    return 0;
}

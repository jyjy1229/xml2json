#include <stdio.h>
#include <string.h>
#include <string>
#include <stack>

using namespace std;

stack <string> tags;

int main(){
    FILE *xml = NULL;
    xml = fopen("network.xml", "r");

    // Error check
    if(xml == NULL){
        printf("Cannot open file");
        return 0;
    }
    // Read xml file
    char sentence[1023], tag[100];
    while(!feof(xml)){
        fgets(sentence, 1023, xml);
        // printf("%s", sentence);
        
        // Get tag
        int i = 0, j = 0;
        memset(tag, 0, sizeof(tag));
        while(sentence[i] == ' ' || sentence[i] == '<' || sentence[i] == '/'){
            i++;
        }
        while(sentence[i] != ' ' && sentence[i] != '>'){
            tag[j] = sentence[i];
            i++;
            j++;
        }
        printf("%s\n", tag);
    }

    fclose(xml);
    return 0;
}
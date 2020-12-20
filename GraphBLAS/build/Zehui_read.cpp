#include <stdio.h>
#include <string.h>
#include<iostream>
#include<sstream>
#include<string>
using namespace std;
int main (int argc, char **argv)
{
    FILE *fp = NULL,*fw=NULL;
    char str[100]={0};

    fp = fopen(argv [1],"r");

    string outputFile ="out_";
   outputFile = outputFile + argv [1];
    fw = fopen((char*)outputFile.data(),"w");
    while(fgets(str,sizeof(str),fp))
    {

        char str3[sizeof(str)+2];
           char const  *str2 = "0\n";
        str[strlen(str)-1]= ' ';
        // str[strlen(str)-1]= '0';
        // strcat(str,"\r\n");  str2

        strcat(str,str2); 
        
         sprintf(str3,"%s%s",str,str2);
        if (str[0] != '%') //  &&  strlen(str)<=6     4 1 1  strlen(str)==6
        {
            // std::cout<<str;
            fputs(str,fw);
        }
    }
    fclose(fp);
    fclose(fw);

    return 0;
}
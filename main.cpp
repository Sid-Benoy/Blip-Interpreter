#include <iostream>
#include "Parse.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <iterator>
using namespace std;
vector<string> values;
map<string, int> vars;
int flag = 0;

void run();
void processText();
void processOutput();
void processVar();
void processSet();
int parse(vector<string>& values);
int operate(const string op, int arg1, int arg2);

void run(){
    bool all_done = false;
    string token;
    do {
        if(flag == 0)
            read_next_token();
        token = next_token();
        if (token == "text")
            processText();
        else if (token == "output")
            processOutput();
        else if (token == "var")
            processVar();
        else if (token == "set")
            processSet();
        else if(token == "//")
            skip_line();
        else if (next_token_type == END)
            all_done = true;
        else
            cout << "error in reading input file" << endl;
    } while(!all_done);

}

int parse(vector<string>& values){
    string first = values[0];
    values.erase(values.begin());
    if((first[0] >= 'A' && first[0] <= 'Z') || (first[0] >= 'a' && first[0] <= 'z'))
        return vars.at(first);
    else if(first.compare("+")!= 0 && first.compare("-")!= 0 && first.compare("*")!= 0
    && first.compare("/")!= 0 && first.compare("&&")!= 0 && first.compare("%")!= 0 && first.compare(">")!= 0 &&first.compare("<")!= 0
    && first.compare("==")!=0 && first.compare("!=")!= 0 && first.compare("~")!= 0 && first.compare("!")!= 0){
        return stoi(first);
    }
    int arg1 = parse(values);
    if(first == "~" || first == "!")
        return operate(first, arg1, 0);
    int arg2 = parse(values);
    return operate(first, arg1, arg2);

}

int operate(const string op, int arg1, int arg2){
    if(op == "+") return arg1 + arg2;
    if(op == "-") return arg1-arg2;
    if(op == "*") return arg1*arg2;
    if(op == "/") return arg1/arg2;
    if(op == "%") return arg1%arg2;
    if(op == "&&") return arg1&&arg2;
    if(op == "||") return arg1||arg2;
    if(op == "<") return arg1 < arg2;
    if(op == ">") return arg1 > arg2;
    if(op == "==") return arg1==arg2;
    if(op == "!=") return arg1!= arg2;
    if(op == "~") return arg1*-1;
    if(op == "!" && arg1 == 0) return 1;
    if(op == "!") return 0;

}

void processText(){
    read_next_token();
    string output = next_token();
    flag = 0;
    cout << output;
}

void processOutput(){
    read_next_token();
    if(next_token_type == NUMBER)
        cout << token_number_value;
    else if(next_token_type == NAME){
        cout << vars[next_token()];
    }
    else if(next_token_type == SYMBOL){
        values.push_back(next_token());
        read_next_token();
        if(next_token_type == NAME){
            values.push_back(next_token());
        }
        while(1){
            values.push_back(next_token());
            read_next_token();
            string output = next_token();
            if(output == "var" || output == "text" ||output == "set"){
                flag = 1;
                break;
            }
        }
        cout << parse(values);
        values.clear();
    }

}

void processVar(){
    int flag1 =0;
    map<string, int> :: iterator iter;
    read_next_token();
    string variable = next_token();
    read_next_token();
    int number = token_number_value;
    for(iter = vars.begin(); iter != vars.end(); iter++){
        if(iter->first == variable) {
            cout << "variable already exists";
            vars[iter->first] = number;
            flag1 = 1;
        }
    }
    if(flag1!= 1) vars.insert(pair<string, int>(variable, number));

}

void processSet(){
    int result = 0;
    map<string, int> :: iterator iter;
    read_next_token();
    string variable = next_token();
    read_next_token();
    if(next_token_type == NUMBER){
       vars.at(variable) = token_number_value;
    }
    else if(next_token_type == SYMBOL){
        values.push_back(next_token());
        read_next_token();
        string output = next_token();
        while(1){
            values.push_back(next_token());
            read_next_token();
            string output = next_token();
            if(output == "var" || output == "text" ||output == "set"){
                flag = 1;
                break;
            }
        }
        result = parse(values);
        values.clear();
    }
    vars[variable] = result;

}


int main(void){
    set_input("test_grader.blip");
   run();

}


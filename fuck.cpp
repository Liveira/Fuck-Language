#include <regex>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
/*
    FUCK SOURCE
*/

using namespace std;
/*
    Functions!
*/
regex Func("^([A-Z_a-z]+[\\d]*)(\\{(.*)??\\});{0,1}$");
regex Var("^([\\w]+|[\\_]+)\\s*(\\<\\<\\-|\\<\\+\\-)\\s*(.*);{0,1}$");
vector<string> split(string Text,string Del){
    vector<string> Array;
    auto start = 0;
    auto end = Text.find(Del);
    while(end != string::npos){
        string recor = Text.substr(start,end - start);
        start = end + Del.length();
        end = Text.find(Del,start);
        Array.push_back(recor);
    }
    string recor = Text.substr(start,end - start);
    start = end + Del.length();
    end = Text.find(Del,start);
    Array.push_back(recor);
    return Array;
}
string print(string text, ...){
    cout << endl << text << endl;
    return "NoFuck";
}
string input(string text, ...){
    cout << text << endl;
    string var;
    cin >> var;
    return var;
}
bool startsWith(string text,string what){
    if(text.rfind(what,0) == 0){
        return true;
    } else {
        return false;
    }
}
bool endsWith(string text,string what){
    return(0 == text.compare (text.length() - what.length(), what.length(), what));
}
bool isString(string text){
    if (startsWith(text,"\"") && endsWith(text,"\"")){
        return true;
    } else if (startsWith(text,"\'") && endsWith(text,"\'")){
        return true;
    } else {
        return false;
    }
}
void SyntaxError(string Line, int NL){
    cerr << "SyntaxError -> \"" << Line << "\", Line -> " << NL << endl ; 
    exit(1);
}
void stringSplit(string &text){
    string* poin = &text;
    poin->replace(0,1,"");
    poin->replace(text.length()-1,text.length(),"");
}
bool isFunction(string text){
    if(regex_match(text,Func)){
        return true;
    } else {return false;}
}
bool isVar(string text){
    if(regex_match(text,Var)){
        return true;
    } else {return false;}
}
string ReturnFunc(vector<string> text){

}
int main(int carg,char *argv[]){
    // Criando variáveis! 
    typedef string (*FnPtr)(string...);
    
    map<string,FnPtr> funcAmbient = {
        {"fuckout",print},
        {"fuckin",input}
    }; 
    map<string,string> vars = {

    };
    vector<string> Code;
    if (carg <= 1){
        cout << "ERROR: no such file.";
    }
    fstream File(argv[1]);
    if (!File) {
		cout << "ERROR: No such file.";
	}
	else {
        string line;
		while (getline(File, line)) {
            Code.push_back(line);
        }
        File.close();
	}
    for(int i=0;i<Code.size();i++){
        string L = Code[i];
        smatch Match;
        if(L == "" || L == " "){
            continue;
        }
        if(isFunction(L)){
            regex_match(L,Match,Func);
            string FunctionName = Match[1];
            vector<string> Args = split(Match[3],",");
            if(funcAmbient.find(FunctionName) != funcAmbient.end()){
                for(int i=0;i<Args.size(); i++){
                    if(Args[i] == ""){continue;}
                    if(isString(Args[i])){
                        stringSplit(Args[i]);
                    }else if (vars.find(Args[i]) != vars.end()){
                        Args[i] = vars[Args[i]];
                    } else {
                        SyntaxError(L,i);
                    }
                }
                string first = Args[0];
                Args.erase(Args.begin());
                funcAmbient[FunctionName](first,Args);
            }
        } else if (isVar(L)){
            regex_match(L,Match,Var);
            if (Match[2] == "<+-" && vars.find(Match[1]) == vars.end()){
                SyntaxError(L,i);
            } else if (vars.find(Match[1]) == vars.end()) {
                if (isString(Match[3])){
                    string a = Match[3];
                    stringSplit(a);
                    vars[Match[1]] = a;
                } else if (vars.find(Match[3]) != vars.end()) { 
                    vars[Match[1]] = vars[Match[3]];
                }
            } else if (Match[2] == "<+-" && vars.find(Match[1]) != vars.end()){
                vars[Match[1]] += vars[Match[3]];
            } else {
                SyntaxError(L,i);
            }

        }
    }
    return 0;   
    
}

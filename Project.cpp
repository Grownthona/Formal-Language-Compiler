#include<bits/stdc++.h>
using namespace std;

struct symbol_table
{
   string name,id,data,scope,val;
};

int cnt = 0;
string output = "",output_2="";
bool in_double_comment = false;
bool in_single_comment = false;
bool func_on = false;
bool assignment_on = false;
vector<symbol_table>table;
map<string,string>check_var;
char q = '\'';
map<char,int>op={{'+',1},{'-',1},{'*',1},{'/',1},{'=',1},{'%',1},{'>',1},{'<',1},{'!',1}};
map<char,int>sep={{';',1},{',',1},{':',1}};
map<char,int>par = {{'{',1},{'}',1},{'(',1},{')',1},{'[',1},{']',1}};
set<string>st{"auto","double","int","struct","break","else","long","switch","case","typedef","char","return","union","continue","for","signed","void","do","if","static","while","default","goto","sizeof","const","float","unsigned","stdio.h","#include"};

vector<string>error;

int number(string s)
{
    int s1=0;
    if(isdigit(s[0]))
    {
    int pos = -1;
    int s2=0,s3=0;
    pos = s.find(".");
    if(pos>-1)
    {
        s2 =1;
        if(pos>0){
        for(int i=0;i<pos;i++)
        {
            if(!isdigit(s[i]))
            {
                s3 = 1;
                break;
            }
        }}
        if(s3==0)
        {
            for(int i=pos+1;i<s.length();i++)
            {
                if(!isdigit(s[i]))
                {
                    s3 = 1;
                    break;
                }
            }
        }
        if(s3==0)
            s1 = 1;
    }
    else if(!s2)
    {
        for(int i=0;i<s.length();i++)
        {
            if(!isdigit(s[i]))
            {
                s3 = 1;
                break;
            }
        }
        if(s3==0)
            s1 = 1;
    }
    else
        s1=0;
    }
    return s1;
}
bool identifier(string id)
{
    bool flag = true;

    if(id == "main")
        return true;

    if(isalpha(id[0]) || id[0]=='_')
    {
        for(int i=1;i<id.length();i++)
        {
            if(!isdigit(id[i]))
            {
                if(!isalpha(id[i]))
                {
                    if(id[i]!='_')
                    {
                        flag = false;
                        break;
                    }
                }
            }
        }
    }
    else{
        flag = false;
    }

    return flag;
}
void remove_comments_and_spaces(string input)
{
    int c=0;
    cnt++;
    output_2 += to_string(cnt)+" ";
    for(int i=0;i<input.length();i++)
    {
        if(input[i]==' ' && input[i+1]==' '){
            continue;
        }
        if(input[i]=='/' && input[i+1]=='/'){

            in_single_comment = true;
            i++;
        }
        else if(input[i]=='/' && input[i+1]=='*'){

            in_double_comment = true;
            i++;
        }
        else if(input[i]=='*' && input[i+1]=='/'){

            in_double_comment = false;
            i++;
        }
        else if(!in_single_comment && !in_double_comment){

            output += input[i];
            output_2 += input[i];
        }
    }
    output_2 +='\n';
    in_single_comment = false;
}

string isFunc(vector<string>& strs,string s)
{
    string func_name = "";
    if(s == "(")
    {
        int i = strs.size()-1;
        string id_name = strs[i-1];
        // s[i-1] = f1
        // s[i-2] = id
        // s[i-3] = double
        if(strs[i-2]=="id" && st.find(strs[i-3])!=st.end())
        {
            func_name = strs[i-1];
            func_on = true;
        }
    }
    return func_name;
}
void insert_to_table(string name,string id,string data,string scope,string val)
{
    table.push_back({name,id,data,scope,val});
    check_var[name] = scope;
}
void display()
{
    for(int i=0;i<table.size();i++)
    {
        cout << i+1 << ' ' << table[i].name << ' ' << table[i].id << ' ' << table[i].data << ' ' <<table[i].scope << ' ' << table[i].val << endl;
    }
}
bool isIdDeclared(vector<string>& strs,int index)
{
    // s[index] = id
    // s[index -1] = float
    bool id_check = false;
    if(strs[index-1]=="int" || strs[index-1]=="double"|| strs[index-1]=="float")
    {
        id_check = true;
    }
    return id_check;
}

string doArithmetic(vector<string>& strs,int index)
{
    double k = 0,sum = 0;
    int n = strs.size()-1,flag = 0;

    for(int i = index + 1; i<n; i++)
    {
        if(isdigit(strs[i][0]))
        {
            stringstream ss;
            ss << strs[i];
            ss >> k;
            flag = 1;
        }
        else if(strs[i] == "-")
        {
            sum -= k;
        }
        else if(strs[i] == "+")
        {
            sum += k;
        }
    }

    sum += k;
    if(!flag)
    {
        return "invalid";
    }
    return to_string(sum);
}

int main()
{
    ifstream fout;
    ofstream fin;
    string line,str,set_space,lex_space;

    fout.open("Input.c",ios :: out);
    fin.open("Output.txt",ios :: in | ios ::trunc);

    if(fout.is_open())
    {
        while(fout.good())
        {
            getline(fout,line);
            remove_comments_and_spaces(line);
        }
    }
    else
        cout << "File can not be open" << endl;

    cout << "After removing comments and extra spaces : " << endl;
    cout <<  output << endl;


    fin << output;
    fin.close();
    fout.close();


    ifstream readans;
    ofstream write_lexime_separate;
    readans.open("Output.txt",ios :: out);
    write_lexime_separate.open("Output2.txt",ios :: in | ios ::trunc);
    if(readans.is_open())
    {
        while(readans.good())
        {
            getline(readans,str);
            for(int i=0;i<str.length();i++)
            {
                 set_space += str[i];
                 if(op[str[i]]==1 || op[str[i+1]]==1)
                    set_space+=' ';
                else if(par[str[i]]==1 || par[str[i+1]]==1)
                    set_space+=' ';
                 else if(sep[str[i]]==0 && sep[str[i+1]]==1)
                    set_space+=' ';
                 else if(sep[str[i+1]]==1 && sep[str[i]]==1)
                    set_space+=' ';
                 else if(sep[str[i]]==1 && sep[str[i+1]]==0)
                     set_space += ' ';
                 else if(str[i]=='\'' && str[i+1]!='\'')
                    set_space+=' ';
                 else if(str[i]!='\'' && str[i+1]=='\'')
                    set_space+=' ';
            }
        }
    }
    else
        cout << "File can not be open" << endl;

    cout << "After Lexemes are separated : " << endl;
    cout << set_space << endl;
    write_lexime_separate << set_space;
    write_lexime_separate.close();
    readans.close();

    string write_lex;
    ifstream token_read;
    ofstream write_token_identifier;
    token_read.open("Output2.txt",ios :: out);
    write_token_identifier.open("Output3.txt",ios :: in | ios ::trunc);

    while(getline(token_read,lex_space,' '))
    {
        if(st.find(lex_space)!=st.end())
            write_lex+="[kw "+ lex_space + "] ";
        else if(par[lex_space[0]]==1)
            write_lex+="[par " + lex_space + "] ";
        else if(op[lex_space[0]])
            write_lex+= "[op " + lex_space + "] ";
        else if(sep[lex_space[0]] || lex_space[0]==q)
            write_lex+="[sep " + lex_space + "] ";
        else if(identifier(lex_space)==true)
            write_lex+= "[id " + lex_space + "] ";
        else if(number(lex_space)==1)
            write_lex+="[num " + lex_space + "] ";
        else{
           if(lex_space!=""){
           write_lex +="[unk "+ lex_space + "] ";
           error.push_back(lex_space);}}
    }

    cout << "After Lexemes are identified : " << endl;
    cout << write_lex << endl;
    write_token_identifier << write_lex;
    token_read.close();
    write_token_identifier.close();

    //assignment 03
    string new_lex,token_stream;
    ifstream lex_read;
    lex_read.open("Output3.txt",ios :: out);

    if(lex_read.is_open())
    {
        while(lex_read.good())
        {
            getline(lex_read,new_lex,'[');
            string str_comp = new_lex.substr(0,new_lex.length()-2);

            istringstream iss(str_comp);
            string str_79;
            int c = 0,id_not_found = 0;

            while(iss >> str_79)
            {
                c++;

                if(str_79 == "id")
                {
                    id_not_found = 1;
                    token_stream += "[";
                    token_stream += "id ";
                }
                if(c%2 == 0)
                {
                    if(!id_not_found)
                        token_stream += "[";
                    token_stream +=  str_79+"] ";
                }
            }
        }
    }
    else
        cout << "File can not be open" << endl;

    cout << "After lexemes are stremed :" << endl;
    cout << token_stream;
    map<string,vector<string>>myMap;
    map<string,string>scopeTrack;
    istringstream ss(token_stream);
    bool id_found = false;
    bool start = false;
    string k;
    int index = 0,a_index=0;
    string func_name = "global";
    string id_name = "";
    string data_type = "";
    string id_Type = "";
    vector<string>st_arr;
    string return_sum_result;

    int table_size = 0;
    string assign_var = "";
    while(ss >> k)
    {
        string cutoff;
        id_Type = "var";
        table_size = table.size();
        for(int i=0;i<k.length();i++)
        {
            if(k[i]!='[' && k[i]!=']')
            {
                cutoff += k[i];
            }
        }
        st_arr.push_back(cutoff);
        index = st_arr.size()-1;

        if(func_on==true && cutoff=="}")
        {
            func_on = false;
            func_name = "global";
        }
        else if(cutoff == "=")
        {
            a_index = index;
            assignment_on = true;
            assign_var = st_arr[index-1];
        }
        else if(assignment_on == true && cutoff == ";")
        {
            assignment_on = false;
            return_sum_result = doArithmetic(st_arr,a_index);

            for(int i=table.size()-1;i>=0;i--)
            {
                if(table[i].name == assign_var)
                {
                    if(return_sum_result != "invalid")
                    {
                        table[i].val = return_sum_result;
                        break;
                    }
                }
            }
        }
        else if(cutoff == "(")
        {
            if(isFunc(st_arr,cutoff)!="")
            {
                func_name = isFunc(st_arr,cutoff);
                table[table_size-1].id = "func";
            }
        }
        else if(index>1 && st_arr[index-1]=="id")
        {
            if(isIdDeclared(st_arr,index-1))
            {
                id_name = st_arr[index];
                data_type = st_arr[index-2];
                //if(check_var[id_name] == func_name)

                    insert_to_table(id_name,"var",data_type,func_name,"");
                //table.push_back({id_name,"var",data_type,func_name,""});
            }
        }
    }
    display();

    //assignment 4

    //cout << output_2;
    istringstream strm(output_2);
    string add_token,result_add_token = "",result_error;

    while(strm >> add_token)
    {
        if(isdigit(add_token[0]))
        {
            double num;
            stringstream x;
            x << add_token;
            x >> num;

            if(floor(num) == num && result_add_token != "")
            {
                result_add_token += '\n';
            }
        }
        if(st.find(add_token)!=st.end())
           result_add_token += "kw ";
        else if(identifier(add_token)==true)
            result_add_token+= "id ";

        result_add_token += add_token+" ";
    }
    istringstream strm2(result_add_token);
    int line_number = 0;
    bool is_if = false;
    int brace_count = 0;
    vector<char>stck;
    vector<string>new_vec;
    string error_check;
    while(getline(strm2, result_error))
    {
        line_number ++;
        int k = result_error.find("else if");

        if(k > -1)
        {
            if(!is_if)
            {
                cout << "else if statement used incorrectly on line : " << line_number << endl;
            }
        }
        else if(result_error.find("if") != string::npos) {
            is_if = true;
        }
        else if(result_error.find("else") != string::npos)
        {
            if(!is_if)
            {
                cout << "else statement used incorrectly on line : " << line_number << endl;
            }
            is_if = false;
        }

        for(int i = 0;i<result_error.length();i++)
        {
            if(result_error[i]=='{' || result_error[i]=='(' || result_error[i]=='['){
                stck.push_back(result_error[i]);
            }
            else if(result_error[i]=='}' || result_error[i]==')' || result_error[i]==']')
            {
                int sk_size = stck.size()-1;
                if(stck[sk_size] == '(' && result_error[i]==')')
                    stck.pop_back();
                else if(stck[sk_size] == '{' && result_error[i]=='}')
                    stck.pop_back();
                else if(stck[sk_size] == '[' && result_error[i]==']')
                    stck.pop_back();
                else{
                    cout << "Unmatched closing brace on line" << line_number << endl;
                }
            }
        }
        }

    }

}

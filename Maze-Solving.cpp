#include<iostream>
#include<fstream>
#include<string>
#include<stack>
#include<cmath>
#include <vector>
#include <sstream>
#include <iomanip>
using namespace std;
double digit_math(char&,double&,double&);
bool check_priority(char &cr1, char &cr2);
int priority(char &cr1);
string  set_precision(double &num1);
vector<double> Vector_math(char &sign,vector<double>& v1,vector<double>& v2);
int main(int argc, char* argv[])//argc為argv中的string數量(也就是輸入的指令數量( file input_name ouput_name 總共有三個參數))，argv為檔案的路徑
{
    ios_base::sync_with_stdio(0);
    cin.tie(0);
    ifstream fin;
    fin.open(argv[1]);
    string InPut,data="";
    bool need_endl = false;
    while(getline(fin, data,'\n')){
        if(need_endl){
            cout<<endl;
        }
        stack<double> number;
        stack<char> sign;
        stack<vector<double>> V_stack;
        string postfix="",num="",str_temp;
        int index=0;
        
        vector<double> list;
        for(auto &i:data){
            if(!isspace(i)){
                str_temp.push_back(i);
            }
        }
        for(auto &i:str_temp){//將數字丟進stack內
            //中序式轉後序式
            if(isdigit(i)||i=='.'){
                num.push_back(i);
                
            }
            else if(i=='['){
                sign.push(i);
                postfix.push_back(i);
            }
            else if(i=='+'||i=='-'||i=='*'||i=='/'||i=='('||i=='^'){
                if(num!=""){
                    if(postfix.back()!='~'){postfix+=" ";}
                    postfix+=num;
                    num="";
                }
                if(i=='-'&&!(isdigit(str_temp[index-1]))&&str_temp[index-1]!=')'&&str_temp[index-1]!=']'){
                    sign.push('~');
                }
                else if(sign.empty()||i=='('){
                    sign.push(i);
                }
                else{
                    if(check_priority(i,sign.top())){
                        while(!sign.empty()&&!(sign.top()=='['||sign.top()=='(')&&check_priority(i,sign.top())){
                            postfix.push_back(sign.top());
                            sign.pop();
                            
                            
                        }
                    }
                    sign.push(i);
                }
            }
            else if(i==','||i==')'||i==']'){
                if(num!=""){
                    if(postfix.back()!='~'){postfix+=" ";}
                    postfix+=num;
                    num="";
                }
                while(!(sign.top()=='('||sign.top()==','||sign.top()=='[')){
                    postfix.push_back(sign.top());
                    sign.pop();
                }
                if(i==')'||i==']'){
                    sign.pop();
                }
                if(i==','||i==']'){
                    postfix.push_back(i);
                }
                // if(!sign.empty()){
                //     cout<<"i:"<<i<<" top:"<<sign.top()<<endl;
                // }
                
            }
            ++index;
        }
        while(!sign.empty()){
            postfix.push_back(sign.top());
            sign.pop();
            }
        // cout<<"infix:"<<str_temp<<endl<<endl;
        // cout<<"postfix:"<<postfix<<endl<<endl;
        //----------------------------
        //處理後序式內的數字
        stringstream ss;
        double num_temp=0;
        for(auto i:postfix){
            if(isdigit(i)||i=='.'){
                num.push_back(i);
            }
            else{
                if(num!=""){//sting to double
                    ss<<num;
                    ss>>num_temp;
                    number.push(num_temp);
                    num="";
                    ss.clear();
                    }
                
                if(i==']'||i==','){
                    list.push_back(number.top());//將處理好的數字放入vector內
                    number.pop();num="";
                    
                    if(i==']'){
                        V_stack.push(list);
                        list.clear();
                    }
            }
                else if (!isspace(i)&&i!='['){
                    if(i=='~'&&!number.empty()){
                        number.top()*=-1;
            }
                    else if(!number.empty()){
                        double temp1=number.top();number.pop();
                        double temp2=number.top();number.pop();
                        
                        number.push(digit_math(i,temp2,temp1));
                    }
                    else if(!V_stack.empty()){
                        //cout<<"i:"<<i<<endl;
                        vector<double> v_temp1(V_stack.top());V_stack.pop();
                        vector<double> v_temp2(V_stack.top());V_stack.pop();
                        if(i=='/'||i=='-'){
                            V_stack.push(Vector_math(i,v_temp2,v_temp1));
                        }
                        else{
                            V_stack.push(v_temp1.size()>v_temp2.size()?Vector_math(i,v_temp1,v_temp2):Vector_math(i,v_temp2,v_temp1));
                        }
                    }
                }
            }
            
            }
        
    //----------------------------------------------------------------
    //處理向量計算
    vector<double> v_last(V_stack.top());
        string result="";
        double ttemp;
        int V_count=0;
        cout<<"[";
        for(auto i=v_last.begin();i!=v_last.end();++i){
            ttemp=*i;
            if(ttemp!=floor(ttemp)){
                cout<<set_precision(ttemp);
            }
            else{
                cout<<fixed<<setprecision(0)<<ttemp;
            }
            V_count++;
            if(V_count!=v_last.size()){
                cout << ",";
            }
            
        }
        cout<<"]";
        data="";
        need_endl=true;
    }
    fin.close();
}
string  set_precision(double &num1){
    stringstream ss;
    string temp;
    ss.precision(2);
            ss<<fixed<<num1;
            ss>>temp;
            if(temp.back()=='0'){
                temp.pop_back();
            }
            ss.clear();
            return temp;
}
bool check_priority(char &cr1, char &sign_top){
    int temp1=priority(cr1);
    int temp2=priority(sign_top);
    return temp1<=temp2;
}
int priority(char& cr1){
    switch(cr1){
        case '(':case '[':return 0;
        case '+':case '-':return 1;
        case '*':case '/':case '~':return 2;
        case '^':return 3;
        case ']':case ')':case ',':return 4;

    }
}
double digit_math(char &sign,double &num1,double &num2){
    double temp;
    if(sign=='+'){
        temp = num1 + num2;
        return  temp;
    }
    else if(sign=='-'){
        temp = num1 - num2;
        return  temp;
    }
    else if(sign=='*'){
        temp = num1 * num2;
        return  temp;
    }
    else if(sign=='/'){
        temp = num1 / num2;
        return  temp;
    }
    else if(sign=='^'){
        temp = pow(num1,num2);
        return  temp;
    }
    
}

vector<double> Vector_math(char &sign,vector<double>& v1,vector<double>& v2){
    vector<double> temp1;
    int count=0;
    switch (sign) {
        case '+':case '*':
            for(int i=0;i<v1.size();++i){
                if(count==v2.size()){
                    count=0;
                }
                    if(sign=='+'){
                        temp1.push_back(v1[i]+v2[count]);
                    }
                    else if(sign=='*'){
                        temp1.push_back(v1[i]*v2[count]);
                    }
                
                ++count;
            }
            return temp1;
        case '-':case '/':
            if(v1.size()>v2.size()){
                for(int i=0;i<v1.size();++i){
                        if(count==v2.size()){
                            count=0;
                        }

                if(sign=='-'){
                    temp1.push_back(v1[i]-v2[count]);
                }
                else if(sign=='/'){
                    temp1.push_back(v1[i]/v2[count]);
                }
                        ++count;
                    }
                }
            else{
                for(int i=0;i<v2.size();++i){
                        if(count==v1.size()){
                            count=0;
                        }
                if(sign=='-'){
                    temp1.push_back(v1[count]-v2[i]);
                }
                else if(sign=='/'){
                    temp1.push_back(v1[count]/v2[i]);
                }
                        ++count;
                    }
            }
            return temp1;
        
    }


}
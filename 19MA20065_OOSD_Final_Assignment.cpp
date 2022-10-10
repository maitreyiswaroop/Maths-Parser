/*
OOSD Final Assignment
---------------------
Name:     Maitreyi Swaroop
Roll No.: 19MA20065
Email id.: maitreyiswaroop@gmail.com, maitreyiswaroop@iitkgp.ac.in
---------------------
Instructions
1. Ensure that all statements are correctly parenthesised.
2. All arguments to functions must be put within parentheses, i.e. write sin(x) NOT sinx and log(x) NOT logx.
3. All powers/exponents must be put within parentheses, i.e. write e^(x), x^(7) NOT e^x, x^7. Also, expressions like sin^2(x) (== sin(x) whole squared) must be entered as (sin(x))^(2). Same for cos/tan/log etc.
4. All + and - signs must have a space around them, i.e. x + tan(x^(10) - sin(x^(3) + x))
    (Do not worry too much about this point however, there is a utility function to correct lack of padding around + and - signs.)
 5. Inverse functions are to be written as arcsin, arccos etc. NOT sin-1, cos-1.
---------------------
 (PS. Throughout the code there are commented lines starting with "DEBUG <function name> random number" - I used these to identify specific problem parts when running the code and chose to leave them in in case you wish to uncomment them and see the various function calls and intermediate values. )
*/

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

// ----- utility functions list
int str2int(string s); // converts a string to an integer
int strIsInt(string s); // checks whether given string represents an integer
int subtermCount(string expr); // returns the number of terms in an expression, eg. sin(x) + tan(x) has 2 terms
string int2str(int x); //converts an integer to a string
string pruneRemove(string expr); // corrects formatting of output, leans the expression down
string addReqSpaces(string expr); // corrects formatting of input, adds required spaces around '+' and '-'
bool checkExpr(string expr); // checks whether input expression is formatted properly, according to given instructions
void printD(string expr); // prints the output of string D(string expr)
void printDeriv(string expr); // prints the output of string derivMain(string expr)
// ----- end utility functions list


// ----- calculus functions list
string D(string expr); // returns derivative of single function
string derivMain(string expr); // returns derivative of expression, recursively
string derivProd(string expr); // called by derivMain, returns derivative of product of terms, recursively
string derivQuot(string expr); // called by derivMain, returns derivative of expression of form (subexpression_1)/(subexpression_2), recursively
string derivExpn(string expr); // called by derivMain, returns derivative of expression of form (subexpression_1)^(subexpression_2), recursively
// ----- end calculus functions list

// ----- stack code
struct node{
    int data;
    node* next;
    node* prev;
 };

class Stack{
    private:
        node * head; // pointer to the head link
        node * tail; // pointer to the tail link

    public:
        Stack(){
            head=NULL;
            tail = NULL;
            head = tail;
        }
    
        void push(int d);  // adds a data item to the linked list (in front)
        void pop();   // deletes the first data item
        void display(); // displays the whole linked list
        void displayAsList(); // displays the whole linked list
        int headData();
};

string RemoveParentheses(string s);
// ----- end of stack code


// ----- calculus functions
string D(string expr){
//     cout<<"DEBUG D 1. Finding derivative of:"<<expr<<endl;
   string f = "", arg = "";
   string ans;
   int i=0, sign=0;
  if(expr[i]=='-'){
      sign =1;
      i++;
  }
   while(expr[i]!='(' and i<expr.length())  f+=expr[i++];
   i++;
   while(i<expr.length()-1)  arg+=expr[i++];
    // cout<<"DEBUG D 43534. f=:"<<f<<endl;
//   cout<<"DEBUG function D(string expr) 1:expr="<<expr<<endl<<"\tf = "<<f<<",arg = "<<arg<<endl<<"END debug function D(string expr) 1:"<<endl;
   if(f.length()==1){
      if(f=="x") ans = "1";
      else ans = "";
   }
   else if(f[0]>='0' and f[0] <= '9'){
       string temp="", f1="";
       i=0;
       while(f[i]>='0' and f[i] <= '9') temp+=f[i++];
       while(i<f.length()) f1+=f[i++];
       string temp2 = D(f1+"("+arg+")");
       if(temp2 == "0") ans="";
       else if(temp2 == "1") ans=temp;
       else ans+=temp+"("+D(f1+"("+arg+")")+")";
//        cout<<"DEBUG here in D() 1234, f="<<f<<endl;
   }
   else if(f[f.length()-1] == 'x'){
       string temp="";
       for(i=0;i<f.length()-1;i++)  temp+=f[i];
       if (strIsInt(temp)){
           ans = temp;
       }
       // cout<<"DEBUG here in D() 1234, f="<<f<<endl;
   }
   else if(f == "x^"){
    //   cout<<"DEBUG D in f==x^| expr="<<expr<<"| f="<<f<<"| arg="<<arg<<endl;
      if (strIsInt(arg)){
        //   cout<<"Here in DEBUG D in f==x^| expr="<<expr<<"| f="<<f<<"| arg="<<arg<<endl;
         int intArg = str2int(arg);
         if(intArg == 0) ans = "";
         else if(intArg == 1) ans = "1";
         else if(intArg == 2) ans = "2x";
         else ans = arg + f + "("+int2str(intArg-1)+")";
      }
      else ans = f + "("+arg+" - 1)";
   }
   // D exp and log
   else if(f.length()==2 and f[1]=='^'){
      if(f[0]>='a' and f[0]<='z'){
         if(f[1]=='^'){
            if(f[0]=='e')  ans = expr;
            else ans = expr+"ln("+f[0]+")";
         }
      }
   }
   else if(f == "ln" or f=="log"){
      ans = "(1/("+arg+"))";
   }
   // D trigo
   else if(f == "sin"){
      ans = "cos("+arg+")";
   }
   else if(f == "cos"){
      ans = "- sin("+arg+")";
   }
   else if(f == "tan"){
      ans = "(sec("+arg+"))^(2)";
   }
   else if(f == "cosec" or f=="csc"){
      ans = "- csc("+arg+")"+"cot("+arg+")";
   }
   else if(f == "sec"){
      ans = "sec("+arg+")"+"tan("+arg+")";
   }
   else if(f == "cot"){
      ans = "- (csc("+arg+"))^(2)";
   }
   // D inv trigo
   else if(f == "arcsin"){
      ans = "1/((1 - ("+arg+")^(2))^(1/2))";
   }
   else if(f == "arccos"){
      ans = "- 1/((1 - ("+arg+")^(2))^(1/2))";
   }
   else if(f == "arctan"){
      ans = "1/(1+("+arg+")^(2))";
   }
   else if(f == "arccosec" or f=="arccsc"){
      ans = "- 1/(|"+arg+"|((("+arg+")^(2) - 1)^(1/2)))";
   }
   else if(f == "arcsec"){
      ans = "1/(|"+arg+"|((("+arg+")^(2) - 1)^(1/2)))";
   }
   else if(f == "arccot"){
      ans = "- 1/(1+("+arg+")^(2))";
   }
   // D hyp
   else if(f == "sinh"){
      ans = "cosh("+arg+")";
   }
   else if(f == "cosh"){
      ans = "sinh("+arg+")";
   }
   else if(f == "tanh"){
      ans = "1 - tanh^(2)("+arg+")";
   }
   else if(f == "cosech" or f=="csch"){
      ans = "-coth("+arg+")"+"csch("+arg+")";
   }
   else if(f == "sech"){
      ans = "tanh("+arg+")"+"sech("+arg+")";
   }
   else if(f == "coth"){
      ans = "1 - (coth("+arg+"))^(2)";
   }
   else ans = "";
//   cout<<"DEBUG D 42. Derivative of:"<<expr<<"= "<<ans<<"| f = "<<f<<endl;
   if (sign==1) return "-"+ans;
   return ans;
}

string derivQuot(string expr){
    string ans="", numerator="", denominator="";
    int j=0, brackets = 0;
    while(j<expr.length()){
        if(brackets == 0 and expr[j] == '/')    break;
        if(expr[j]=='(') brackets++;
        else if(expr[j]==')') brackets--;
        numerator+=expr[j++];
   }
   j++;
   while(j<expr.length()){
        denominator+=expr[j++];
   }
   // by quotient rule:
   // (f(x)/g(x))' = (gf' - fg')/(g^2)
//  cout<<"DEBUG derivQuot 1: numerator: "<<numerator<<endl<<"\t\t\tdenominator: "<<denominator<<endl;
    string numDer=derivMain(numerator), denDer=derivMain(denominator);
//    cout<<"DEBUG derivQuot 1.2: numDer: "<<numDer<<endl<<"\t\t\tdenDer: "<<denDer<<endl;
    string sign1="", sign2="";
    if(numDer[0]=='-'){
        numDer.erase(0,1);
        sign1+="-";
    }
    if(denDer[0]=='-'){
        denDer.erase(0,1);
        sign2+="-";
    }
    if(numDer == ""){
        if(sign2=="") ans = "(-" +numerator+"("+denDer+")" +")/(("+denominator+")^(2))";
        else ans = "("+numerator+"("+denDer+")" +")/(("+denominator+")^2)";
    }
    else if(denDer == ""){
        if(sign1=="-") ans = "(- "+denominator+"("+numDer+")" +")/(("+denominator+")^(2))";
        else ans = ans = "("+denominator+"("+numDer+")" +")/(("+denominator+")^(2))";
    }
    else if(sign1=="-"){
        if(sign2=="") ans = "-("+denominator+"("+numDer+")" + " + "+numerator+"("+denDer +")"+")/(("+denominator+")^(2))";
        else ans = "-("+denominator+"("+numDer+")" + " - "+numerator+"("+denDer+")" +")/(("+denominator+")^(2))";
    }
   else if(sign2=="-") ans = "("+denominator+"("+numDer+")" + " + "+numerator+"("+denDer+")" +")/(("+denominator+")^(2))";
    else ans = "("+denominator+"("+numDer+")" + " - "+numerator+"("+denDer+")" +")/(("+denominator+")^(2))";
   return ans;
}

string derivProd(string expr){
    string ans="";
    int j=0, brackets = 0, termCount=0, errhandl=0;
    string temp="", termsList[10];
    while(j<expr.length()+1){
        if(brackets == 0){
            errhandl = j%expr.length();
            if (expr[j-1]==')' or(expr[j-1]=='x' and expr[errhandl-1]!='^')){
                termsList[termCount] = temp;
                termCount++;
                temp.erase();
            }
            //new for const mult
            else if((expr[j]>='0' and expr[j]<='9') or expr[j]=='-'){
                for(int k=j; k<expr.length();k++){
                    if(!(expr[k]>='0' and expr[k]<='9')){
                        termsList[termCount] = temp;
                        termCount++;
                        temp.erase();
                        j=k;
                        break;
                    }
                    else temp+=expr[k];
                }
            }
            //new
        }
        if(expr[j]=='(') brackets++;
        if(expr[j]==')') brackets--;
        temp+=expr[j++];
   }
//   cout<<"DEBUG derivProd 1: termCount ="<<termCount<<endl<<"\ttemp = "<<temp<<endl;
  for(j=0; j<termCount; j++){
    //   cout<<"DEBUG derivProd 2: term ("<<j+1<<") = "<<termsList[j]<<endl;
  }
  // product rule
    temp.erase();
    int signInt;
    for(j=0; j<termCount; j++){
        temp = derivMain(termsList[j]);
        if(temp!=""){
            if(temp[0]=='-'){
                signInt = 1;
                temp.erase(0,1);
            }
            else{
                signInt = 0;
            }
            if(signInt){
                if(j>0) ans+=" - ";
                else ans+="-";
            }
            else{
                if(j>0) ans+=" + ";
            }
            for(int i=0; i<j; i++){
                ans+=termsList[i];
            }
            if(temp!="1" and temp!="-1")    ans+=temp;
            for(int i=j+1; i<termCount; i++){
                ans+=termsList[i];
            }
        }
        temp.erase();
    }
    return ans;
}

string derivExpn(string expr){
    string ans="", subexpr="", expn="";
    int j=0, brackets = 0;
    while(j<expr.length()){
        if(brackets == 0 and expr[j] == '^')    break;
        if(expr[j]=='(') brackets++;
        else if(expr[j]==')') brackets--;
        subexpr+=expr[j++];
   }
   j+=2;
   while(j<expr.length()){
       if(j==expr.length()-1 and expr[j]==')')  break;
       expn+=expr[j++];;
   }
    string coeff="";
   if(subexpr[0]>='0' and subexpr[0] <= '9'){
        string f1="";
        int i=0;
        while(subexpr[i]>='0' and subexpr[i] <= '9') coeff+=subexpr[i++];
        while(i<subexpr.length()) f1+=subexpr[i++];
       subexpr.erase();
       subexpr+=f1;
       f1.erase();
//         cout<<"DEBUG here in derivExpn() 1234, coeff="<<coeff<<endl<<" subexpr="<<subexpr<<endl;
    }
   // by exp rule:
   // (f(x))^(g(x))' = (f(x))^(g(x))(g'(x)log(f(x)) + (g(x)f'(x))/(f(x)))
//   cout<<"DEBUG derivExpn 1: subexpr: "<<subexpr<<endl<<"\t\t\texponent: "<<expn<<endl;
    if (strIsInt(expn)){
        int expnInt = str2int(expn);
        // cout<<"DEBUG derivExpn 2: subexpr: "<<subexpr<<endl<<"\t\t\texponent: "<<expn<<endl;
        if(subexpr.length()==1) return D(expr);
        // cout<<"DEBUG derivExpn 3: subexpr: "<<subexpr<<", exponent: "<<expn<<endl<<"\t\t ans = "<<ans<<endl;
        if(expnInt == 0) ans = "";
        else if(expnInt == 1) ans = coeff+derivMain(subexpr);
        else if(expnInt == 2){
            if(coeff=="" or coeff=="1") ans = expn+"("+subexpr+")("+derivMain(subexpr)+")";
            else ans = int2str(str2int(expn)*str2int(coeff))+"("+subexpr+")("+derivMain(subexpr)+")";
        }
            //ans = int2str(str2int(expn)*str2int(coeff))+"("+subexpr+")("+derivMain(subexpr)+")"; //coeff+"("+expn+")("+subexpr+")("+derivMain(subexpr)+")";
        else{
            if(coeff=="" or coeff=="1")   ans = expn+"("+subexpr+")^("+int2str(expnInt-1)+")("+derivMain(subexpr)+")";
            else ans = int2str(str2int(expn)*str2int(coeff))+"("+subexpr+")^("+int2str(expnInt-1)+")("+derivMain(subexpr)+")";
            //coeff+"("+expn+")("+subexpr+")^("+int2str(expnInt-1)+")("+derivMain(subexpr)+")";
        }
    }
    else if(subexpr.length()==1){
        if(subexpr[0]!='x'){
            string temp = derivMain(expn);
//        cout<<"DEBUG derivExpn expr="<<expr<<" | temp="<<temp<<endl;
            if(temp=="1")   ans+=D(expr);
            else if(temp=="")   ans+=D(expr);
            else ans+=D(expr)+"("+temp+")";
        }
        else{
            string temp = derivMain(expn);
        // cout<<"DEBUG derivExpn 678676 expr="<<expr<<" | temp="<<temp<<endl;
            if(temp=="1" or temp=="-1" or temp=="- 1")   ans+=expr+"(log(x) + 1)";
            // else  ans+=expr+"("+temp+")";
            else ans+=D(expr)+"(xlog("+subexpr+")("+temp+") + "+expn+")";
        }
    }
    else {
        string temp = derivMain(expn), temp1=derivMain(subexpr);
        if(subtermCount(temp)>1){
            if(subtermCount(temp1)>1) ans = expr + "(("+ temp +")log(" + subexpr + ") + ("+expn+"("+temp1+")"+")/("+subexpr+"))";
            else ans = expr + "(("+ temp +")log(" + subexpr + ") + ("+expn+temp1+")/("+subexpr+"))";
        }
        else if(subtermCount(temp1)>1) ans = expr + "("+ temp +"log(" + subexpr + ") + ("+expn+"("+temp1+")"+")/("+subexpr+"))";
        else ans = expr + "("+ temp +"log(" + subexpr + ") + ("+expn+temp1+")/("+subexpr+"))";
    
        // ans = expr + "("+ derivMain(expn) +"log(" + subexpr + ") + ("+expn+derivMain(subexpr)+")/("+subexpr+"))";
        
    }
//   cout<<"DEBUG derivExpn 4: subexpr: "<<subexpr<<", exponent: "<<expn<<endl<<"\t\t ans = "<<ans<<endl;
   return ans;
}

string derivMain(string input){
//    cout<<"DEBUG derivMain 1.0. Finding derivative of input:"<<input<<endl;
    string expr = RemoveParentheses(input);
//    cout<<"DEBUG derivMain 1. Finding derivative of:"<<expr<<endl;
   string ans="", subexpr;
   int i=0, j, brackets = 0;
   int termCount = 0;
   bool quot=0, expn=0;
   while(i<expr.length()){
       j=i;
       subexpr = "";
       quot=0;
       expn=0;
       while((expr[j]!=' ' or brackets!=0) and j<expr.length()){
           subexpr+=expr[j];
           if(expr[j]=='/' and brackets==0) quot=1;
           if(expr[j]=='^' and brackets==0) expn=1;
           else if(expr[j]=='(') brackets++;
           else if(expr[j]==')') brackets--;
           j++;
       }
       termCount++;
       string temp;
       if(j>=expr.length() and termCount==1)    break;
       if (quot == 1) temp=derivQuot(subexpr);
       else if(expn == 1) temp=derivExpn(subexpr);
       else if(subexpr.length()==1) temp += D(subexpr);
       else temp = derivMain(subexpr);
       if (temp=="") temp = D(subexpr);
//       if (temp=="") cout<<"DEBUG deriv 18798798: subexpr: "<<subexpr<<endl;
       if(temp[0]=='-'){
           temp.erase(0,1);
           if(ans.length()>0){
               if(ans[ans.length()-2]=='+'){
                   ans[ans.length()-2] = '-';
                   ans+=RemoveParentheses(temp);
               }
               else if(ans[ans.length()-2]=='-'){
                   ans[ans.length()-2] = '+';
                   ans+=RemoveParentheses(temp);
               }
           }
           else ans+="-"+RemoveParentheses(temp);
       }
       else ans+=RemoveParentheses(temp);
    //  cout<<"DEBUG deriv 1: subexpr: "<<subexpr<<endl;
       subexpr.erase();
       if(temp!="") while(expr[j]==' ' or expr[j]=='+' or expr[j]=='-')  ans+=expr[j++];
       else{
           while(expr[j]==' ' or expr[j]=='-') ans+=expr[j++];
           while(expr[j]==' ' or expr[j]=='-') ans+=expr[j++];
       }
//       if(temp!="") while(expr[j]==' ' or expr[j]=='+' or expr[j]=='-')  ans+=expr[j++];
//       else while(expr[j]==' ' or expr[j]=='+' or expr[j]=='-') j++;
//       else while(expr[j]==' ' or expr[j]=='+' or expr[j]=='-'){
//           if(expr[j]=='-'){
//               ans+=expr[j++];
//               if(expr[j]==' ') ans+=expr[j++];
//           }
//           else j++;
//       }
       i=j;
   }
//    cout<<"DEBUG derivMain 85: subexpr: "<<subexpr<<"| termcount "<<termCount<<endl;
   if (termCount==1){
       if(quot == 1) ans+=derivQuot(subexpr);
       else if(expn == 1) ans+=derivExpn(subexpr);
       else if(subexpr.length()==1) ans += D(subexpr);
       else{
           if(subtermCount(subexpr) == 1){
            //   checks if composite function or not
               for(int i=0; i<subexpr.length(); i++){
                   if(subexpr[i]=='('){
                       if(subexpr[i+1]=='x' and subexpr[i+2]==')'){
                           ans+=D(subexpr); //not a composite function
//                           cout<<"DEBUG derivMain lastloop1: ans = "<<ans<<endl;
                           break;
                       }
                       else{
                           // case of composite function
                           string temp="";
                           brackets++;
                           i++;
                           while(brackets!=0 and i<subexpr.length()-1){
                               if(subexpr[i]=='(') brackets++;
                               else if(subexpr[i]==')') brackets--;
                               temp+=subexpr[i++];
                           }
//                           cout<<"DEBUG derivMain lastloop2: temp="<<temp<<endl;
                           int case1=0;
                           for(j=0; j<temp.length();j++){
                               if(temp[j]<'0' or temp[j]>'9'){
                                   if(temp[j]!='x') {
                                       if(temp[j]=='^'){
                                           for(int k = j+2; temp[k]!=')';k++){
                                               if(temp[k]<'0' or temp[k]>'9'){
                                                   case1=1;
                                                   break;
                                               }
                                           }
                                           break;
                                       }
                                       else case1=1;
                                   }
                               }
                           }
//                           cout<<"DEBUG derivMain lastloop3: temp="<<temp<<endl;
//                           cout<<"DEBUG derivMain lastloop 0909: case1="<<case1<<"| expr="<<expr<<endl;
                           if(case1==1){
                               string temp2 = derivMain(temp);
                               if(temp2 == "") ans+=D(subexpr)+"("+D(temp)+")";
                               else ans+=D(subexpr)+"("+derivMain(temp)+")";
                           }
                           else {
                               string part1 = derivMain(temp), part2=D(subexpr), signpart="";
                               if(part1[0]=='-'){
                                   signpart+='-';
                                   part1.erase(0,1);
                               }
                               if(part2[0]=='-'){
                                   if(signpart=="-") signpart.erase();
                                   else signpart+='-';
                                   part2.erase(0,1);
                               }
                               ans+=signpart+"("+part1+")"+"("+part2+")";
                           }
                           break;
                       }
                   }
               }
           }
           else ans+=derivProd(subexpr);
       }
   }
    return ans;
}

int subtermCount(string expr){
    int j=0, brackets = 0, termCount=0, errhandl=0;
    while(j<expr.length()+1){
        if(brackets == 0){
            errhandl = j%expr.length();
            if (expr[j-1]==')' or(expr[j-1]=='x' and expr[errhandl-1]!='^')){
                termCount++;
            }
            //new for const mult
            else if((expr[j]>='0' and expr[j]<='9') or expr[j]=='-'){
                for(int k=j; k<expr.length();k++){
                    if(!(expr[k]>='0' and expr[k]<='9')){
                        termCount++;
                        j=k;
                        break;
                    }
                }
            }
            //new
        }
        if(expr[j]=='(') brackets++;
        else if(expr[j]==')') brackets--;
        j++;
   }
//   cout<<"DEBUG subtermCount, expr="<<expr<<" | termCount = "<<termCount<<endl;
    return termCount;
}

// ----- end calculus functions

int main(){
    printf("OOSD Final Assignment\n---------------------\n");
    printf("Name:     Maitreyi Swaroop\nRoll No.: 19MA20065");
    printf("\n---------------------\nInstructions\n");
    printf("1. Ensure that all statements are correctly parenthesised.\n2. All arguments to functions must be put within parentheses, i.e. write sin(x) NOT sinx and log(x) NOT logx.");
    printf("\n3. All powers/exponents must be put within parentheses, i.e. write e^(x), x^(7) NOT e^x, x^7");
    printf("\n4. All + and - signs must have a space around them, i.e. x + tan(x^(10) - sin(x^(3) + x))");
//    printf("\n---------------------\nExamples:\n");
    printf("\n---------------------\nYour input:\n");
    string expr = "";
    // file handling portion
    fstream myfile;
    myfile.open("input.txt");
    char c='0';
    myfile.get(c); // reads a character
    //int count=0;
    while (c!=EOF){
        if(c=='\n'){
            cout<<expr; printDeriv(expr);
//            cout<<"\nEg."<<(count++)+1<<"."; printDeriv(expr);
            expr.erase();
            break;
        }
        expr+=c;
        myfile.get(c); // reads a character
    }
    return 0;
}

// ----- utility functions (body) here
int strIsInt(string s){
   // checks if the string is a representation of an integer.
    int i = 0;
    while(i<s.length()){
        if(s[i]-'0' > 9) return 0;
        i++;
    }
    return 1;
}

int str2int(string s){
    int i = 0, x = 0;
    int sign = 1;
    if(s[i]=='-'){
        sign=-1;
        i++;
        if(s.length()==1){
            return sign*1;
        }
    }
    while(i<s.length()){
        x*=10;
        x+=s[i]-'0';
        i++;
    }
    return x*sign;
}
string int2str(int x){
    string ans = "";
    string temp = "";
    if(x<0){
        ans+='-'; x*=-1;
    }
    int i=1;
    while(x!=0){
        temp+= '0'+x%10;
        x/=10;
    }
    for(i=temp.length()-1;i>=0;i--) ans+=temp[i];
    return ans;
}

void printD(string expr){
    cout<<"(d/dx)("<<(expr)<<")"<<endl<<"= "<<D(expr)<<endl;
}
void printDeriv(string expr){
    string enter = pruneRemove(addReqSpaces(RemoveParentheses(expr)));
    if(checkExpr(enter)==0) return;
    string ans = derivMain(enter);
    cout<<endl<<"____________________________________________"<<endl;
    cout<<"Evaluating derivative with respect to x:"<<endl;
    cout<<"(d/dx)("<<(expr)<<")"<<endl<<"= "<<pruneRemove(RemoveParentheses(ans))<<endl;
//    cout<<"(d/dx)("<<(expr)<<")"<<endl<<"= "<<derivMain(enter)<<endl;
    cout<<"____________________________________________"<<endl;
    return;
}

bool checkExpr(string expr){
    bool check = 1;
    if (expr[0]==' '){
        cout<<"\tRemove whitespace from expression start."<<endl;
        check = 0;
    }
    int brackets=0;
    for(int i=0; i<expr.length(); i++){
        if(expr[i]==' '){
            if(i+1<expr.length()){
                if(expr[i+1]==' ') {
                    cout<<"\tCheck whitespaces. Consecutive whitespaces found."<<endl;
                    check = 0;
                }
            }
        }
        else if(expr[i]=='(') brackets++;
        else if(expr[i]==')') brackets--;
    }
    if(brackets!=0) {
        cout<<"\tCheck parentheses.";
        if(brackets>0) cout<<" Extra '(' found.";
        else if (brackets<0) cout<<" Extra ')' found."<<endl;
        check = 0;
    }
    return check;
}

string pruneRemove(string expr){
    string ans="";
    int j;
    for(int i=0; i<expr.length(); i++){
        if(expr[i]=='('){
            j=i+1;
            while(expr[j]==' ' or expr[j]=='+') j++;
            ans+=expr[i];
            i+=j-(i+1);
        }
        else if(expr[i]==' '){
            j=i+1;
            while(expr[j]==' ' or expr[j]=='+' or expr[j]=='-') j++;
            if(expr[j]==')') i+=j-(i+1);
            else ans+=expr[i];
        }
        else ans+=expr[i];
    }
    return ans;
}

string addReqSpaces(string expr){
    string ans="";
    for(int i=0; i<expr.length(); i++){
        if(expr[i]=='+'){
            if(expr[i-1]!=' '){
                ans+=" ";
            }
            ans+=expr[i];
            if(expr[i+1]!=' '){
                ans+=" ";
            }
        }
        else if(expr[i]=='-'){
            if(expr[i-1]!=' ' and expr[i-1]!='('){
                ans+=" ";
            }
            ans+=expr[i];
            if(expr[i+1]!=' '){
                ans+=" ";
            }
        }
        else ans+=expr[i];
    }
    return ans;
}
// ----- end utility functions (body) here

// ----- stack functions (body)
// the stack functions here make use of the linked list code shared with the class by Prof. Bodhayan Roy
void Stack::push(int d){
    node *newnode = new node;

    newnode-> data = d;
    newnode-> prev = NULL;
    newnode->next = head;

    if(head!=NULL){
        head->prev = newnode;
    }
    head = newnode;
    
    if (newnode->next!=NULL and (newnode->next)->next==NULL){
        tail=newnode->next;
    }
}

void Stack::pop(){
    if(head==NULL){
    }
    else if (head->next==NULL){
        head = NULL;
    }
    else{
        head=head->next;
        delete (head->prev); // deletes the erstwhile head
        head->prev = NULL;
    }
}

 
void Stack::display(){
    node* current = head;
    while(current != NULL){
        cout << current->data;
        current = current->next;
    }
//    cout<< endl;
}

void Stack::displayAsList(){
    node* current = head;
    while(current != NULL){
        cout << current->data;
        if(current->next != NULL){
            cout<<"->";
        }
        current = current->next;
    }
    cout<< endl;
}

int Stack::headData(){
    node* current = head;
    return current->data;
}

string RemoveParentheses(string s){
    // quick check to see whether it is a quotient
    int brackets=0,quot=0;
    for(int i=0;i<s.length();i++){
        if(s[i]=='(')   brackets++;
        else if(s[i]==')')   brackets--;
        if(brackets==0 and s[i]=='/'){
            quot=1;
            break;
        }
    }
    if(quot)    return s;
    string ans = "", dummy = s;
    Stack exprStack;
    int top =0;
    int i = 0;
    while(i < s.length()){
        if(s[i] == '('){
            if(s[i+1] == '(')   exprStack.push(-i);
            else    exprStack.push(i);
        }
        else if(s[i] == ')'){
            top = exprStack.headData();
            if(s[i-1] == ')' and top <= 0){
                dummy[-top] = '!';
                dummy[i] = '!';
                exprStack.pop();
            }
            else if(s[i-1] != ')' and top > 0)  exprStack.pop();
        }
        i++;
    }
    for(i = 0; i<dummy.length();i++){
        if(dummy[i]!='!')   ans+=dummy[i];
    }
//    exprStack.display();
    return ans;
}
// ----- end of stack functions (body)



//
//  main.cpp
//  LexicalAnal
//
//  Created by 양준식 on 2020/04/20.
//  Copyright © 2020 양준식. All rights reserved.
//

#include <iostream>
using namespace std;
#include <string>
#include <fstream>
#include <map>
#include <queue>
#include "MergedTable.h"
#include "symbolMap.h"

//to store returned tokens for printing out at final
queue<string> gl_queue; // " TokenName | lexeme "을 하나의 원소로 하는 큐. output파일에 출력할 이 원소들을 다 갖고있다가, input파일을 다 읽고 output파일에 써줄것임.
queue<char> lex_queue;// input파일을 읽는 동안, TOKEN으로 분류되기 전까지의 문자열을 저장해놓을 큐. 특정 토큰으로 확정되면 바로 lex_queue에 있는 원소들을 모두 꺼내어 TOKEN으로 분류해줄 것.

//to print out result in one time after termination of lexical analyzer.

void result_print(string input_file_name) { // "input.txt"  //분류되어 gl_queue 에 저장된 "TOKENNAME | LEXEME"을 모두 output파일에 출력해주는 함수!
    string result_name = input_file_name.substr(0, input_file_name.size()-4) + "_out.txt";
    cout << result_name << endl;
    ofstream result_file(result_name);
    string lex_to_syntax;
    int orposition=0;
    string forarith;
    while(!gl_queue.empty()) {//큐에 있는 모든걸 다 출력
        string write_result = gl_queue.front();
        gl_queue.pop();
        //우리는 이곳에서 WS를 제거해준다. output파일에 공백문자가 필요없으므로!
        if(write_result=="WS |" || write_result=="WS | " || write_result=="WS |  " || write_result=="WS |   " || write_result=="WS |    " || write_result=="WS |\t" ||write_result=="WS |\n" || write_result=="WS | \t" ||write_result=="WS | \n"){
            continue;
        }
        
    //////////////////신택스 애널라이저를 위해 변경한 부분//////////////
      
        for(int leng=0;leng<write_result.length();leng++){
            if(write_result[leng]=='|'){
                orposition=leng;
                break;
            }
        }
        
        lex_to_syntax="";
      
        
        for(int u=0;u<orposition;u++){
            if(write_result[u]!=' '){
            lex_to_syntax+=write_result[u];
            }
        
        }
          forarith="";
        
        for(int fa=0;fa<write_result.length();fa++){
            if(write_result[fa]!=' '){
                forarith+=write_result[fa];
            }
        }
        
        
        
        
        ////////////////// 여기까지가 VAR | int  를    "VAR"로  공백도 없이 딱 이름만 잘라준 것
        ///
        ///
        ///
        ////////여기는 이제 VAR -> vtype 과 같은 작업을 해주는, 출력 전 마지막 작업
        if( lex_to_syntax=="VAR"){
            lex_to_syntax="vtype";
        }
        else if(lex_to_syntax=="INT" ){
            lex_to_syntax="num";
        }
        else if(lex_to_syntax== "FLOAT"){
            lex_to_syntax="float";
        }
        else if(lex_to_syntax== "STRING"){
            lex_to_syntax="literal";
        }
        else if(lex_to_syntax=="ID" ){
            lex_to_syntax="id";
        }
        else if(lex_to_syntax=="IF" ){
            lex_to_syntax="if";
        }
        else if(lex_to_syntax=="ELSE" ){
            lex_to_syntax="else";
        }
        else if(lex_to_syntax=="WHILE" ){
            lex_to_syntax="while";
        }
        else if(lex_to_syntax=="FOR" ){
            lex_to_syntax="for";
        }
        else if(lex_to_syntax=="RETURN" ){
            lex_to_syntax="return";
        }
        else if(lex_to_syntax=="ARITHOP" ){
            
            if(forarith.back()=='+' || forarith.back()=='-'){
                lex_to_syntax="addsub";
            }
            else{
                lex_to_syntax="multdiv";
            }
        }
        else if(lex_to_syntax=="ASGN" ){
            lex_to_syntax="assign";
        }
        else if(lex_to_syntax=="COMPOP" ){
            lex_to_syntax="comp";
        }
        else if(lex_to_syntax=="LBRACE" ){
            lex_to_syntax="lbrace";
        }
        else if(lex_to_syntax=="RBRACE" ){
            lex_to_syntax="rbrace";
        }
        else if(lex_to_syntax=="LPAREN" ){
            lex_to_syntax="lparen";
        }
        else if(lex_to_syntax=="RPAREN" ){
            lex_to_syntax="rparen";
        }
        else if(lex_to_syntax=="COMMA" ){
            lex_to_syntax="comma";
        }
        else if(lex_to_syntax=="TERM" ){
            lex_to_syntax="semi";
        }
      
        
        result_file << lex_to_syntax;
        if(!gl_queue.empty()){
            result_file <<endl;
        }
    }
    result_file.close();
    cout << "output file done" << endl;
}



//actual lexical analyzing function.
void lexical_analyzer(char temp,int* former_state, int* current_state) { //actual lexical analyzing function. // temp에는,  main함수에서 input파일의 글자를 하나씩 가져오고(temp), 이전 state와 현재 state를 가져와서,  temp로 인해 현재 state가 어디로 가는지 계산한다.( Merged Table이용)
                      
  
    *former_state = *current_state;
    *current_state= MergedTable[*current_state][m[temp]];
   ////////////////////////////////////////////밑의 cout 세줄은 오류를 잡기위해 써놓은 코드이므로 지워도 프로그램 돌아가는 것과 무관하다.//////////////////////////////////////////////////////////////////////////////////
    cout << "lexical_analyzer __ ";
    cout << "former state : " << *former_state << "  current state : " << *current_state;
    cout << "   character : " << temp << endl;
////////////////////////////////////////////위의 cout 세줄은 오류를 잡기위해 써놓은 코드이므로 지워도 프로그램 돌아가는 것과 무관하다.//////////////////////////////////////////////////////////////////////////////////
    ///
   ///.  된 과 안된. 으로 state들을 구분하여 작성하였다. '된'은 (,),{,},COMPOP@,BITOP@와 같이 , 해당 state에 있다는 것이 100% 해당 state의 토큰으로 분류되었다는 것을 보장하는 state들을 의미.
    // '안된'으로 분류되는 state들은 (INT | IF | ID) ID @ state와 같이 아직 어떤 Token으로 분류될 지 결정되지 않은 상태에 있는 state 들을 의미함.
    
    // if, elseif 문을 작성한 순서는 , 가장먼저 T0->된, T0->안된. 그리고나서 1.된->된 state이동.  2. 된->안된 state이동. 3. 안된->된 state이동. 4. 안된->안된 state이동.   간단한 순으로 작성하였다.
    // 아래 조건문에서 작성한 former_state, current_state 쌍이. DFA(MergedTable) 상에 아예 존재하지 않는 쌍도 있다. 이는 코드작성과정에서 편의를 위해 그냥 적었다.
  // 첫 문자를 읽기 전, 맨 처음의 state는 mergedTable에 써있듯 T0이다.
  if (*current_state == 69){//FAIL state로 들어간 경우
      
      while(!lex_queue.empty()) {
          lex_queue.pop();//렉스큐에 있는 모든 문자들을 버리고, 프로그램 종료
      }
      cout<<" input_file has error"<<endl;
      exit(0);
  }
  // T0 state에서 출발을 한 경우. T0->된 state들. 된 state들은 바로 분류가 되기 때문에, 바로 gl_queue에 넣어준다.
  else if( *former_state==0 && (*current_state == 1 || *current_state ==2 || *current_state ==3 || *current_state ==4 || *current_state ==5||*current_state ==6||*current_state ==8||*current_state ==9||*current_state ==12||*current_state ==13||*current_state ==59||*current_state ==60)){
        string tmp=s[*current_state] + " | ";
        tmp.append(1,temp);
        gl_queue.push(tmp); //queue.push()함수를 이용하여 분류된  "토큰 | lexeme" 형식의 string원소를 넣어줄 것이다.
      }
    /////////. T0->안된state들   안된 state들은 아직 어느 토큰이 될 지 모르기때문에 그냥 lex_queue에 temp를 넣어주기만 하면 된다.
  else if(( *former_state==0 )&&(*current_state != 1 && *current_state != 2 && *current_state != 3 && *current_state != 4 && *current_state != 5 && *current_state != 6 && *current_state != 8 && *current_state != 9 && *current_state != 12 && *current_state != 13 && *current_state != 59 && *current_state != 60)){
        lex_queue.push(temp);
    }
    
    //된->된 부터//////// 앞의 된state는 이전 단계에서 이미 분류되어 gl_queue로 출력이 되었을 것이다. 따라서 새로 온 된 state만 분류해서 gl_queue에 넣어주면 된다.
      else if ( (*former_state == 1 || *former_state == 2 || *former_state == 3 || *former_state == 4 || *former_state == 5 ||*former_state ==  6 || *former_state == 8 || *former_state == 9 || *former_state == 12 || *former_state == 13 || *former_state == 59 || *former_state == 60)&& (*current_state == 1 || *current_state == 2 || *current_state == 3 || *current_state == 4 || *current_state == 5 ||*current_state ==  6 || *current_state == 8 || *current_state == 9 || *current_state == 12 || *current_state == 13 || *current_state == 59 || *current_state == 60)){
          string tmp=s[*current_state] + " | ";
          tmp.append(1,temp);
          gl_queue.push(tmp);
             }
  
  
  
  //된 -> 안된으로 가는 경우. 아직 현재 state가 어떤 token으로 분류될 지 모르기 때문에 queue 에 어떤 것도 넣어주지 않는다. 단지 lex-queue에 읽은 문자를 넣어주면 됨.
     else if ( (*former_state == 1 || *former_state == 2 || *former_state == 3 || *former_state == 4 || *former_state == 5 ||*former_state ==  6 || *former_state == 8 || *former_state == 9 || *former_state == 12 || *former_state == 13 || *former_state == 59 || *former_state == 60) && (*current_state != 1 && *current_state != 2 && *current_state != 3 && *current_state != 4 && *current_state != 5 && *current_state != 6 && *current_state != 8 && *current_state != 9 && *current_state != 12 && *current_state != 13 && *current_state != 59 && *current_state != 60)){
          lex_queue.push(temp);
          
          }
  

  
  //안된->된으로 가는 경우. 안된 state의 바로 종료 시의 state(헤더파일에서 map s에 정의해놓은 것처럼)를 gl_queue에 넣어주고, 새로 들어온 된 state 또한 gl_queue에 넣어준다.
         else if( (*former_state == 16 || *former_state ==17 || *former_state ==20 ||*former_state == 21 || *former_state ==22 || *former_state ==24 || *former_state ==25 || *former_state ==26 || *former_state ==27 || *former_state ==28 || *former_state ==29 || *former_state ==31 || *former_state ==32 || *former_state ==33 || *former_state ==34 || *former_state ==35 || *former_state ==36 || *former_state ==38 || *former_state ==39 || *former_state ==40 || *former_state ==42 ||*former_state == 43 || *former_state ==44 || *former_state ==46 || *former_state ==47 || *former_state ==48 || *former_state ==49 || *former_state ==50 || *former_state ==52 || *former_state ==53 ||*former_state ==54 || *former_state ==55 || *former_state ==57||*former_state ==70)&& (*current_state == 1 || *current_state == 2 || *current_state == 3 || *current_state == 4 || *current_state == 5 ||*current_state ==  6 || *current_state == 8 || *current_state == 9 || *current_state == 12 || *current_state == 60)){ //끝나면 ID인 상태에서 된 state로 갔을 때. 둘 다 gl_queue에 넣어줘야함
    string tmp = "ID | ";                                                                         // 13, 59번은 해당 former-state에서 갈 수가 없어서 안 써준거임
    while(true){
        if(!lex_queue.empty()){
            tmp.append(1,lex_queue.front());
            lex_queue.pop();
        }
        else{
            break;
        }
    
    }
    gl_queue.push(tmp);//앞에꺼는 완료
    
    string tmp2 =s[*current_state] + " | ";
    tmp2.append(1,temp);
    gl_queue.push(tmp2); //현재 들어온 state까지 queue에 입력 완료.
    
    
    
}
  else if( (*former_state ==18 || *former_state ==19 || *former_state ==23 || *former_state ==30 || *former_state ==37 || *former_state ==41 ||*former_state == 45 || *former_state ==51 || *former_state ==56 ) && (*current_state == 1 || *current_state ==2 || *current_state ==3 ||*current_state == 4 || *current_state ==5 || *current_state ==6 || *current_state ==8 || *current_state ==9 || *current_state ==12 ||*current_state == 13 || *current_state ==59 || *current_state ==60)) { //끝나면 ID인 상태에서 된 state로 갔을 때. 둘 다 gl_queue에 넣어줘야함
  
  string tmp = s[*former_state] +" | ";                                                                         // 13, 59번은 해당 former-state에서 갈 수가 없어서 안 써준거임
     while(true){
         if(!lex_queue.empty()){
             tmp.append(1,lex_queue.front());
             lex_queue.pop();
         }
         else{
             break;
         }
     
     }
     gl_queue.push(tmp);
      
          string tmp2 =s[*current_state] + " | ";
      tmp2.append(1,temp);
      gl_queue.push(tmp2); //현재 들어온 state까지 queue에 입력 완료.
      
  }
  else if( (*former_state == 10 || *former_state ==11 || *former_state ==15 ) && (*current_state == 1 || *current_state ==2 || *current_state ==3 ||*current_state == 4 || *current_state ==5 || *current_state ==6 || *current_state ==8 || *current_state ==9 || *current_state ==12 || *current_state ==59 || *current_state ==60)) { //끝나면 ID인 상태에서 된 state로 갔을 때. 둘 다 gl_queue에 넣어줘야함
      
      if((*former_state == 10 || *former_state == 11) && *current_state == 12 ){// < 나 >에서 < > & | 중 하나가 들어온 경우.
          if(temp==('<' | '>')){
              string tmp = "BITOP | ";
              tmp.append(2,temp);
              gl_queue.push(tmp);
              lex_queue.pop();
              
          }
          else{// &나 |가 들어온 경우.
              string tmp = "COMPOP | ";
              tmp.append(1,lex_queue.front());
              gl_queue.push(tmp);
              lex_queue.pop();
              string tmp2 = "BITOP | ";
              tmp2.append(1,temp);
              gl_queue.push(tmp2);
              
          }
          
          
             
          }
      
      else{//나머지 모든 일반적인 경우.
                       string tmp = s[*former_state]+" | ";
                    while(true){
                        if(!lex_queue.empty()){
                            tmp.append(1,lex_queue.front());
                            lex_queue.pop();
                        }
                        
                        else{
                            break;
                        }
                    
                    }
                    gl_queue.push(tmp);//앞에꺼는 완료
                    
                    string tmp2 =s[*current_state] + " | ";
                    tmp2.append(1,temp);
                    gl_queue.push(tmp2); //현재 들어온 state까지 queue에 입력 완료.
                    
                   }
          
      }
      
      else if((*former_state == 10 || *former_state ==11 || *former_state ==15 ) && *current_state == 13){// < > =에서 =을 받은 경우
          if(*former_state == 10 || *former_state ==11){
              string tmp = " COMPOP | ";
              tmp.append(1,lex_queue.front());
              lex_queue.pop();
              tmp.append(1,temp);//=을 붙여줌
              gl_queue.push(tmp);
          }
          else {  // 15 state에서 13 state로 간 경우
              string tmp = " COMPOP | ";
              tmp.append(1,lex_queue.front());
              lex_queue.pop();
              tmp.append(1,temp);//=을 붙여줌
              gl_queue.push(tmp);
          }
      }
      
      
      
       
   
  // 안도니->된 중 특이한 놈들
  else if( *former_state == 7 && *current_state == 9) { // \만 있는 상태에서 n 혹은 t가 들어와서 공백이 된 경우
    string tmp = " WS | ";
                   tmp.append(1,lex_queue.front());
                   lex_queue.pop();
                   tmp.append(1,temp);//t 혹은 n을 붙여줌
                   gl_queue.push(tmp);
  }
  
  else if( *former_state == 14 && *current_state == 13) { // !에서 =이 들어온 경우
   string tmp = " COMPOP | ";
                  tmp.append(1,lex_queue.front());
                  lex_queue.pop();
                  tmp.append(1,temp);//=을 붙여줌
                  gl_queue.push(tmp);
     }
  
  else if( *former_state == 58 && *current_state == 59) {// "..... 의 상태에서 "이 들어와 string이 완성된 경우
   string tmp = " STRING | ";
      while(true){
          if(!lex_queue.empty()){
              tmp.append(1,lex_queue.front());
              lex_queue.pop();
          }
          
          else{
              break;
          }
      
      }
                 
                  tmp.append(1,temp);//"을 붙여줌
                  gl_queue.push(tmp);
     }
 
  
  
            //////////// 안된-> 된 끝
           
  
  
  //마지막 안된-> 안된.   -- 이 부분이 가장 까다롭다.
   else if((*former_state == 16 || *former_state ==17 || *former_state ==20 || *former_state ==21 || *former_state ==22 ||*former_state ==24 || *former_state ==25 ||*former_state ==26 || *former_state ==27 || *former_state ==28 || *former_state ==29 || *former_state ==31 || *former_state ==32 || *former_state ==33 ||*former_state == 34 || *former_state ==35 || *former_state ==36 ||*former_state ==38 || *former_state ==39 || *former_state ==40 || *former_state ==42 || *former_state ==43 || *former_state ==44|| *former_state ==46||*former_state ==47||*former_state ==48||*former_state ==49||*former_state ==50||*former_state ==52||*former_state ==53||*former_state ==54||*former_state ==55||*former_state ==57||*former_state ==70)   &&(*current_state == 18||*current_state ==19||*current_state ==23||*current_state ==30||*current_state ==37||*current_state ==41||*current_state ==45||*current_state ==51||*current_state ==56)) {//끝나면 ID인 state들에서 IF,ELSE등 특수 state들로 간 경우, 다시 ID state로 갈 수도 있으므로 lex_queue에 추가만 해둔다.
      lex_queue.push(temp);
   }
  //끝나면 ID 인 state들에서 안된state로 가는데, 다시 ID로 돌아올 수 없는 state인 경우, ID로 분류해주고, 뒤에 것은 렉스큐에 추가만 해줌.
  else if((*former_state == 16 || *former_state ==17 || *former_state ==20 || *former_state ==21 || *former_state ==22 ||*former_state ==24 || *former_state ==25 ||*former_state ==26 || *former_state ==27 || *former_state ==28 || *former_state ==29 || *former_state ==31 || *former_state ==32 || *former_state ==33 ||*former_state == 34 || *former_state ==35 || *former_state ==36 ||*former_state ==38 || *former_state ==39 || *former_state ==40 || *former_state ==42 || *former_state ==43 || *former_state ==44|| *former_state ==46||*former_state ==47||*former_state ==48||*former_state ==49||*former_state ==50||*former_state ==52||*former_state ==53||*former_state ==54||*former_state ==55||*former_state ==57||*former_state ==70)   && ( *current_state == 7||*current_state == 10||*current_state == 11||*current_state == 14||*current_state == 15||*current_state == 58)) {
        string tmp = " ID | ";
             while(true){
                 if(!lex_queue.empty()){
                     tmp.append(1,lex_queue.front());
                     lex_queue.pop();
                 }
                 
                 else{
                     break;
                 }
             
             }
                        
                  gl_queue.push(tmp);
                 lex_queue.push(temp);
      
            }
    else if( (*former_state == 16 || *former_state ==17 || *former_state ==20 || *former_state ==21 || *former_state ==22 ||*former_state ==24 || *former_state ==25 ||*former_state ==26 || *former_state ==27 || *former_state ==28 || *former_state ==29 || *former_state ==31 || *former_state ==32 || *former_state ==33 ||*former_state == 34 || *former_state ==35 || *former_state ==36 ||*former_state ==38 || *former_state ==39 || *former_state ==40 || *former_state ==42 || *former_state ==43 || *former_state ==44|| *former_state ==46||*former_state ==47||*former_state ==48||*former_state ==49||*former_state ==50||*former_state ==52||*former_state ==53||*former_state ==54||*former_state ==55||*former_state ==57||*former_state ==70)   && (*current_state == 16 || *current_state ==17 || *current_state ==20 || *current_state ==21 || *current_state ==22 ||*current_state ==24 ||*current_state == 25 ||*current_state ==26 || *current_state ==27 || *current_state ==28 || *current_state ==29 ||*current_state == 31 || *current_state ==32 || *current_state ==33 || *current_state ==34 || *current_state ==35 || *current_state ==36 ||*current_state ==38 ||*current_state ==39 || *current_state ==40 || *current_state ==42 || *current_state ==43 || *current_state ==44|| *current_state ==46||*current_state ==47||*current_state ==48||*current_state ==49||*current_state ==50||*current_state ==52||*current_state ==53||*current_state ==54||*current_state ==55||*current_state ==57||*current_state ==70) ) {//끝나면 ID인 state들에서 IF,ELSE등 특수 state들로 간 경우, 다시 ID state로 갈 수도 있으므로 lex_queue에 추가만 해둔다.
           lex_queue.push(temp);
        }
       
  
  else if( *former_state==70 && *current_state==65){// 숫자로 끝나는 ID_NUM에서 Decimal piint가 찍혔을때 맨 마지막 숫자를 다음 FLOAT을 위해 내준다.
    
      string tmp = " ID | ";
      while(true){
          
          if(!lex_queue.empty()){
              tmp.append(1,lex_queue.front());
              lex_queue.pop();
              if(lex_queue.size()==1){
                  break;
              }
          }
          
          else{
                      
              break;
          }
      
      }
                 
           gl_queue.push(tmp);
      
       lex_queue.push(temp);
  }
  
       else if((*former_state == 18||*former_state == 19||*former_state == 23||*former_state == 30||*former_state == 37||*former_state == 41||*former_state == 45||*former_state == 51||*former_state == 56) && (*current_state == 57 ||*current_state == 70)){// IF, ELSE, true, BOOL과 같은 ID가 될 가능성이 있는 지정어 state에서 다시 ID가 되는 경우, 렉스큐에 추가만 해줌
           lex_queue.push(temp);
           
       }
  else if((*former_state ==18||*former_state ==19||*former_state ==23||*former_state ==30||*former_state ==37||*former_state ==41||*former_state ==45||*former_state ==51||*former_state ==56)   && (*current_state ==7||*current_state ==10||*current_state ==11||*current_state ==14||*current_state ==15||*current_state ==58) ){
      string tmp = s[*former_state]+" | ";//IF, ELSE, true, BOOL과 같은 ID가 될 가능성이 있는 지정어 state에서 ID가 아닌 다른 곳으로 가, 지정어가 분류되어야 하는 경우. 분류해주고 ,뒤에것은 렉스큐에 추가만.
             while(true){
                 if(!lex_queue.empty()){
                     tmp.append(1,lex_queue.front());
                     lex_queue.pop();
                 }
                 
                 else{
                     break;
                 }
             
             }
                        
                  gl_queue.push(tmp);
                 lex_queue.push(temp);
      
  }
  
  
 else if((*former_state ==18||*former_state ==19||*former_state ==23||*former_state ==30||*former_state ==37||*former_state ==41||*former_state ==45||*former_state ==51||*former_state ==56)   && *current_state ==61 ){ //IF, ELSE, true, BOOL과 같은 ID가 될 가능성이 있는 지정어 state에서 -가 들어왔을 때
     
     string tmp = s[*former_state]+" | ";
     while(true){
         if(!lex_queue.empty()){
             tmp.append(1,lex_queue.front());
             lex_queue.pop();
         }
         
         else{
             break;
         }
     
     }
                
          gl_queue.push(tmp);
         lex_queue.push(temp);
     
 }
  
  else if(*former_state ==58 && *current_state ==58 ){
      
      lex_queue.push(temp);
      
      
  }
  
  else if((*former_state == 10 ||*former_state ==11) && (*current_state == 16 || *current_state ==17 || *current_state ==20 || *current_state ==21 || *current_state ==22 ||*current_state ==24 || *current_state ==25 || *current_state ==26 ||*current_state == 27 || *current_state ==28 || *current_state ==29 || *current_state ==31 || *current_state ==32 || *current_state ==33 || *current_state ==34 ||*current_state ==35 || *current_state ==36 ||*current_state ==38 || *current_state ==39 || *current_state ==40 || *current_state ==42 || *current_state ==43 || *current_state ==44|| *current_state ==46||*current_state ==47||*current_state ==48||*current_state ==49||*current_state ==50||*current_state ==52||*current_state ==53||*current_state ==54||*current_state ==55||*current_state ==57||*current_state ==70)){ // .<혹은 >만 읽은 state에서 <나>가 먼저 분류되어야하는 경우.
      string tmp= " COMPOP | ";
      tmp.append(1,lex_queue.front());
      lex_queue.pop();
      gl_queue.push(tmp);
      
      lex_queue.push(temp);
      
  }
  
  else if((*former_state ==10||*former_state ==11) && (*current_state ==7||*current_state ==14||*current_state ==58) ){// <나> 뒤에 ,    \혹은 ! 혹은 " 가 나온 경우.
      string tmp= " COMPOP | ";
      tmp.append(1,lex_queue.front());
      lex_queue.pop();
      gl_queue.push(tmp);
      
      lex_queue.push(temp);
      
  }
  
  else if((*former_state ==10||*former_state ==11) && (*current_state ==61||*current_state ==62||*current_state ==63) ){//.   <나> 뒤에서, int,float이 시작되는 state로 가는경우.
      string tmp= " COMPOP | ";
      tmp.append(1,lex_queue.front());
      lex_queue.pop();
      gl_queue.push(tmp);
      
    
      lex_queue.push(temp);
      
      
  }
  else if(*former_state ==10 && *current_state==11 ){// <나> 뒤에 ,    \혹은 ! 혹은 " 가 나온 경우.
      string tmp= " COMPOP | ";
      tmp.append(1,lex_queue.front());
      lex_queue.pop();
      gl_queue.push(tmp);
      
      lex_queue.push(temp);
      
  }
    else if(*former_state ==11 && *current_state==10 ){// <나> 뒤에 ,    \혹은 ! 혹은 " 가 나온 경우.
        string tmp= " COMPOP | ";
        tmp.append(1,lex_queue.front());
        lex_queue.pop();
        gl_queue.push(tmp);
        
        lex_queue.push(temp);
        
    }
    
 
  
  else if(*former_state ==15 && (*current_state == 16 ||*current_state == 17 ||*current_state ==20 || *current_state ==21 || *current_state ==22 ||*current_state ==24 || *current_state ==25 || *current_state ==26 || *current_state ==27 || *current_state ==28 || *current_state ==29 || *current_state ==31 || *current_state ==32 || *current_state ==33 || *current_state ==34 || *current_state ==35 || *current_state ==36 ||*current_state ==38 || *current_state ==39 || *current_state ==40 || *current_state ==42 || *current_state ==43 || *current_state ==44|| *current_state ==46||*current_state ==47||*current_state ==48||*current_state ==49||*current_state ==50||*current_state ==52||*current_state ==53||*current_state ==54||*current_state ==55||*current_state ==57||*current_state ==70)){
      string tmp= " ASGN | ";
             tmp.append(1,lex_queue.front());
             lex_queue.pop();
             gl_queue.push(tmp);
             
             lex_queue.push(temp);
  }
  
  else if(*former_state ==15 && (*current_state == 7||*current_state ==14||*current_state ==58)){
         
         string tmp= " ASGN | ";
         tmp.append(1,lex_queue.front());
         lex_queue.pop();
         gl_queue.push(tmp);
         
         lex_queue.push(temp);
         
         
     }
  
  else if(*former_state ==15 && (*current_state == 61||*current_state == 62||*current_state == 63)){
         string tmp= " ASGN | ";
                tmp.append(1,lex_queue.front());
                lex_queue.pop();
                gl_queue.push(tmp);
                
      lex_queue.push(temp);// INt 혹은 float가 될 -니까.
         
         
         
     }

  
  else if((*former_state == 61||*former_state == 62||*former_state == 63||*former_state == 64||*former_state == 65||*former_state == 66||*former_state == 67||*former_state == 68) && (*current_state == 1||*current_state ==2||*current_state ==3||*current_state ==4||*current_state ==5||*current_state ==6||*current_state ==8||*current_state ==9||*current_state ==12||*current_state ==60)){
      if(*former_state==61){
      string tmp= " ARITHOP | ";
                tmp.append(1,lex_queue.front());
                lex_queue.pop();
                gl_queue.push(tmp);
                
      string tmp2 = s[*current_state]+" | ";
      tmp2.append(1,temp);
      gl_queue.push(tmp2);
      }
      else if (*former_state == 62||*former_state == 63){
          string tmp= " INT | ";
         while(true){
                   if(!lex_queue.empty()){
                       tmp.append(1,lex_queue.front());
                       lex_queue.pop();
                   }
                   
                   else{
                       break;
                   }
               
               }
                    gl_queue.push(tmp);
          string tmp2 = s[*current_state]+" | ";
                 tmp2.append(1,temp);
                 gl_queue.push(tmp2);
      }
      
      else if (*former_state == 64){
          string tmp=" ARITHOP | -";
          gl_queue.push(tmp);
          string tmp2=" INT | 0";
           gl_queue.push(tmp2);
          string tmp3= s[*current_state]+" | ";
          tmp3.append(1,temp);
          gl_queue.push(tmp3);
      }
      else if (*former_state ==66||*former_state ==68){
          string tmp=" FLOAT | ";
              while(true){
                  if(!lex_queue.empty()){
                      tmp.append(1,lex_queue.front());
                      lex_queue.pop();
                  }
                  
                  else{
                      break;
                  }
              
              }
                   gl_queue.push(tmp);
          string tmp2 = s[*current_state]+" | ";
          tmp2.append(1,temp);
          gl_queue.push(tmp2);
          }
      else if (*former_state == 67){
          string tmp=" FLOAT | ";
                     while(true){
                                
                                if(!lex_queue.empty()){
                                    tmp.append(1,lex_queue.front());
                                    lex_queue.pop();
                                    if(lex_queue.size()==1){
                                        break;
                                    }
                                }
                                
                                else{
                                            
                                    break;
                                }
                            
                            }
                                       
                                 gl_queue.push(tmp);
          string tmp2=" INT | 0";
          gl_queue.push(tmp2);
          lex_queue.pop();
          string tmp3= s[*current_state]+" | ";
          tmp3.append(1,temp);
          gl_queue.push(tmp3);
      }
     
      else{}
  }
  
        else if((*former_state == 61||*former_state ==62||*former_state ==63||*former_state ==64||*former_state ==65||*former_state ==66||*former_state ==67||*former_state ==68) && (*current_state == 10||*current_state ==11||*current_state ==15||*current_state ==7||*current_state ==14||*current_state ==58||*current_state ==16 || *current_state ==17 || *current_state ==20 || *current_state ==21 || *current_state ==22 ||*current_state ==24 || *current_state ==25 || *current_state ==26 || *current_state ==27 || *current_state ==28 || *current_state ==29 || *current_state ==31 || *current_state ==32 || *current_state ==33 ||*current_state == 34 || *current_state ==35 || *current_state ==36 ||*current_state ==38 || *current_state ==39 || *current_state ==40 || *current_state ==42 || *current_state ==43 || *current_state ==44|| *current_state ==46||*current_state ==47||*current_state ==48||*current_state ==49||*current_state ==50||*current_state ==52||*current_state ==53||*current_state ==54||*current_state ==55||*current_state ==57||*current_state ==70)){
           
            if(*former_state==61){
            string tmp= " ARITHOP | ";
                      tmp.append(1,lex_queue.front());
                      lex_queue.pop();
                      gl_queue.push(tmp);
                      
                lex_queue.push(temp);
            }
            else if (*former_state ==62||*former_state ==63){
                string tmp= " INT | ";
               while(true){
                         if(!lex_queue.empty()){
                             tmp.append(1,lex_queue.front());
                             lex_queue.pop();
                         }
                         
                         else{
                             break;
                         }
                     
                     }
                          gl_queue.push(tmp);
                lex_queue.push(temp);
            }
            
            else if (*former_state == 64){
                string tmp=" ARITHOP | -";
                gl_queue.push(tmp);
                string tmp2=" INT | 0";
                 gl_queue.push(tmp2);
                lex_queue.push(temp);
            }
            else if (*former_state ==66||*former_state ==68){
                string tmp=" FLOAT | ";
                    while(true){
                        if(!lex_queue.empty()){
                            tmp.append(1,lex_queue.front());
                            lex_queue.pop();
                        }
                        
                        else{
                            break;
                        }
                    
                    }
                         gl_queue.push(tmp);
                lex_queue.push(temp);
                }
            else if (*former_state == 67){
                string tmp=" FLOAT | ";
                           while(true){
                                      
                                      if(!lex_queue.empty()){
                                          tmp.append(1,lex_queue.front());
                                          lex_queue.pop();
                                          if(lex_queue.size()==1){
                                              break;
                                          }
                                      }
                                      
                                      else{
                                                  
                                          break;
                                      }
                                  
                                  }
                                             
                                       gl_queue.push(tmp);
                string tmp2=" INT | 0";
                gl_queue.push(tmp2);
                lex_queue.pop();
                lex_queue.push(temp);
            }
            else{}
            
        }
   ////////////////숫자관련 state들.   ->.    INT 혹은 FLOAT가 될 가능성이 있는 모든 state들 이 자기들끼리 돌고 도는 경우들이 밑의 경우.
        else if( *former_state ==61 && *current_state== 61){
            string tmp= " ARITHOP | ";
                  tmp.append(1,lex_queue.front());
                  lex_queue.pop();
                  gl_queue.push(tmp);
                  
            lex_queue.push(temp);
        }
 else if( *former_state ==61 && (*current_state== 62 || *current_state== 64)){
       lex_queue.push(temp);
  }
 else if( *former_state ==62 && (*current_state== 62 || *current_state== 65)){
       lex_queue.push(temp);
  }
else if( *former_state ==63 && *current_state== 62){//01  03같은 상태이므로 0을 INT처리해주고 그 뒤는 INT혹은 FLOAT로 될 상태
       string tmp= " INT | ";
       tmp.append(1,lex_queue.front());
       lex_queue.pop();
       gl_queue.push(tmp);
      lex_queue.push(temp);
  }
  else if( *former_state ==63 && *current_state== 63){//00 이므로 0 하나를 INT로 처리. 그 뒤는 다시 0일지 FLOAT일지 모르는 상태
      string tmp= " INT | ";
      tmp.append(1,lex_queue.front());
      lex_queue.pop();
      gl_queue.push(tmp);
      lex_queue.push(temp);
  }
   else if( *former_state ==63 && *current_state== 65){//0.
       lex_queue.push(temp);
  }
 else if( *former_state ==64 && *current_state== 61){
      string tmp=" ARITHOP | -";
       gl_queue.push(tmp);
      string tmp2=" INT | 0";
        gl_queue.push(tmp2);
      lex_queue.pop();lex_queue.pop();
       lex_queue.push(temp);
  }
  else if( *former_state ==64 && *current_state== 62){
      string tmp=" ARITHOP | -";
        gl_queue.push(tmp);
      string tmp2=" INT | 0";
         gl_queue.push(tmp2);
       lex_queue.pop();lex_queue.pop();
      lex_queue.push(temp);
  }
else if( *former_state ==64 && *current_state== 63){
      string tmp=" ARITHOP | -";
         gl_queue.push(tmp);
      string tmp2=" INT | 0";
          gl_queue.push(tmp2);
        lex_queue.pop();lex_queue.pop();
       lex_queue.push(temp);
  }
 else if( *former_state ==64 && *current_state== 65){
       lex_queue.push(temp);
  }
 else if( *former_state ==65 && *current_state== 66){
       lex_queue.push(temp);
  }
  else if( *former_state ==66 && *current_state== 67){
       lex_queue.push(temp);
  }
   else if( *former_state ==66 && *current_state== 68){
       lex_queue.push(temp);
  }
    else if( *former_state ==67 && *current_state== 65){//////////////
          string tmp=" FLOAT | ";
                        while(true){
                            
                            if(!lex_queue.empty()){
                                tmp.append(1,lex_queue.front());
                                lex_queue.pop();
                                if(lex_queue.size()==1){
                                    break;
                                }
                            }
                            
                            else{
                                        
                                break;
                            }
                        
                        }
                             gl_queue.push(tmp);
          lex_queue.push(temp);
     }
  else if( *former_state ==67 && *current_state== 67){
       lex_queue.push(temp);
  }
else if( *former_state ==67 && *current_state== 68){
       lex_queue.push(temp);
  }
    else if( *former_state ==68 && *current_state== 65){////////
         string tmp=" FLOAT | ";
                      while(true){
                          
                          if(!lex_queue.empty()){
                              tmp.append(1,lex_queue.front());
                              lex_queue.pop();
                              if(lex_queue.size()==1){
                                  break;
                              }
                          }
                          
                          else{
                                      
                              break;
                          }
                      
                      }
                           gl_queue.push(tmp);
        lex_queue.push(temp);
    }
 else if( *former_state ==68 && *current_state== 67){
       lex_queue.push(temp);
  }
  else if( *former_state ==68 && *current_state== 68){
       lex_queue.push(temp);
  }
  
         
            else { //사실 이런 경우는 없을거라고 생각. but 혹시 몰라.
                cout<<"ERROR"<<endl;
                exit(0);
            }
      
  
}


int main(int argc, const char * argv[]) {
    //opening txt file
    map_initialization_m();//map STL을 이용하여 모든 가능한 입력symbol들을 숫자에 맵핑한 것을 헤더파일에서 갖고옴.
    map_initialization_s();//map STL을 이용하여 모든 state들을 숫자에 맵핑한 것을 헤더파일에서 갖고옴.
    string input_file_name = argv[1];
    ifstream input_file;
    input_file.open(input_file_name); // directly open stream without specific function
    if(!input_file) { //인풋 파일이 안 열린 경우
        cout << "error occured while opening file" << endl;
        exit(1);
    }
    int former_state = 0; //이전 state
    int current_state = 0;//input파일에서 읽어온 하나의 문자로 인해, 이전 state가 이동한 현재의 state
    //when stream is opened, read text per character unit.
    char read_word; //input파일에서 하나씩 글자를 읽어와 저장할 변수
    
        do{ //하나씩 input파일에서 글자를 읽으면서 lexical_analyzer()함수에 전달해주어 필요한 과정을 거치고오면, 다시 또 다음 글자를 input파일에서 읽어와 함수에 전달.
            read_word=input_file.get();
    ////////////////////////////////////////////밑의 cout 세줄은 오류를 잡기위해 써놓은 코드이므로 지워도 프로그램 돌아가는 것과 무관하다.//////////////////////////////////////////////////////////////////////////////////
            cout << "on main function _ ";
            cout << "former_state : " << former_state << "  current_state : " << current_state;
            cout << "  character : " << read_word << endl;
    ////////////////////////////////////////////위의 cout 세줄은 오류를 잡기위해 써놓은 코드이므로 지워도 프로그램 돌아가는 것과 무관하다.//////////////////////////////////////////////////////////////////////////////////
            lexical_analyzer(read_word, &former_state, &current_state);
        }while (!input_file.eof());
    
    input_file.close();
    result_print(input_file_name); //gl_queue에 있는 모든 원소들 (" TokenNAME | lexeme" 형태)을 output파일에 출력해주고,ws가 output파일에 안 들어가게 제거.
    cout << "terminated" << endl;
    return 0;
}


//
//  symbolMap.h
//  LexicalAnal
//
//  Created by 양준식 on 2020/05/09.
//  Copyright © 2020 양준식. All rights reserved.
//

#ifndef symbolMap_h
#define symbolMap_h




#include <iostream>
using namespace std;
#include <string>
#include <fstream>
#include <map>

map<char,int> m;//각각의 입력 심볼들에 번호를 차례로 지정할 것이다. 이 번호들은 MergedTable 배열에서 column번호로 역할을 한다.

void map_initialization_m () {//먼저 우리는 문자 하나, 하나를 읽어와서 state의 변화를 관찰해야하므로, 입력받을 수 있는 모든 문자들을 0번부터 40번까지 맵핑하였습니다.
    m['a']=0;
    m.insert(make_pair('b',1));
    m.insert(make_pair('c',2));
    m.insert(make_pair('e',3));
    m.insert(make_pair('f',4));
    m.insert(make_pair('h',5));
    m.insert(make_pair('i',6));
    m.insert(make_pair('l',7));
    m.insert(make_pair('n',8));
    m.insert(make_pair('o',9));
    m.insert(make_pair('r',10));
    m.insert(make_pair('s',11));
    m.insert(make_pair('t',12));
    m.insert(make_pair('u',13));
    m.insert(make_pair('w',14));
    
    m['d']=15;  m['g']=15; m['j']=15; m['k']=15; m['m']=15; m['p']=15;   //d,g,j,k,m,p,q,v,x,y,z는 if,else와 같은 특별한 case에 전혀 속하는 일이 없어서
    m['q']=15; m['v']=15; m['x']=15; m['y']=15; m['z']=15;               // state 전이에 있어서는 뭉뚱그려 한 종류의 input으로 받아들여도 된다.
    
    m['A']=16; m['B']=16; m['C']=16; m['D']=16; m['E']=16; m['F']=16; m['G']=16; m['H']=16; m['I']=16; m['J']=16; m['K']=16;
    m['L']=16; m['M']=16; m['N']=16; m['O']=16; m['P']=16; m['Q']=16; m['R']=16; m['S']=16; m['T']=16; m['U']=16; m['V']=16;
    m['W']=16; m['X']=16; m['Y']=16; m['Z']=16;                      //[A-Z]도 위와 같은 이유로 state전이에 있어서 뭉뚱그려 생각해주어도 된다.
    
    m.insert(make_pair('0',17));
    
    m['1']=18; m['2']=18; m['3']=18; m['4']=18; m['5']=18; m['6']=18; m['7']=18; m['8']=18; m['9']=18;
    
    m.insert(make_pair('.',19)); //Decimal point
    m.insert(make_pair('-',20)); //MINUS
    m.insert(make_pair('_',21)); //BOTTOM
    m.insert(make_pair(' ',22));   m.insert(make_pair('\t',22));  m.insert(make_pair('\n',22));  //blank, \t, \n
    m.insert(make_pair('\\',23));  // reverse slash
    m.insert(make_pair('<',24));
    m.insert(make_pair('>',25));
    m.insert(make_pair('!',26));
    m.insert(make_pair('=',27));
    
    m.insert(make_pair('(',28));
    m.insert(make_pair(')',29));
    m.insert(make_pair('{',30));
    m.insert(make_pair('}',31));
    
    m.insert(make_pair(';',32));
    m.insert(make_pair('&',33));
    m.insert(make_pair('|',34));
    m.insert(make_pair('+',35));
    m.insert(make_pair('*',36));
    m.insert(make_pair('/',37));
    m.insert(make_pair(',',38)); //COMMA
    m.insert(make_pair('"',39));
    m['?']=40;  m['@']=40; m['#']=40; m['$']=40; m['%']=40; m['^']=40; m['[']=40;m[']']=40;//1~40번에 해당되지 않는 symbol들은 모두 41번으로 처리해줄 것임.
    m[':']=40;m['~']=40;
    //////////////////////////////////////////////////////////////////////// SYMBOL MAP END LINE/////////////////////////////////////////////////////////////////////////
}
   

 
 /////////////////////////////////////////////////////////////////////////////////////////////////////////////// State MAP //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
map<int,string> s;//각각의 입력 심볼들에 번호를 차례로 지정할 것이다.

void map_initialization_s () { // DFA에서 필요한 71개의 모든 state들에 번호를 차례로 맵핑해줌. 이는 해당 상태에서 입력이 끝나버렸을 경우 해당 state가 되어야 할 TOKENNAME을 기준으로 하였다.
    // 예를 들면, 0만 입력받은 경우에는, 이것이 INT인 0이 될 지, 뒤에.이 나와서 FLOAT이 될 지 모른다. 하지만 여기서 입력이 끝나면 이 state는 INT가 된다. 따라서 INT로 맵핑.
    s.insert(make_pair(0,"T0"));
    s[1]="LPAREN";
    s[2]="RPAREN";
    s[3]="LBRACE";
    s[4]="RBRACE";
    s[5]="COMMA";
    s[6]="TERM";
    s[7]="Going (\t | \n ) state";
    s[8]="WS";
    s[9]="WS";
    s[10]="COMPOP";
    
    s[11]="COMPOP";
    s[12]="BITOP";
    s[13]="COMPOP";
    s[14]="Going != state";
    s[15]="ASGN";
    s[16]="ID";
    s[17]="ID";
    s[18]="VAR";
    s[19]="IF";
    s[20]="ID";
    
    s[21]="ID";
    s[22]="ID";
    s[23]="ELSE";
    s[24]="ID";
    s[25]="ID";
    s[26]="FOR";
    s[27]="ID";
    s[28]="ID";
    s[29]="ID";
    s[30]="VAR";
    
    s[31]="ID";
    s[32]="ID";
    s[33]="ID";
    s[34]="ID";
    s[35]="ID";
    s[36]="ID";
    s[37]="BOOL";
    s[38]="ID";
    s[39]="ID";
    s[40]="ID";
    
    s[41]="VAR";
    s[42]="ID";
    s[43]="ID";
    s[44]="ID";
    s[45]="VAR";
    s[46]="ID";
    s[47]="ID";
    s[48]="ID";
    s[49]="ID";
    s[50]="ID";
    
    s[51]="RETURN";
    s[52]="ID";
    s[53]="ID";
    s[54]="ID";
    s[55]="ID";
    s[56]="WHILE";
    s[57]="ID";
    s[58]="Starting LString";
    s[59]="STRING";
    s[60]="ARITHOP";
    
    s[61]="ARITHOP";
    s[62]="INT";
    s[63]="INT";
    s[64]="ARITHOP + INT";
    s[65]="( FLOAT ) .";
    s[66]="FLOAT";
    s[67]="( FLOAT )0.X0)";
    s[68]="FLOAT";
    s[69]="FAIL";
    s[70]="ID";
}
 
#endif /* symbolMap_h */

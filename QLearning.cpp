#include <iostream>
#include <vector>
#include <fstream>
#include <random>
#include <string>


using namespace std;






class state
{
    public:
    int X;
    int Y;
    float R,U,Qu = 0, Ql = 0, Qr = 0, Qd = 0,Qt=0;
    float Nu=0,Nd=0,Nl=0,Nr=0;
    char D ='^';
    char type;
    state(){};
    state(int x, int y){ X = x; Y = y; };
    state(int x, int y, char d){ X = x; Y = y; D =d;};
    state(int x, int y, float u, float r, char t){ X = x; Y = y; U = u; R =r; type =t;};

    float maxQ();
    char policy();
    void updateQ(char d,float Qmax, float E,float G);
    void updateQTerminal(float E,float G);
    void updateN(char d);

    void print()
    {
        cout<<"X = "<<X<<" Y = "<<Y<<" U = "<<U<<" R = "<<R<<" Type = "<<type<<endl;
    }

};

string alignStr(float f)
{
    string s1(to_string(f));
    string s;
    s = s1.substr(0,6);
    //cout<<s1<<endl;
    while(s.length() <6 )
    {
        s += " ";
    }
    return s;
}

bool notInSet(int x, int y, vector<state> T)
{
    for ( int i=0;i<T.size();i++)
    {
        if(T[i].X==x && T[i].Y==y) return false;
    }

    return true;
}


class Simulator
{
    vector<state> possible_state;
    int worldX, worldY;
    vector<state>  world, stateTmp;
    vector<float>  P = {0,0,0,0};
    float R,G,E;
    state S;
    vector<state> T,F,B;

    
    public:
        Simulator(string fileName);
        state take_action(int x, int y, char a, vector<state> sT );
        void printSimulate();
        state startState();
        float getE();
        float getG();


};


void printMap(vector<state> map);
state returnStateQs(char action, vector<state> map);
float errorValue(vector<state> map);

template<typename T>
T randomE(T from, T to)
{
    random_device rand_dev;
    mt19937 generater(rand_dev());
    uniform_real_distribution<T> distr(from, to);
    return distr(generater);
}

char randomAction()
{
    double ran = randomE(0.0,4.0);
    if ( ran<=1) return '^';
    else if (ran <=2) return '<';
    else if (ran <=3) return '>';
    else return 'v';
}


int main ( int argc, char **data)
{
    Simulator s(data[1]);
    s.printSimulate();
    char action ;
    char preAction;
    int xCurrent, yCurrent;
    state nState;
    vector<state> map{s.startState()};
    float E = s.getE();
    float G = s.getG();
    xCurrent = map[0].X;
    yCurrent = map[0].Y;
    float Qmax;
    cout<<endl;
    cout<<endl;
    cout<<E<<G<<endl;
    int aha;
    double sumCur = 0, sumPre = 0;
    double randomExploration;
    action = '^';
    int iteration = 0;


    ofstream outfile;
    outfile.open("dataOut.dat");


try {
    while(iteration < 10000)
    {
        if(nState.type == 'T')
        {
            xCurrent = map[0].X;
            yCurrent = map[0].Y;
            action   = map[0].policy();
            outfile<<iteration+1<<" ";
            for( auto elm:map){
                string str1(to_string(elm.maxQ()));

                outfile << str1<<" ";
                
            }
             outfile<<endl;
            //if((sumCur - sumPre) < 0.00001) break;
            //sumPre = sumCur;
            iteration++;
        }

        randomExploration = randomE(0.00,1.00);
        if(randomExploration < s.getE())    action = randomAction();

        //cout<<"randoom number "<<randomExploration<<"and E "<<s.getE()<<endl;

        //cin>>aha;
        //cout<<" Current location: "<<xCurrent<<" "<<yCurrent<<endl;
        //printMap(map);
        //cout<<"Action "<< action<<endl;
        

        
        nState = s.take_action(xCurrent,yCurrent,action,map);
        if(notInSet(nState.X,nState.Y,map)) map.push_back(nState);
        Qmax = nState.maxQ();
        //cout<<"Qnax ="<<nState.type<<"Qnax ="<<nState.X<<"Qnax ="<<nState.Y<<endl;
        for( auto& elm:map)
        {
            if(elm.X == xCurrent&& elm.Y == yCurrent)
            {
                elm.updateN(action);
                if(elm.type =='T')
                    elm.updateQTerminal(0.5,G);
                else
                    elm.updateQ(action,Qmax,E,G);
                
                
            }
            else if(elm.X == nState.X && elm.Y == nState.Y && nState.type == 'T') 
            {
                elm.updateN(action);
                //cout<<xCurrent<<yCurrent<<endl;
                //cout<<"type: "<<elm.type<<endl;
                elm.updateQTerminal(0.5,G);
               
            }
            sumCur+= elm.maxQ();
                
        }        
        

        action = nState.policy();






        xCurrent = nState.X;
        yCurrent = nState.Y;
        
    



        
      //  cout<<iteration<<endl;

    }

   
       
      
} catch (const char* msg) {
    cerr << msg << endl;
}

printMap(map);
   outfile.close();

}








Simulator::Simulator(string fileName){ 


      ifstream myfile;
    myfile.open(fileName,ios::in);
     char Parametr;
     int x,y;
     float u;
   while(1)
   {
         if(myfile.eof())
       {
           break;
       }
       myfile>>Parametr;
        //cout<<Parametr<<endl;

        switch(Parametr)
        {
        case 'W':
            {

                myfile>>worldX>>worldY;;
               // W1(x,y);
                break;
            }
        case 'S':
            {

                myfile>>x>>y;
                S.X =x;
                S.Y =y;

                break;
            }
        case 'P':
            {
                float P1,P2,P3;
                myfile>>P[0]>>P[1]>>P[2];
                break;
            }
        case 'R':
            {
                myfile>>R;
                break;
            }
        case 'E':
            {
                myfile>>E;
                break;
            }
        case 'G':
            {
                myfile>>G;
                break;
            }
        case 'T':
            {
                myfile>>x>>y>>u;
                T.push_back({x,y,u,u,'T'});
                break;
            }
        case 'F':
            {
                myfile>>x>>y;
                F.push_back({x,y,'F'});  
                break;
            }
        case 'B':
            {
                float Br;
                myfile>>x>>y>>Br;
                B.push_back({x,y,0,Br,'N'});
                break;
            }    
        default:
            {
                myfile>>x;
                break;
            }
        }


   }
   myfile.close();

     for ( int j=1;j<worldY+1;j++)
        for( int i=1;i<worldX+1;i++)
    {
        if(notInSet(i,j,F)&&notInSet(i,j,T)&&notInSet(i,j,B))
        {
            if(i == S.X && j == S.Y)
            {
                world.push_back({i,j,0,R,'S'});
                S.D = 'S';
                S.R = R;
                S.U = 0;
            }
            else
                world.push_back({i,j,0,R,'N'});
        }

    }
    for( int i=0;i<B.size();i++)
    {
        world.push_back(B[i]);
    }
     for( int i=0;i<T.size();i++)
    {
        world.push_back(T[i]);
    }






       /*      worldX = X; 
            worldY = Y; 
            F = nF;
            T = nT;

            for( int i=0 ; i < X; ++i)
                for( int j = 0 ; j < Y; ++j)
                        if(notInSet(i,j,F)) possible_state.push_back({i,j});
 */
            
}

state Simulator::take_action(int x, int y, char a, vector<state> sT )
        {

            int xtmp,ytmp,newX,newY;
            switch (a)
            {
            case '^' :
                {
                    xtmp =0;
                    ytmp =1;
                    break;
                }
            case 'v' :
                {
                    xtmp =0;
                    ytmp =-1;
                    break;
                }
            case '<' :
                {
                    xtmp =-1;
                    ytmp =0;
                    break;
                }
            case '>' :
                {
                    xtmp =1;
                    ytmp =0;
                    break;
                }
            default:
                {
                    cout<<"ERROR!!"<<endl;
                }
            }

            newX = x + xtmp;
            newY = y + ytmp;
            if(notInSet(newX,newY,world)) {newX = x; newY = y;};

            for( auto e:sT)
            {
                if(e.X == newX && e.Y==newY) return e;
            }

            for( auto elm:world)
            {
                if(elm.X == newX && elm.Y==newY) return elm;
            }

}

void Simulator::printSimulate()
    {
        for ( auto& e: world)
        {
            e.print();
        }
    }
float Simulator::getE() {return E;}
float Simulator::getG() {return G;}

float state::maxQ()
{
    float max = -99999;
    if(Qu > max) {max =Qu;};
    if(Qd > max) {max =Qd;};
    if(Ql > max) {max =Ql;};
    if(Qr > max) {max =Qr;};
    if (type == 'T')
        return Qt;
    return max; 
} 


char state::policy()
{
    char a;
    float max = -99999;
    if(Qu > max) {max =Qu; a = '^';};
    if(Qd > max) {max =Qd; a = 'v';};
    if(Ql > max) {max =Ql; a = '<';};
    if(Qr > max) {max =Qr; a = '>';};
    

    return a; 
}  

 void state::updateQ(char d,float Qmax, float E,float G)
    {
                        
        switch(d)
        {
            case '^':
            {
                float Nlearn = 1/Nu;
               // cout<<"index learn"<<Nlearn<<endl;
                Qu += Nlearn*(R + G*Qmax -   Qu) ;

                break;
            }
            case '<':
            {
                float Nlearn = 1/Nl;
                Ql += Nlearn*(R + G*Qmax -   Ql) ;
                break;
            }
            case 'v':
            {
                float Nlearn = 1/Nd;
                //cout<<"index learn"<<(R + G*Qmax -   Qd) <<endl;
                Qd += Nlearn*(R + G*Qmax -   Qd) ;
                break;
            }
            case '>':
            {
                float Nlearn = 1/Nr;
                Qr += Nlearn*(R + G*Qmax -   Qr) ;
                break;
            }
            default:
                {
                    cout<<"ERRORT1!!"<<endl;
                }
        }
    }
void state::updateN(char d)
{
     switch(d)
        {
            case '^':
            {
                //cout<<E<<R<<G<<Qu<<endl;
                Nu++;
                break;
            }
            case '<':
            {
                Nl++;
                break;
            }
            case 'v':
            {
                Nd++;
                break;
            }
            case '>':
            {
                Nr++;
                break;
            }
            default:
                {
                    throw " Wrong updateN!";
                }
        }
    
}
void state::updateQTerminal(float E,float G)
{
    if(type != 'T')
       throw " Incorrect datatype in updateQTerminal!";
    else
    {
       // cout<<"HELLO!!"<<endl;
        //Qt = 0.5*(R + G*Qt);
        Qt = U;
        
    }        
}
state Simulator::startState()
{
    return S;
}

void printMap(vector<state> map)
{
    for(auto elm:map)
    {
        cout<<" X = "<<elm.X<<" Y = "<<elm.Y<<" Ql = "<<elm.Ql<<" Qr = "<<elm.Qr<<" Qu= "<<elm.Qu<<" Qd = "<<elm.Qd<<" Qt= "<<elm.Qt<<endl;
    }
}
float errorValue(vector<state> map)
{
    state Up,Left,Right,Down;
    for(auto elm:map)
    {
        // Up = returnStateQs('^',elm,map);
        // Left = returnStateQs('<',elm,map);
        // Right = returnStateQs('>',elm,map);
        // Down = returnStateQs('v',elm,map);


    }

}

state returnStateQs(char action,state current, vector<state> map)
{
    int xtmp,ytmp;
    switch (action)
    {
    case '^' :
        {
            xtmp =0;
            ytmp =1;
            break;
        }
    case 'v' :
        {
            xtmp =0;
            ytmp =-1;
            break;
        }
    case '<' :
        {
            xtmp =-1;
            ytmp =0;
            break;
        }
    case '>' :
        {
            xtmp =1;
            ytmp =0;
            break;
        }
    default:
        {
            cout<<"ERROR!!"<<endl;
        }
    }


    for (int i=0; i< map.size();i++)
    {
        if(map[i].X == current.X + xtmp && map[i].Y == current.Y + ytmp  )
        {
             return map[i];
        }
    }
    return current;;
}
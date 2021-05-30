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
    float Nu=0,Nd=0,Nl=0,Nr=0,Nt =0;
    char D ='^';
    char type;
    state(){};
    state(int x, int y){ X = x; Y = y; };
    state(int x, int y, char d){ X = x; Y = y; D =d;};
    state(int x, int y, float u, float r, char t){ X = x; Y = y; U = u; R =r; type =t;};

    float maxQ();
    char policy();
    void updateQ(char d,float Qmax,float G);
    void updateQTerminal(float G);
    void updateN(char d);

    void print()
    {
        cout<<"X = "<<X<<" Y = "<<Y<<" U = "<<U<<" R = "<<R<<" Type = "<<type<<endl;
    }

};

string alignStr(float f);
bool notInSet(int x, int y, vector<state> T);


class Simulator
{
    vector<state> possible_state;
        vector<state>  world, stateTmp;
    vector<float>  P = {0,0,0,0};
    float R,G,E;
    state S;
    vector<state> T,F,B;

    
    public:
        int worldX, worldY;

        Simulator(string fileName);
        state take_action(int x, int y, char a, vector<state> sT );
        void printSimulate();
        state startState();
        void setE(float e);
        void setG(float g);
        float getE();
        float getG();
        vector<state> getF();


};

state returnElm(int x, int y, vector<state> T);
void printMap(vector<state> map);
state returnStateQs(char action, vector<state> map);
float errorValue(vector<state> map);
char randomAction();
template<typename T>
T randomE(T from, T to)
{
    random_device rand_dev;
    mt19937 generater(rand_dev());
    uniform_real_distribution<T> distr(from, to);
    return distr(generater);
}




int main ( int argc, char **data)
{
    



    Simulator s(data[1]);
    s.printSimulate();
    
    state nState;
    vector<state> map{s.startState()};



    if(argc > 2) 
    {
        string sData(data[2]); 
        float G = stof(sData);
        s.setG(G);
    }
    if (argc > 3 )
    {
        string sData(data[3]); 
        float E = stof(sData);
        s.setE(E);
    }


    char action ;
    int xCurrent, yCurrent;
    xCurrent = map[0].X;
    yCurrent = map[0].Y;
    float Qmax;
    cout<<endl;
    cout<<endl;

    cout<<s.getE()<<s.getG()<<endl;

    double randomExploration;
    action = '^';
    int iteration = 0;


    ofstream outfile;
    outfile.open("datatmp.dat");


try {
    while(iteration < 100000)
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
            iteration++;
        }

        randomExploration = randomE(0.00,1.00);
        //cout<<"rand: "<<randomExploration<<endl; 
        if(randomExploration < s.getE()) action = randomAction();

        nState = s.take_action(xCurrent,yCurrent,action,map);
        if(notInSet(nState.X,nState.Y,map)) map.push_back(nState);
        Qmax = nState.maxQ();
        for( auto& elm:map)
        {
            if(elm.X == xCurrent&& elm.Y == yCurrent)
            {
                elm.updateN(action);

                elm.updateQ(action,Qmax,s.getG());
                
                
            }
            // else if(elm.X == nState.X && elm.Y == nState.Y && nState.type == 'T') 
            // {
            //     elm.updateN(action);
            //     //cout<<xCurrent<<yCurrent<<endl;
            //     //cout<<"type: "<<elm.type<<endl;
            //     elm.updateQTerminal(s.getG());
               
            // }
 
                
        }        
        

        action = nState.policy();

        xCurrent = nState.X;
        yCurrent = nState.Y;


    }

   
       
      
} catch (const char* msg) {
    cerr << msg << endl;
}
float tmp;
  outfile.close();

    string line ="";

    ofstream outfile1;
    outfile1.open("dataOut.dat");

    ifstream infile1;
    infile1.open("datatmp.dat");

    outfile1<<"State ";
        for( auto elm:map)
        {   
            tmp = elm.X+ float(elm.Y)/10;
            outfile1<<"State=("<<tmp<<") ";

        }
        outfile1<<endl;
    while(true)
    {
        getline(infile1,line);
        outfile1<<line;
        if(!infile1.good()) break;
        outfile1<<endl;
    }

infile1.close();





printMap(map);
vector<state> stateF = s.getF();
for( auto elm: stateF)
    map.push_back(elm);

int M=s.worldX,N=s.worldY;
    float tmpNum;
    state temp;
    for ( int i=3*N;i>0;i--)
    {
        if(i%3 ==0)
        for ( int j=0;j<M;j++)
            {
        cout<<"-------";

            }
  
        else if(i%3 ==1)    
        {
            
            for ( int j=0;j<M;j++)
            {
                temp = returnElm(j+1,(i-1)/3+1,map);
             
                // cout<<"   "<<tmp.D<<"  |";
                cout<<alignStr(temp.maxQ())<<"|";

            }
        }
        else
        {
             for ( int j=0;j<M;j++)
            {
                temp = returnElm(j+1,(i-2)/3+1,map);
                //cout<<tmp.X<<tmp.Y<<"    "<<(i-2)/3+1<<j+1<<"  ";
                if(temp.type == 'T' || temp.type =='F')
                    cout<<"   "<<temp.type<<"  |";
                else
                    cout<<"   "<<temp.policy()<<"  |";

            }
        }
            

    cout<<endl;

    }






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

        switch(Parametr)
        {
        case 'W':
            {

                myfile>>worldX>>worldY;
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
                F.push_back({x,y,0,0,'F'});  
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
void Simulator::setE(float e) { E =e;}
void Simulator::setG(float g) { G =g;}
vector<state> Simulator::getF(){
    return F;
}

float state::maxQ()
{
    float max = -99999;
    if(Qu > max) {max =Qu;};
    if(Qd > max) {max =Qd;};
    if(Ql > max) {max =Ql;};
    if(Qr > max) {max =Qr;};
    if (type == 'T')
        return U;
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

 void state::updateQ(char d,float Qmax, float G)
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
void state::updateQTerminal(float G)
{
    if(type != 'T')
       throw " Incorrect datatype in updateQTerminal!";
    else
    {
       // cout<<"HELLO!!"<<endl;
        //Qt = 0.2*(R + G*Qt);
       // cout<<Qt<<endl;
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
        cout<<" X = "<<elm.X<<" Y = "<<elm.Y<<" Ql = "<<elm.policy()<<" Qr = "<<elm.Qr<<" Qu= "<<elm.Qu<<" Qd = "<<elm.Qd<<" Qt= "<<elm.Qt<<endl;
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
    return current;
}


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
state returnElm(int x, int y, vector<state> T)
{
  for(auto elm:T)
    {
      if(elm.X == x && elm.Y == y)
        return elm;
    }
    return T[0];
}
char randomAction()
{
    double ran = randomE(0.0,4.0);
   // cout<<ran<<endl;
    if ( ran<=1) return '^';
    else if (ran <=2) return '<';
    else if (ran <=3) return '>';
    else return 'v';
    
}